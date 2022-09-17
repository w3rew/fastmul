#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include "numpy/arrayobject.h"
#include <stdio.h>

static PyObject* test(PyObject* self, PyObject* args)
{
    printf("test\n");
    Py_RETURN_NONE;
}
static_assert(sizeof(npy_float) == 4, "Float should be 32-bit");
static_assert(sizeof(npy_double) == 8, "Double should be 64-bit");

static inline float inn3_f32(float* a, float* b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static int make_mul_f32(PyArrayObject* A, PyArrayObject* B,
                        PyArrayObject* result, npy_intp* shape)
{
    npy_intp* strides = PyArray_STRIDES(A);
    if (strides[2] != 4)
        return -1;
    npy_float *A_data = (npy_float*)PyArray_BYTES(A),
          *B_data = (npy_float*)PyArray_BYTES(B),
          *res_data = (npy_float*)PyArray_BYTES(result);

    uint64_t size = shape[0] * shape[1];

#pragma omp parallel for
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t indx = i * 3;
        res_data[indx] = inn3_f32(A_data + indx, B_data);
        res_data[indx + 1] = inn3_f32(A_data + indx, B_data + 3);
        res_data[indx + 2] = inn3_f32(A_data + indx, B_data + 6);
    }


    return 0;
}

static PyObject* dot(PyObject* self, PyObject* args_tuple)
{
    PyArrayObject *A = NULL, *B = NULL;
    if (!PyArg_ParseTuple(args_tuple, "OO",
                          &A, &B)) {
        return NULL;
    }

    if (PyArray_TYPE(A) != NPY_FLOAT32 || PyArray_TYPE(B) != NPY_FLOAT32) {
        return NULL;
    }
    
    npy_intp* shape = PyArray_DIMS(A);

    //printf("shape: %ld %ld %ld\n", shape[0], shape[1], shape[2]);

    PyArrayObject *result = PyArray_Empty(3, shape, PyArray_DTYPE(A), false);
    if(make_mul_f32(A, B, result, shape))
        return NULL;
    Py_INCREF(result);
    //Py_DECREF(A);
    //Py_DECREF(B);

    return result;
    Py_RETURN_NONE;
}

static PyMethodDef fastmul_methods[] = {
    {
        .ml_name = "test",
        .ml_meth = test,
        .ml_flags = METH_VARARGS,
        .ml_doc = "Test method"
    },
    {
        .ml_name = "dot",
        .ml_meth = dot,
        .ml_flags = METH_VARARGS,
        .ml_doc = "Dot product of nxmx3 and 3x3 arrays"
    },
    {
        NULL, NULL, 0, NULL
    }
};

PyMODINIT_FUNC PyInit_fastmul()
{
    static PyModuleDef module_def = {
        .m_base = PyModuleDef_HEAD_INIT,
        .m_name = "fastmul",
        .m_size = -1,
        .m_methods = fastmul_methods
    };
    import_array();

    return PyModule_Create(&module_def);
}

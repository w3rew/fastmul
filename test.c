#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static inline float inn3_f32(float* a, float* b)
{
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static inline int make_mul(float* a, float* b, float* res, uint64_t size)
{
#pragma omp parallel for
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t indx = i * 3;
        res[indx] = inn3_f32(a + indx, b);
        res[indx + 1] = inn3_f32(a + indx, b + 3);
        res[indx + 2] = inn3_f32(a + indx, b + 6);
    }

    return 0;
}

int main()
{
    float* a = malloc(sizeof(float) * 10000 * 10000 * 3);
    float* b = malloc(sizeof(float) * 3 * 3);
    float* res = malloc(sizeof(float) * 10000 * 10000 * 3);

    make_mul(a, b, res, 10000 * 10000);


    printf("%f\n", res[0]);

    return 0;
}

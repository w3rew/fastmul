import numpy as np
import fastmul

A = np.ones((10000, 10000, 3), dtype = np.float32)
B = np.ones((3, 3), dtype = np.float32)

C = fastmul.dot(A, B)
print(C.shape)

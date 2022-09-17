import numpy as np

A = np.ones((10000, 10000, 3), dtype = np.float32)
B = np.ones((3, 3), dtype = np.float32)

C = A @ B
print(C.shape)

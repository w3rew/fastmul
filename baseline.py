import numpy as np

A = np.ones((20000, 20000, 3), dtype = np.float32)
B = np.ones((3, 3), dtype = np.float32)

C = A @ B
print(C.shape)

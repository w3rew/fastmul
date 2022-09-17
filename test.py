import unittest
import numpy as np
import fastmul

class TestMul(unittest.TestCase):

    def perform_test(self, A, B):
        gt = np.tensordot(A, B, axes = (-1, -1))
        assert(gt.dtype == np.float32)
        my = fastmul.dot(A, B)

        try:
            np.testing.assert_almost_equal(gt, my)
        except AssertionError:
            self.fail("Arrays are not close")

    def testDummy(self):
        A = np.ones((2, 2, 3), dtype = np.float32)
        B = np.ones((3, 3), dtype = np.float32)
        self.perform_test(A, B)
    def testAnotherDummy(self):
        A = np.ones((2, 2, 3), dtype = np.float32)
        B = np.ones((3, 3), dtype = np.float32)
        B[0, 1] = 0
        self.perform_test(A, B)
    def testRandom(self):
        A = np.random.uniform(0, 1, size = (1000, 1000, 3)).astype(np.float32)
        B = np.random.uniform(0, 1, size = (3, 3)).astype(np.float32)
        self.perform_test(A, B)

if __name__ == '__main__':
    tc = TestMul()
    tc.testAnotherDummy()

import numpy as np


################################################# EXPEREIMENT FUNCTION, DONT USE
def distanceCostFunction(s1, s2):
    return np.abs(s1-s2)

def DTW(signal1, signal2, costFuntion):
    #signal 1 is a temporal signal with length n
    #signal 2 is a temporal signal with length m
    #costFunction: compute distance function for two signal at time i and time j

    DEF_INFINITY = float("inf")
    
    n = signal1.shape[0]
    m = signal2.shape[0]
    d = DEF_INFINITY*np.ones((n+1, m+1))
    d[0,0] = 0
    
    for i in range(1,n+1):
        for j in range(1,m+1):
            d[i, j] = costFuntion(signal1[i-1], signal2[j-1]) + min(d[i-1, j], d[i-1, j-1] , d[i, j-1])
    #print(d, d.shape)
    return d[n, m]
####################################################

class DTWManager():


    def __init__(self, signal1, signal2, costFunction):
        if signal2.shape[1] != signal1.shape[1]:
            BaseException("Error, signal 1 and signal2 should have the same dimension")
        self.signal1 = signal1
        self.signal2 = signal2
        self.costFunction = costFunction
        self.n = signal1.shape[0]
        self.m = signal2.shape[0]
        self.dim = signal1.shape[1]
        self.d = DEF_INFINITY*np.ones((self.n+1, self.m+1))
        self.d[0,0] = 0

    def get_prev_min(self, i, j):
        return min(self.d[i-1, j], self.d[i-1, j-1] , self.d[i, j-1])

    def perform_dtw(self):
        for i in range(1,self.n+1):
            for j in range(1,self.m+1):
                self.d[i, j] = self.costFunction(self.signal1[i-1], self.signal2[j-1]) + self.get_prev_min(i, j)
        #print(self.d, self.d.shape)
        return self.d[self.n, self.m]

    def _incremental_dtw_oneside1(self, signal1):
        new_n = signal1.shape[0]
        # augment the matrix, padding DEF_INFINITY
        self.d= np.pad(self.d, ((0, new_n), (0, 0)), mode = 'constant', constant_values = DEF_INFINITY)

        for i in range(self.n+1, self.n+new_n+1):
            for j in range(1, self.m+1):
                #print(i,j)
                self.d[i, j] = self.costFunction(signal1[(i-self.n) -1], self.signal2[j - 1]) + self.get_prev_min(i, j)

        self.n = self.n+new_n        
        self.signal1 = np.vstack((self.signal1, signal1))
        return self.d[self.n, self.m]

    def _incremental_dtw_oneside2(self, signal2):
        new_m = signal2.shape[0]
        # augment the matrix, padding DEF_INFINITY
        self.d= np.pad(self.d, ((0, 0), (0, new_m)), mode = 'constant', constant_values = DEF_INFINITY)

        for i in range(1, self.n+1):
            for j in range(self.m+1, self.m+new_m+1):
                #print(i,j)
                self.d[i, j] = self.costFunction(self.signal1[i-1], signal2[(j-self.m)-1]) + self.get_prev_min(i, j)

        #print(self.d, self.d.shape)
        self.m = self.m+new_m
        self.signal2 = np.vstack((self.signal2, signal2))
        return self.d[self.n, self.m]

    def incremental_dtw(self, signal1, signal2):
        if signal1 is None and signal2 is None:
            # nothing provided, just return current value
            return self.d[self.n, self.m]
               
        if signal1 is not None:
            if signal1.shape[1] != self.dim:
                BaseException("Error, signal 1 and signal2 should have the same dimension as original signal")
            self._incremental_dtw_oneside1(signal1)

        if signal2 is not None:
            if signal2.shape[1] != self.dim:
                BaseException("Error, signal 1 and signal2 should have the same dimension as original signal")
            self._incremental_dtw_oneside2(signal2)

        return self.d[self.n, self.m]



if __name__ == "__main__":
    np.set_printoptions(precision=3)
    np.set_printoptions(suppress=True)
    m = DTWManager(np.array([1,1,2,2,2,7,2,3,4,5]).reshape(-1, 1 ), np.array([1,2,3,4,5]).reshape(-1, 1), distanceCostFunction)
    a = m.perform_dtw()
    a = m.incremental_dtw(np.array([5,5]).reshape(-1, 1), np.array([5,5]).reshape(-1, 1))
    print(a)
    a = m.incremental_dtw(np.array([5,5]).reshape(-1, 1), np.array([5,6]).reshape(-1, 1))
    print(a)
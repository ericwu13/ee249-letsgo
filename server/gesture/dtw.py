import numpy as np

def distanceCostFunction(s1, s2):
    return np.abs(s1-s2)

def DTW(signal1, signal2, costFuntion):
    #signal 1 is a temporal signal with length n
    #signal 2 is a temporal signal with length m
    #costFunction: compute distance function for two signal at time i and time j
    n = signal1.shape[0]
    m = signal2.shape[0]
    d = 100000*np.ones((n+1, m+1))
    d[0,0] = 0
    
    for i in range(1,n+1):
        for j in range(1,m+1):
            d[i, j] = costFuntion(signal1[i-1], signal2[j-1]) + min(d[i-1, j], d[i-1, j-1] , d[i, j-1])
    
    return d[n, m]

if __name__ == "__main__":
    a = DTW(np.array([1,1,2,2,2,7,2,3,4,5]), np.array([1,2,3,4,5]), distanceCostFunction)
    print(a)
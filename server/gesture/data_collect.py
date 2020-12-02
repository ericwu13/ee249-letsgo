import numpy as np
import sys
from dtw import DTW
from dtw import DTWManager

fpath1 = sys.argv[1]
fpath2 = sys.argv[2]
## check array
def raw2array(fpath1):
	array = []
	with open(fpath1, 'r') as file:
		for i, line in enumerate(file.readlines()):
			tokens = line.split()
			if len(tokens) != 13:
				break
			datapoint = list(map(float, tokens))
			array.append(datapoint)

	return np.array(array)

array1 = raw2array(fpath1)
array2 = raw2array(fpath2)

def dis(a1, a2):
	return np.mean(np.abs(a1 - a2))

print(array1.shape, array2.shape)
print(DTW(array1[:, 2], array2[:, 2], dis))

dm = DTWManager(array1, array2[:1], dis)
dm.perform_dtw()

for i in range(1, array2.shape[0]):
	print(i, dm.incremental_dtw(None, array2[i:i+1]))

## collect data

## save data

#np.save(gesture_data)
import numpy as np
import sys


fpath = sys.argv[1]
## check array
with open(fpath, 'r') as file:
	for i, line in enumerate(file.readlines()):
		a = list(map(float, line.split()))
		print(i, a)

## collect data

## save data

#np.save(gesture_data)
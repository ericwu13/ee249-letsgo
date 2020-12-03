import numpy as np
import sys
import struct

fpath1 = sys.argv[1]
fpath2 = sys.argv[2]

def raw2bin(fpath1, fpath2):

	with open(fpath1, 'r') as file_i:
		with open(fpath2, 'wb') as file_o:
			for i, line in enumerate(file_i.readlines()):
				tokens = line.split()
				if len(tokens) != 13:
					break
				datapoint = list(map(float, tokens))
				for d in datapoint:
					file_o.write(struct.pack('f', d))

raw2bin(fpath1, fpath2)
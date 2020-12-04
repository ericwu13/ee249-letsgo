import numpy as np
import sys
import struct

fpath1 = sys.argv[1]
fpath2 = sys.argv[2]

label = 'F'
threshold = 100
length_c = 13 


def raw2bin(fpath1, fpath2):

	with open(fpath1, 'r') as file_i:
		with open(fpath2, 'wb') as file_o:
			lines = file_i.readlines()
			length_r = 0
			for i, line in enumerate(lines):
				tokens = line.split()
				if len(tokens) != 13:
					break
				length_r += 1

			file_o.write(struct.pack('c', label))
			file_o.write(struct.pack('f', threshold))
			file_o.write(struct.pack('i', length_r))
			file_o.write(struct.pack('i', length_c))
			print("Label: {}, Threshold: {}, R: {}, C: {}".format(label, threshold, length_r, length_c))
			for i, line in enumerate(lines):
				tokens = line.split()
				if len(tokens) != 13:
					break
				datapoint = list(map(float, tokens))
				for d in datapoint:
					file_o.write(struct.pack('f', d))

raw2bin(fpath1, fpath2)
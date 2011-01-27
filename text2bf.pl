#!/usr/bin/python

import sys

m = 0

for x in sys.argv[1:]:
	for i in x:
		n = ord(i)
		if n > m:
			print (n-m)*"+", "."
		else:
			print (m-n)*"-", "."
		m = n

print ">", 10*"+", "."

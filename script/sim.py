#!/usr/bin/python
import csv
import random
import subprocess
import argparse
import time

# sample by weight
def weighted_sample(weights):
    rnd = random.random() * sum(weights)
    for i, w in enumerate(weights):
        rnd -= w
        if rnd < 0:
            return i

# sample uniformly
def uniform_sample(start, end):
	return random.randint(start, end)

# gaussian dist
def gauss_sample(list):
	mid = len(list)/2.0
	stdev = (len(list) - mid)/3.0
	result = int(random.gauss(mid,stdev))
	if result < 0:
		result = 0
	if result >= len(list):
		result = len(list) - 1
	return result

def run(program, host, input, output, num_samples, pattern):
	result = []
	with open(input, 'r') as f:
		reader = csv.reader(f)
		rows = list(reader)
		# remove headers
		rows = rows[1:]
		if (pattern == 'weighted'):
			weights = []
			for i in range(1, len(rows) -1):
				weights.append(float(rows[i][2]))

		# begin sampling
		for i in range(1, num_samples + 1):
			which = None
			if (pattern == 'uniform'):
				which = uniform_sample(0, len(rows) - 1)
			elif (pattern == 'weighted'):
				which = weighted_sample(weights)
			elif (pattern == 'gauss'):
				which = gauss_sample(rows)
			rank = rows[which][0]
			url = rows[which][1]
			proc = subprocess.Popen([program, host, url], stdout=subprocess.PIPE)
			time = proc.communicate()[0].split()[0]
			result.append([rank, url, time])
	with open(output, 'w') as f:
		writer = csv.writer(f)
		writer.writerow(['Rank', 'URL', 'Latency'])
		for r in result:
			writer.writerow(r)
	print output + ' generated'


def main():
	parser = argparse.ArgumentParser(description='script')
	parser.add_argument('--host', dest='host', default='localhost', help='host')
	parser.add_argument('--pattern', dest='pattern', default='uniform', help='requests pattern: uniform, weighted, gauss')
	parser.add_argument('--samples', dest='samples', default=100, help='number of samples', type=int)
	parser.add_argument('--input', dest='input', help='input file')
	parser.add_argument('--output', dest='output', help='output file')
	parser.add_argument('--program', dest='program', default='../client', help='client program path')
	args = parser.parse_args()

	print 'running'
	start = time.time()
	run(args.program, args.host, args.input, args.output, args.samples, args.pattern)
	end = time.time()
	print 'elapsed time: ', round(end - start, 3), 'seconds'

if __name__ == '__main__':
	main()

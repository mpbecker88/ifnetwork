import numpy as np

ratefile = open("rate.dat", "w")
fanofile = open("fano.dat", "w")

ratelist = []
nspikelist = []
dtlist = []

f = open('raster_filtered_gei5_gie2.dat')

for tlim in range(10000, 500000, 5000):
	t = 0

	spk = np.zeros(8000)
	print(tlim)

	while t < tlim:
		line = f.readline()
		t = float(line.strip().split()[0])
		n = int(line.strip().split()[1])

		spk[n] += 1

	fanofile.write(str(tlim - 5000) + " " + str(np.var(spk)/np.mean(spk)) + "\n")
	rate = np.array(spk)/5000.
	ratefile.write(str(tlim - 5000) + " " + str(np.mean(rate)) + "\n")		

				
ratefile.close()
fanofile.close()
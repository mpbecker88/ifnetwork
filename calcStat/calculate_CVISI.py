from pylab import frange
import numpy as np
import matplotlib.pyplot as plt

cvifile = open("CVI.dat", "w")

for g_ei in frange(1, 4, 0.25):
	for g_ie in frange(1, 4, 0.25):
		fname = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered.dat" 	

		tlist, nlist = np.loadtxt(fname, unpack=True)
		dtlist = []
		cvilist = []

		for i in xrange(4096):
			dtlist.append([])

		print g_ei, g_ie
		t0 = np.zeros(4096)
		
		for t, n in zip(tlist, nlist):
			n = int(n)

			if t0[n] == 0:
				t0[n] = t
			else:
				dtlist[n].append(t - t0[n])
				t0[n] = t

		for i in xrange(4096):
			if len(dtlist[i]) >= 3:
				cvilist.append(np.std(dtlist[i])/np.mean(dtlist[i]))

		cvilist = np.array(cvilist)

		plt.figure()
		plt.hist(cvilist, bins='auto', density=True)
		plt.savefig("ISIhist_" + str(g_ei) + "_" + str(g_ie) + ".png")
		plt.close()

		#for i in xrange(4096):
		#	cvilist[i] = np.var(dtlist)/np.mean(dtlist)

		print cvilist
		cvifile.write(str(np.mean(cvilist)) + " ")

	cvifile.write("\n")
from pylab import frange
import numpy as np
import matplotlib.pyplot as plt

for g_ei in frange(1, 4, 0.25):
	for g_ie in frange(1, 4, 0.25):
		fname = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered.dat" 	

		tlist, nlist = np.loadtxt(fname, unpack=True)
		dtlist = []
		cvilist = np.zeros(4096)

		#for i in xrange(4096):
		#	dtlist.append([])

		print g_ei, g_ie
		
		plt.figure()
		plt.xlim(48000, 50000)
		plt.ylim(0, 4096)
		plt.scatter(tlist, nlist, s=0.1, marker='o')
		plt.savefig("rasterplot_" + str(g_ei) + "_" + str(g_ie) + ".png", dpi=720)
		plt.close()

		#for i in xrange(4096):
		#	cvilist[i] = np.var(dtlist)/np.mean(dtlist)

		#print cvilist

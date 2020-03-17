from pylab import frange
import numpy as np
import matplotlib.pyplot as plt

autocorrfile = open("rate_autocorr.dat", "w")

for g_ei in frange(1, 4, 0.25):
	for g_ie in frange(1, 4, 0.25):
		print g_ei, g_ie
		fname = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered.dat" 	

		tlist, nlist = np.loadtxt(fname, unpack=True)
		histogram = np.zeros(1001)
		corr = np.zeros(100)
		histogram2 = histogram

		print 1
		for t in tlist:
			if t >= 45000:
				#print(int((t-40000)/5))
				histogram[int((t-45000)/5)] += 1
		print 2
		corr[0] = 1
		for i in xrange(99):
			histogram2 = np.delete(histogram2, i)
			corr[i+1] = np.corrcoef(histogram[:-(i+1)],histogram2)[0,1]

			plt.figure()
			plt.plot(range(0, 500, 5), corr)
			plt.savefig("autocorr_" + str(g_ei) + "_" + str(g_ie) + ".png")
			plt.close()
		print 3
		#for i in xrange(4096):
		#	cvilist[i] = np.var(dtlist)/np.mean(dtlist)

		
		autocorrfile.write(str(abs(max(corr[5:])) - min(abs(corr[5:])))  + " ")

	autocorrfile.write("\n")
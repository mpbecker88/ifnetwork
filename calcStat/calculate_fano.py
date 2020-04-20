import numpy as np
import matplotlib.pyplot as plt

fanofile = open("fano.dat", "w")
ratefile = open("rate.dat", "w")

for g_ei in range(1, 4, 0.25):
	for g_ie in range(1, 4, 0.25):
		fname = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered.dat" 	

		tlist, nlist = np.loadtxt(fname, unpack=True)

		print(g_ei, g_ie)
		nspikes = np.zeros(4096)
		nrates = np.zeros(4096)
		
		for t, n in zip(tlist, nlist):
			n = int(n)

			nspikes[n] += 1

		nrates = nspikes/5

		plt.figure()
		plt.hist(nrates, bins='auto', density=True)
		plt.savefig("ratehist_" + str(g_ei) + "_" + str(g_ie) + ".png")
		plt.close()

		#for i in range(4096):
		#	cvilist[i] = np.var(dtlist)/np.mean(dtlist)

		fano = np.var(nrates)/np.mean(nrates)

		ratefile.write(str(np.mean(nrates)) + " ")
		fanofile.write(str(fano) + " ")


	ratefile.write("\n")
	fanofile.write("\n")
import numpy as np
import matplotlib.pyplot as plt
from sklearn.neighbors import KernelDensity

cvifile = open("CVI.dat", "w")

for g_ei in np.arange(1, 4.1, 0.25):
	for g_ie in np.arange(1, 4.1, 0.25):
		fname = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered_E.dat" 	
		
		print(g_ei, g_ie)
		
		tList, nList = np.loadtxt(fname, unpack=True)
		dtlist = []
		cvilist = []
		dtLofL = []
		cvilist = []
		dtAll = []

		for i in range(4096):
			dtLofL.append([])

		t0 = np.zeros(4096)

		for t, n in zip(tList, nList):
			n = int(n)

			if t0[n] == 0:
				t0[n] = t
			else:
				dtLofL[n].append(t - t0[n])
				dtAll.append(t - t0[n])
				t0[n] = t
    
			if (t - t0[n]) > 2000:
				print(t, t0[n], n)

		for dtList in dtLofL:
			if len(dtList) >= 3:
				cvilist.append(np.std(dtList)/np.mean(dtList))

		dtAll = np.array(dtAll)
		dt_d = np.linspace(0, int(max(dtAll)), 1000)
		kde = KernelDensity(bandwidth=2.0, kernel='gaussian')
		kde.fit(dtAll[:, None])

		dtDist = np.exp(kde.score_samples(dt_d[:, None]))

		plt.fill_between(dt_d, dtDist, alpha=0.5)
		plt.plot(dtAll, np.full_like(dtAll, -0.01), '|k', markeredgewidth=1)
		plt.savefig("ISIhist_" + str(g_ei) + "_" + str(g_ie) + ".png")
		plt.close()

		#for i in range(4096):
		#	cvilist[i] = np.var(dtlist)/np.mean(dtlist)

		#print(cvilist)
		cvifile.write(str(np.mean(cvilist)) + " ")

	cvifile.write("\n")
import numpy as np
import matplotlib.pyplot as plt
from sklearn.neighbors import KernelDensity

autoCorrFile = open("rate_autocorr.dat", "w")
CGHeterogeneityFile = open("cg_heterogeneity.dat", "w")

for g_ei in np.arange(1, 4.1, 0.25):
	for g_ie in np.arange(1, 4.1, 0.25):
		print(g_ei, g_ie)
		fname = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered_E.dat"

		tList, nList = np.loadtxt(fname, unpack=True)
		corr = np.zeros(1000)
		t_d = np.linspace(40000, 50000, 5001)

		#kernel density estimator for global rate estimation
		kde = KernelDensity(bandwidth=2.0, kernel='gaussian')
		kde.fit(tList[:, None])

		spkDist = np.exp(kde.score_samples(t_d[:, None]))
		popRate = spkDist*len(tList)/4096 
		popRate2 = popRate

		#coarse grained kernel density estimator for spike heterogeneity estimation
		kdeCG = KernelDensity(bandwidth=200.0, kernel='gaussian')
		kdeCG.fit(tList[:, None])

		spkDistCG = np.exp(kdeCG.score_samples(t_d[:, None]))

		#calculate autocorrelation
		corr[0] = 1
		for i in range(999):
			popRate2 = np.delete(popRate2, 0)
			corr[i+1] = np.corrcoef(popRate[:-(i+1)], popRate2)[0,1]

		plt.figure()
		plt.plot(range(0, 1000, 1), corr)
		plt.savefig("autocorr_" + str(g_ei) + "_" + str(g_ie) + ".png")
		plt.close()

		plt.figure()
		plt.fill_between(t_d, popRate, alpha=0.5)
		plt.plot(tList, np.full_like(tList, -0.0001), '|k', markeredgewidth=1)
		plt.savefig("popRate_" + str(g_ei) + "_" + str(g_ie) + ".png")
		plt.close()

		plt.figure()
		plt.fill_between(t_d, spkDistCG, alpha=0.5)
		plt.plot(tList, np.full_like(tList, -0.0001), '|k', markeredgewidth=1)
		plt.savefig("cg_skdist_" + str(g_ei) + "_" + str(g_ie) + ".png")
		plt.close()

		autoCorrFile.write(str(abs(max(corr[10:])) - min(abs(corr[10:])))  + " ")
		CGHeterogeneityFile.write(str(4*np.std(spkDistCG[2500:-100])/max(spkDistCG)) + " ")

	autoCorrFile.write("\n")
	CGHeterogeneityFile.write("\n")

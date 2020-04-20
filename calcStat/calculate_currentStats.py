import numpy as np

meanfile = open("currentMean.dat", "w")
stdfile = open("currentSTD.dat", "w")
corrfile = open("currentCorr.dat", "w")

for g_ei in range(1, 4, 0.25):
	for g_ie in range(1, 4, 0.25):
		fname = "currents" + '%.6f' % g_ei + "_" '%.6f' % g_ie + ".dat"

		CurrMatrix = np.loadtxt(fname, usecols = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20))

		print(g_ei, g_ie)

		cmean = np.zeros(10)
		cstd = np.zeros(10)
		ccorr = np.zeros(10)

		for i in range(0, 10, 2):
			coltemp1 = CurrMatrix.transpose()[i][-100000:]
			coltemp2 = CurrMatrix.transpose()[i+1][-100000:]

			cmean[i] = np.mean(coltemp1 + coltemp2)
			cstd[i] = np.std(coltemp1 + coltemp2)
			ccorr[i] = np.corrcoef(coltemp1, coltemp2)[1][0]



		meanfile.write(str(np.mean(cmean)) + " ")
		stdfile.write(str(np.mean(cstd)) + " ")
		corrfile.write(str(np.mean(ccorr)) + " ")

	meanfile.write("\n")
	stdfile.write("\n")
	corrfile.write("\n")

import numpy as np

meanEfile = open("currentMeanE.dat", "w")
stdCurrNeuronEfile = open("stdCurrNeuronE.dat", "w")
stdEfile = open("currentSTDE.dat", "w")
corrEfile = open("currentCorrE.dat", "w")
meanIfile = open("currentMeanI.dat", "w")
stdCurrNeuronIfile = open("stdCurrNeuronI.dat", "w")
stdIfile = open("currentSTDI.dat", "w")
corrIfile = open("currentCorrI.dat", "w")

for g_ei in np.arange(1, 4.1, 0.25):
	for g_ie in np.arange(1, 4.1, 0.25):
		fname = "currents" + '%.6f' % g_ei + "_" '%.6f' % g_ie + ".dat"

		CurrMatrixE = np.loadtxt(fname, usecols = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20))
		CurrMatrixI = np.loadtxt(fname, usecols = (21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32))
		
		print(g_ei, g_ie)

		cmeanE = np.zeros(10)
		cstdE = np.zeros(10)
		ccorrE = np.zeros(10)

		cmeanI = np.zeros(6)
		cstdI = np.zeros(6)
		ccorrI = np.zeros(6)

		for i in range(0, 10, 2):
			coltemp1 = CurrMatrixE.transpose()[i][-100000:]
			coltemp2 = CurrMatrixE.transpose()[i+1][-100000:]

			cmeanE[i] = np.mean(coltemp1 + coltemp2)
			cstdE[i] = np.std(coltemp1 + coltemp2)
			ccorrE[i] = np.corrcoef(coltemp1, coltemp2)[1][0]

		for i in range(0, 6, 2):
			coltemp1 = CurrMatrixI.transpose()[i][-100000:]
			coltemp2 = CurrMatrixI.transpose()[i+1][-100000:]

			cmeanI[i] = np.mean(coltemp1 + coltemp2)
			cstdI[i] = np.std(coltemp1 + coltemp2)
			ccorrI[i] = np.corrcoef(coltemp1, coltemp2)[1][0]

		meanEfile.write(str(np.mean(cmeanE)) + " ")
		stdCurrNeuronEfile.write(str(np.std(cmeanE)) + " ")
		stdEfile.write(str(np.mean(cstdE)) + " ")
		corrEfile.write(str(np.mean(ccorrE)) + " ")
		meanIfile.write(str(np.mean(cmeanI)) + " ")
		stdCurrNeuronIfile.write(str(np.std(cmeanI)) + " ")
		stdIfile.write(str(np.mean(cstdI)) + " ")
		corrIfile.write(str(np.mean(ccorrI)) + " ")

	meanEfile.write("\n")
	stdCurrNeuronEfile.write("\n")
	stdEfile.write("\n")
	corrEfile.write("\n")
	meanIfile.write("\n")
	stdCurrNeuronIfile.write("\n")
	stdIfile.write("\n")
	corrIfile.write("\n")

import numpy as np

alphaIString = ["05", "10"]
alphaString = ["02", "04", "06", "08", "10", "r"]
for aIStr in alphaIString:
	currStateEFile = open("currStateE" + aIStr + ".dat", "w")
	for aStr in alphaString:
		
		meanCurrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/currentMeanE.dat")
		stdCurrNeuronM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/stdCurrNeuronE.dat")
		stdCurrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/currentSTDE.dat")
		corrCurrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/currentCorrE.dat")
		autocorrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/rate_autocorr.dat")
		CVIM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/CVI.dat")

		nlin = CVIM.shape[0]
		ncol = CVIM.shape[1]

		for l in range(nlin):
			for c in range(ncol):
				currStateEFile.write(str(meanCurrM[l][c]) + " " + str(stdCurrM[l][c]) + " " + str(corrCurrM[l][c]) + " " + str(stdCurrM[l][c]) + " " + str(autocorrM[l][c]) + " " + str(CVIM[l][c]) + "\n")

	currStateEFile.close()

for aIStr in alphaIString:
	currStateIFile = open("currStateI" + aIStr + ".dat", "w")
	for aStr in alphaString:
	
		meanCurrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/currentMeanI.dat")
		stdCurrNeuronM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/stdCurrNeuronI.dat")
		stdCurrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/currentSTDI.dat")
		corrCurrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/currentCorrI.dat")
		autocorrM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/rate_autocorr.dat")
		CVIM = np.loadtxt("InAlpha" + str(aIStr) + "/alpha" + str(aStr) + "/CVI.dat")

		nlin = CVIM.shape[0]
		ncol = CVIM.shape[1]

		for l in range(nlin):
			for c in range(ncol):
				currStateIFile.write(str(meanCurrM[l][c]) + " " + str(stdCurrM[l][c]) + " " + str(corrCurrM[l][c]) + " " + str(stdCurrM[l][c]) + " " + str(autocorrM[l][c]) + " " + str(CVIM[l][c]) + "\n")

	currStateIFile.close()
from pylab import frange
import numpy as np


CVI_file = open("CVI_alpha.dat", "w")
fano_file = open("fano_alpha.dat", "w")
autocorr_file = open("autocorr_alpha.dat", "w")

alphaString = ["02", "04", "06", "08", "10"]
alphaFloat = [0.2, 0.4, 0.6, 0.8, 1.0]

for aStr, aFl in zip(alphaString, alphaFloat):
	CVI = np.loadtxt("alpha" + aStr + "/CVI.dat")
	fano = np.loadtxt("alpha" + aStr + "/fano.dat")
	autocorr = np.loadtxt("alpha" + aStr + "/rate_autocorr.dat")

	meanCVI = np.mean(CVI)
	meanfano = np.mean(fano)
	meanautocorr = np.mean(autocorr)

	CVI_file.write(str(aFl) + " " + str(meanCVI) + "\n")
	fano_file.write(str(aFl) + " " + str(meanfano) + "\n")
	autocorr_file.write(str(aFl) + " " + str(meanautocorr) + "\n")


CVI_file.close()
fano_file.close()
autocorr_file.close()

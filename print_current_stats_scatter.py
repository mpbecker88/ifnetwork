from pylab import frange
import numpy as np

CVI_currfile = open("CVI-curr.dat", "w")
CVI_corrfile = open("CVI-corr.dat", "w")
CVI_STDfile = open("CVI-STD.dat", "w")
fano_currfile = open("fano-curr.dat", "w")
fano_corrfile = open("fano-corr.dat", "w")
fano_STDfile = open("fano-STD.dat", "w")
autocorr_currfile = open("autocorr-curr.dat", "w")
autocorr_corrfile = open("autocorr-corr.dat", "w")
autocorr_STDfile = open("autocorr-STD.dat", "w")

CVI = np.loadtxt("CVI.dat")
fano = np.loadtxt("fano.dat")
autocorr = np.loadtxt("rate_autocorr.dat")
currentMean = np.loadtxt("currentMean.dat")
currentCorr = np.loadtxt("currentCorr.dat")
currentSTD = np.loadtxt("currentSTD.dat")

for irow, jrow in zip(currentMean, CVI):
	for i, j in zip(irow, jrow):
		CVI_currfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentCorr, CVI):
	for i, j in zip(irow, jrow):
		CVI_corrfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentSTD, CVI):
	for i, j in zip(irow, jrow):
		CVI_STDfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentMean, fano):
	for i, j in zip(irow, jrow):
		fano_currfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentCorr, fano):
	for i, j in zip(irow, jrow):
		fano_corrfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentSTD, fano):
	for i, j in zip(irow, jrow):
		fano_STDfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentMean, autocorr):
	for i, j in zip(irow, jrow):
		autocorr_currfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentCorr, autocorr):
	for i, j in zip(irow, jrow):
		autocorr_corrfile.write(str(i) + " " + str(j) + "\n")

for irow, jrow in zip(currentSTD, autocorr):
	for i, j in zip(irow, jrow):
		autocorr_STDfile.write(str(i) + " " + str(j) + "\n")

CVI_currfile.close()
CVI_corrfile.close()
CVI_STDfile.close()
fano_currfile.close()
fano_corrfile.close()
fano_STDfile.close()
autocorr_currfile.close()
autocorr_corrfile.close()
autocorr_STDfile.close()

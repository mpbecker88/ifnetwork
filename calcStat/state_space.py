import numpy as np
import matplotlib.pyplot as plt

rho_c = 0.0
cvi_c = 0.0
fano_c = 0.0
rate_c = 0.01
maxRate = 0

alphaIString = ["05", "10"]
alphaString = ["02", "04", "06", "08", "10", "r"]

maxDelta = 0
rho_delta = 0

for aIStr in alphaIString:
    for aStr in alphaString:
        aCorrM = np.loadtxt("autocorr" + str(aIStr) + "-" + str(aStr) + ".dat")

        nlin = aCorrM.shape[0]
        ncol = aCorrM.shape[1]

        for l in range(nlin-1):
            for c in range(ncol-1):
                delta = aCorrM[l][c+1] - aCorrM[l][c]
                if abs(delta) > abs(maxDelta):
                    maxDelta = delta
                    rho_delta = aCorrM[l][c]

        for c in range(ncol-1):
            for l in range(nlin-1):
                delta = aCorrM[l+1][c] - aCorrM[l][c]
                if abs(delta) > abs(maxDelta):
                    maxDelta = delta
                    rho_delta = aCorrM[l][c]

rho_c = rho_delta + maxDelta/2

maxDelta = 0
cvi_delta = 0

for aIStr in alphaIString:
    for aStr in alphaString:
        CVIM = np.loadtxt("CVI" + str(aIStr) + "-" + str(aStr) + ".dat")

        nlin = CVIM.shape[0]
        ncol = CVIM.shape[1]

        for l in range(nlin-1):
            for c in range(ncol-1):
                delta = CVIM[l][c+1] - CVIM[l][c]
                if (abs(delta) > abs(maxDelta)) and (CVIM[l][c+1] < 1) and (CVIM[l][c] < 1):
                    maxDelta = delta
                    cvi_delta = CVIM[l][c]

        for c in range(ncol-1):
            for l in range(nlin-1):
                delta = CVIM[l+1][c] - CVIM[l][c]
                if abs(delta) > abs(maxDelta) and (CVIM[l+1][c] < 1) and (CVIM[l][c] < 1):
                    maxDelta = delta
                    cvi_delta = CVIM[l][c]

cvi_c = cvi_delta + maxDelta/2

maxDelta = 0
fano_delta = 0

for aIStr in alphaIString:
    for aStr in alphaString:
        fanoM = np.loadtxt("fano" + str(aIStr) + "-" + str(aStr) + ".dat")

        nlin = fanoM.shape[0]
        ncol = fanoM.shape[1]

        for l in range(nlin-1):
            for c in range(ncol-1):
                delta = fanoM[l][c+1] - fanoM[l][c]
                if abs(delta) > abs(maxDelta):
                    maxDelta = delta
                    fano_delta = fanoM[l][c]

        for c in range(ncol-1):
            for l in range(nlin-1):
                delta = fanoM[l+1][c] - fanoM[l][c]
                if abs(delta) > abs(maxDelta):
                    maxDelta = delta
                    fano_delta = fanoM[l][c]

fano_c = fano_delta + maxDelta/2

maxDelta = 0
rate_delta = 0

for aIStr in alphaIString:
    for aStr in alphaString:
        rateM = np.loadtxt("rate" + str(aIStr) + "-" + str(aStr) + ".dat")

        nlin = rateM.shape[0]
        ncol = rateM.shape[1]

        for l in range(nlin-1):
            for c in range(ncol-1):
                delta = rateM[l][c+1] - rateM[l][c]
                if abs(delta) > abs(maxDelta):
                    maxDelta = delta
                    rate_delta = rateM[l][c]
                if rateM[l][c] > maxRate:
                    maxRate = rateM[l][c]

        for c in range(ncol-1):
            for l in range(nlin-1):
                delta = rateM[l+1][c] - rateM[l][c]
                if abs(delta) > abs(maxDelta):
                    maxDelta = delta
                    rate_delta = rateM[l][c]

rate_c = rate_delta + maxDelta/2

for aIStr in alphaIString:
    for aStr in alphaString:
        phiFile = open("phi" + str(aIStr) + "-" + str(aStr) + ".dat", "w")

        aCorrM = np.loadtxt("autocorr" + str(aIStr) + "-" + str(aStr) + ".dat")
        cviM = np.loadtxt("CVI" + str(aIStr) + "-" + str(aStr) + ".dat")
        fanoM = np.loadtxt("fano" + str(aIStr) + "-" + str(aStr) + ".dat")
        rateM = np.loadtxt("rate" + str(aIStr) + "-" + str(aStr) + ".dat")

        nlin = aCorrM.shape[0]
        ncol = aCorrM.shape[1]
        
        phiM = np.zeros((nlin, ncol))

        for l in range(nlin):
            for c in range(ncol):
                if cviM[l][c] > 1: #C
                    phiFile.write("5 ")
                elif rateM[l][c] < 1: #Q
                    phiFile.write("0 ")
                elif (aCorrM[l][c] < rho_c) and (cviM[l][c] > cvi_c): #AI
                    phiFile.write("1 ")
                elif (aCorrM[l][c] < rho_c) and (cviM[l][c] < cvi_c): #AR
                    phiFile.write("2 ")
                elif (aCorrM[l][c] > rho_c) and (cviM[l][c] > cvi_c): #SI
                    phiFile.write("3 ")
                elif (aCorrM[l][c] > rho_c) and (cviM[l][c] < cvi_c): #SR
                    phiFile.write("4 ")
            phiFile.write("\n")

        phiFile.close()

        


import numpy as np

geilist = [1, 2, 3, 4, 5, 6]
gielist = [1, 2, 3, 4, 5, 6]

cvifile = open("CVI.dat", "w")
ratefile = open("rate.dat", "w")
fanofile = open("fano.dat", "w")

for gei in geilist:
	for gie in gielist:
		print gei, gie
		tlist, nlist = np.loadtxt("raster_filtered_gei" + str(gei) + "_gie" + str(gie) + ".dat", unpack=True)

		CVISIlist = []
		ratelist = []
		nspikelist = []
		dtlist = []


		for i in xrange(1000):
			print i,
			dtlist = []
			spikecount = 0
			tnlist = []

			for t, n in zip(tlist, nlist):
				if n == i:
					tnlist.append(t)
					t0 = t
					spikecount += 1			

			print spikecount
			
			if len(tnlist) > 5:
				for j in xrange(1,len(tnlist)):
					dtlist.append(tnlist[j] - tnlist[j-1])

			ratelist.append(spikecount/tlist[-1])
			nspikelist.append(spikecount)

		cvifile.write(str(np.var(dtlist)/np.mean(dtlist)) + " ")
		ratefile.write(str(np.mean(ratelist)) + " ")
		fanofile.write(str(np.var(nspikelist)/np.mean(nspikelist)) + " ")
		
	cvifile.write("\n")
	ratefile.write("\n")
	fanofile.write("\n")
				
cvifile.close()
ratefile.close()
fanofile.close()
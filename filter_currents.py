import os
from pylab import frange


for g_ei in frange(1, 4, 0.25):
	for g_ie in frange(1, 4, 0.25):
		awkcmd = "'{if ($1 >= 40000) {print $1, ($2 + $3 + $4 + $5 + $6 + $7 + $8 + $9 + $10 + $11)/5}}'"
		inputf = "currents" + '%.6f' % g_ei + "_" '%.6f' % g_ie + ".dat"
		outputf = "currents" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered.dat" 

		str =  "awk " +  awkcmd + " " + inputf + " > " + outputf
		os.system(str)
		#call(["awk", awkcmd, inputf, ">", outputf ])
import os
from pylab import frange


for g_ei in frange(1, 4, 0.25):
	for g_ie in frange(1, 4, 0.25):
		awkcmd = "'{if ($1 >= 45000 && $2 < 4096) {print $1,$2}}'"
		inputf = "raster" + '%.6f' % g_ei + "_" '%.6f' % g_ie + ".dat"
		outputf = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered.dat" 

		str =  "awk " +  awkcmd + " " + inputf + " > " + outputf
		os.system(str)
		#call(["awk", awkcmd, inputf, ">", outputf ])
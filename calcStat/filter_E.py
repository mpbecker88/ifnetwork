import numpy as np

for g_ei in np.arange(1, 4.1, 0.25):
	for g_ie in np.arange(1, 4.1, 0.25):
		input_name = "raster" + '%.6f' % g_ei + "_" '%.6f' % g_ie + ".dat"
		output_name = "raster" + '%.2f' % g_ei + "_" '%.2f' % g_ie + "filtered_E.dat" 

		inputf = open(input_name, 'r')
		outputf = open(output_name, 'w')
		
		print(g_ei, g_ie)
		
		line = inputf.readline()
		while(line):
			line_split = line.split()
			t = float(line_split[0])
			n = int(line_split[1])

			if (t >= 40000) and (n < 4096):
				outputf.write(str(t) + " " + str(n) + "\n")

			line = inputf.readline()
		
		inputf.close()
		outputf.close()
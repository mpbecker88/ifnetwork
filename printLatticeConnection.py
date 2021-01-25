import numpy as np
import matplotlib.pyplot as plt

neurons = np.loadtxt("connections.dat")

nc = 500
xc = nc%64
yc = nc//64

#xc = nc%64
#yc = nc//64

xe = []
ye = []

xi = []
yi = []

for n in neurons[nc,:]:
	if n < 4096:
		xe.append(n%64)
		ye.append(n//64)
	else:
		ni = n - 4096
		xi.append(0.5 + 2*(ni%32))
		yi.append(0.5 + 2*(ni//32))

plt.plot(xe, ye, 'b.')
plt.plot(xi, yi, 'r.')
plt.plot(xc, yc, 'go')
plt.show()
import numpy as np
import math

import matplotlib.pyplot as plt

chopoff = 200
d_nom = 5.74
d_scale = 1.0185
encoder_data = np.loadtxt('Tracks/RUN0018.LOG',delimiter=',')
x1  = encoder_data[chopoff:,0]/(600.0*4)*(d_nom/12.0*math.pi)
x2 = encoder_data[chopoff:,1]/(600.0*4)*(d_nom*d_scale/12.0*math.pi)
D = 23/12.0

print(x1[-1],x2[-1])

k = np.zeros_like(x1)
d = np.zeros_like(x1)
theta = np.zeros_like(x1)
x = np.zeros_like(x1)
y = np.zeros_like(x1)

for i in range(1,np.size(x1)):
	d[i] = (x1[i]-x1[i-1]+x2[i]-x2[i-1])/2
	k[i] = 2/D *(x1[i]-x1[i-1]-x2[i]+x2[i-1])/(x1[i]-x1[i-1]+x2[i]-x2[i-1])
	theta[i] = theta[i-1] + d[i]*k[i]
	x[i] = x[i-1] + math.sin(theta[i])*d[i]
	y[i] = y[i-1] + math.cos(theta[i])*d[i]

print(x[-1],y[-1])

plt.plot(x,y,'-g')
plt.axis('equal')

plt.show()

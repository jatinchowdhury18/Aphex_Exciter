#%%
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt


#%%
a = 15 # R_1p
b = 2400
c = 1100
d = 4.7e-9
e = 56e-12
f = 4.7e-6

#%%
b_4 = a**2 * d**2 * b * c * e * f
b_3 = a**2 * d**2 * (b*e + b*f + c*f)
b_2 = a**2 * d**2
b_1 = 0
b_0 = 0

a_3 = a*b*c*d*e*f
a_2 = (b*c*e*f) + (a*c*d*f) - (a*b*d*f) + (a*b*d*e)
a_1 = a*d + b*e + c*f
a_0 = 1

#%%
w, H = signal.freqs ([b_4, b_3, b_2, b_1, b_0], [a_3, a_2, a_1, a_0])

plt.figure()
plt.semilogx (w / 2 / np.pi, 20*np.log10 (np.abs (H)))
plt.xlim (20, 30000)
plt.ylim (-100, 50)

#%%
def myBilinear (b, a, fs):
    z = np.zeros (5) # numerator
    p = np.zeros (5) # denominator
    c = 2 * fs # no warping (for now)

    b_0 = b[0]*c**4
    b_1 = b[1]*c**3
    b_2 = b[2]*c**2
    b_3 = b[3]*c
    b_4 = b[4]

    z[0] = b_0 + b_1 + b_2 + b_3 + b_4
    z[1] = -4*b_0 - 2*b_1 + 2*b_3 + 4*b_4
    z[2] = 6*b_0 - 2*b_2 + 6*b_4
    z[3] = -4*b_0 + 2*b_1 - 2*b_3 + 4*b_4
    z[4] = b_0 - b_1 + b_2 - b_3 + b_4

    a_0 = a[0]*c**4
    a_1 = a[1]*c**3
    a_2 = a[2]*c**2
    a_3 = a[3]*c
    a_4 = a[4]

    print ([a_0, a_1, a_2, a_3, a_4])

    p[0] = a_0 + a_1 + a_2 + a_3 + a_4
    p[1] = -4*a_0 - 2*a_1 + 2*a_3 + 4*a_4
    p[2] = 6*a_0 - 2*a_2 + 6*a_4
    p[3] = -4*a_0 + 2*a_1 - 2*a_3 + 4*a_4
    p[4] = a_0 - a_1 + a_2 - a_3 + a_4

    print (p)

    z /= p[0]
    p /= p[0]

    return z, p

#%%
b_bilin, a_bilin = signal.bilinear ([b_4, b_3, b_2, b_1, b_0], [0, a_3, a_2, a_1, a_0], 192000)
myB, myA = myBilinear ([b_4, b_3, b_2, b_1, b_0], [0, a_3, a_2, a_1, a_0], 192000)

print (b_bilin)
print (myB)

print (a_bilin)
print (myA)

#%%
z, p, k = signal.tf2zpk (myB, myA)

print (np.abs (p))

#%%

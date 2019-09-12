#%%
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

#%%
N = 1000
n = np.arange (N)
f = 100
fs = 44100
x = (1.58 * 0.3125) * np.sin (2 * np.pi * n * f / fs)

#%%
e_s_plus = 72
e_s_minus = -72

V_cm = (e_s_plus + e_s_minus) / 2
V_dm = (e_s_plus - e_s_minus) / 2
R_p = 0 # 50000
G = (R_p + 150 + 20000) / (R_p + 150)

print (V_cm)
print (V_dm)

#%%
y = np.zeros (N)
y_1 = 0
for i in range (N):
    inner = (G*x[i] - V_cm) / V_dm
    sat = inner
    if sat < -1:
        sat = -1
    elif sat >1:
        sat = 1
    y[i] = V_cm + V_dm * sat


#%%
plt.figure()
plt.plot (n, x)
plt.plot (n, y)
plt.axhline (66)

#%%

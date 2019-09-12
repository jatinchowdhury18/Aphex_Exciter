#%%
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

#%%
N = 100
n = np.arange (N)
f = 1000
fs = 44100
x = 18 * np.sin (2 * np.pi * n * f / fs)
for i in range (N):
    if x[i] > 0:
        x[i] = 18
    elif x[i] < 0:
        x[i] = -18

#%%
R_1 = 10000
R_2 = 2400
R_3 = 1000
C_1 = 4.7e-9
C_2 = 56.0e-12
C_3 = 4.7e-6
R_1p = R_1 + 0 # 50000

#%%
w = 1 / (R_1p * C_1)
G = (R_2 + R_3) / R_2
Q = 1 / (3 - G)

#%%
def calcCoefsLPF (fc, Q, fs):
    wc = 2 * np.pi * fc
    c = 1 / np.tan (wc / (fs * 2))
    c2 = c * c
    K = c / Q
    a0 = c2 + K + 1

    b = [1 / a0, 2 / a0, 1 / a0]
    a = [1, 2*(1-c2) / a0, (c2 - K + 1) / a0]

    return np.asarray (b), np.asarray(a)

#%%
b_lin, a_lin = calcCoefsLPF (w, Q, fs)
y_lin = signal.lfilter (b_lin, a_lin, x)

#%%
A = np.array ([[-2, 1 - 2*G],
               [1, -1+G]])
B = np.array ([[1], [0]])
C = np.array ([0, 1])
F = np.array ([[-2], [1]])

w_d = 2 * np.pi * w / fs

D = np.identity (2) - (w_d / 2) * A
A_d = np.matmul (np.linalg.inv (D), np.identity (2) + (w_d / 2) * A)
B_d = np.matmul (np.linalg.inv (D), w_d *  B)
F_d = np.matmul (np.linalg.inv (D), w_d *  F)
print (F_d)

#%%
def N_fun (v, G, es_plus, es_minus):
    first = G * v - es_minus
    if first > 0:
        first = 0
    
    second = G * v - es_plus
    if second < 0:
        second = 0

    return (first**2 / (2*G)) + (second**2 / (2*G))

def Nabla_N (v, G, es_plus, es_minus):
    first = G * v - es_minus
    if first > 0:
        first = 0
    
    second = G * v - es_plus
    if second < 0:
        second = 0

    return first + second

def Nabla_Bar_N (v_1, v_0, G, es_plus, es_minus):
    if np.abs (v_0 - v_1) > 1e-9:
        return (N_fun (v_1, G, es_plus, es_minus) - N_fun (v_0, G, es_plus, es_minus)) / (v_1 - v_0)
    else:
        return Nabla_N (v_0, G, es_plus, es_minus)

#%%
x_0 = np.array ([[0], [0]])
x_1 = np.array ([[0], [0]])

u_0 = 0
u_1 = 0

y_nonlin = np.zeros (N)
for i in range (N):
    u_0 = u_1
    u_1 = x[i]
    u_bar = (u_1 + u_0) / 2

    v_1 = np.matmul (C, x_1)
    v_0 = np.matmul (C, x_0)
    phi = np.matmul (A_d, x_0) + B_d * u_bar - F_d * Nabla_Bar_N (v_1, v_0, G, 30, -30)
    x_0 = x_1
    x_1 = phi

    v = np.matmul (C, x_1)
    y_nonlin[i] = G*v - Nabla_N (v, G, 30, -30)

#%%
plt.figure()
plt.plot (x)
plt.plot (y_lin)
plt.plot (y_nonlin)

#%%

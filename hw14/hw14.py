from __future__ import print_function, division
import csv
import matplotlib.pyplot as plt
import numpy as np

def sr(t):
    return (len(t) - 1) / (t[-1] - t[0])

def fft_plot(ax1, ax2, t, y1, y2, title, xlabel, ylabel, fft_title, fft_xlabel, fft_ylabel):
    n1, n2 = len(y1), len(y2)
    k1, k2 = np.arange(n1), np.arange(n2)
    Fs1, Fs2 = int(len(y1) / t[-1]), int(len(y2) / t[-1])
    T1, T2 = n1 / Fs1, n2 / Fs2
    frq1, frq2 = k1 / T1, k2 / T2
    frq1, frq2 = frq1[range(int(n1 / 2))], frq2[range(int(n2 / 2))]
    Y1, Y2 = np.fft.fft(y1) / n1, np.fft.fft(y2) / n2
    Y1, Y2 = Y1[range(int(n1 / 2))], Y2[range(int(n2 / 2))]

    ax1.plot(t, y2, 'k', label='Original Data')
    ax1.plot(t[:len(y1)], y1, 'r', label='Filtered Data')
    ax1.set(xlabel=xlabel, ylabel=ylabel, title=title)
    ax1.legend()

    ax2.loglog(frq2, abs(Y2), 'k', label='FFT of Original Data')
    ax2.loglog(frq1, abs(Y1), 'r', label='FFT of Filtered Data')
    ax2.set(xlabel=fft_xlabel, ylabel=fft_ylabel, title=fft_title)
    ax2.legend()

def FIR(ax1, ax2, t, data, cutoff, length, index):
    fS = int(len(data) / t[-1])
    fL, N = cutoff[index], length[index]
    h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))
    h *= np.blackman(N)
    h /= np.sum(h)
    avg = np.convolve(data, h, mode='valid')
    fft_plot(
        ax1, ax2,
        t[:len(avg)], avg, data[:len(avg)],
        f'Filtered Data with FIR fL = {fL}, N = {N}',
        'Time', 'Amplitude',
        f'FFT of Filtered Data with FIR fL = {fL}, N = {N}',
        'Freq (Hz)', '|Y(freq)|'
    )

file_list = ['sigA.csv', 'sigB.csv', 'sigC.csv', 'sigD.csv']

fig, axs = plt.subplots(4, 2, figsize=(15, 20))

for i, file_name in enumerate(file_list):
    t, data1 = [], []
    with open(file_name) as f:
        reader = csv.reader(f)
        for row in reader:
            t.append(float(row[0]))
            data1.append(float(row[1]))

    FIR(axs[i, 0], axs[i, 1], t, data1, cutoff=[100, 50, 5000, 15], length=[40, 60, 1, 250], index=i)

plt.suptitle('Data and FFT Plots', fontsize=16, y=1.02)
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.subplots_adjust(hspace=0.5, wspace=0.3)
plt.show()
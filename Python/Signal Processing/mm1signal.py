import scipy.signal as scp
import scipy.io
import matplotlib.pyplot as plt
import numpy as np

# Load the dataset into variable mat from .mat data file
mat = scipy.io.loadmat('C:/Users/Petaa/Desktop/Skole/Programming/Python/Python-Programming/Python/Signal Processing/ME.mat')
#print(mat.items())

# Variable czData contains the data recorded from electrode at cz in the brain (center)
czData = np.array(mat['data'][4])

fs = 500


# Plot the data vector per Exercise 1.1
fig, axs = plt.subplots(1)
axs.set_title("Signal")
axs.plot(czData, color='C0')
axs.set_xlabel("Time")
axs.set_ylabel("Amplitude")


# IIR filter from Exercise 1.2
#filteredczDATA=scp.iirfilter(4,czData,btype=)
# Plot the data vector per Exercise 1.2
fig, ax = plt.subplots(1)
ax.set_title("Signal")
#ax.plot(filteredczDATA, color='C0')
ax.set_xlabel("Time")
ax.set_ylabel("Amplitude")

plt.show()
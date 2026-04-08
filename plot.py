import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("results.txt")
plt.plot(np.cumsum(data[:, 0]), label="Static")
plt.plot(np.cumsum(data[:, 1]), label="Learning")
plt.legend()
plt.show()
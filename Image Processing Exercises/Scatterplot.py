import numpy as np
from sklearn.neighbors import NearestNeighbors
import matplotlib.pyplot as plt

class Instance:
    def __init__(self, classData):
        self.data = np.loadtxt(classData)
        self.feature1Data = [arr[0] for arr in self.data]
        self.feature2Data = [arr[1] for arr in self.data]
        self.feature3Data = [arr[2] for arr in self.data]
        self.feature4Data = [arr[3] for arr in self.data]

class1 = Instance("trainClass1.dat")
class2 = Instance("trainClass2.dat")
class3 = Instance("trainClass3.dat")
class4 = Instance("trainClass4.dat")

fig, ax = plt.subplots(2)

ax[0].scatter(class1.feature1Data,class1.feature2Data)
ax[1].scatter(class1.feature1Data,class1.feature4Data)

ax[0].scatter(class2.feature1Data,class2.feature2Data)
ax[1].scatter(class2.feature3Data,class2.feature4Data)

ax[0].scatter(class3.feature1Data,class3.feature2Data)
ax[1].scatter(class3.feature3Data,class3.feature4Data)

ax[0].scatter(class4.feature1Data,class4.feature2Data)
ax[1].scatter(class4.feature3Data,class4.feature4Data)

nbrs = NearestNeighbors(n_neighbors=2, algorithm='ball_tree').fit([class1.feature1Data,class1.feature2Data])
print (nbrs)
plt.show()
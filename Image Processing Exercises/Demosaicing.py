import cv2 as cv
import numpy as np

Bayer = np.array([[10, 20, 12, 23],
                 [17, 100, 25, 95],
                 [9, 17, 8, 22],
                 [16, 89, 19, 92]])

demosaic = np.zeros((3,3,3))

for x in (range(len(demosaic))):
    for y in (range(len(demosaic))):
        if (x%2==0):
            if (y%2==0):
                demosaic[x,y,0] = Bayer[x+1,y+1] #R
                demosaic[x,y,1] = Bayer[x,y+1]   #G
                demosaic[x,y,2] = Bayer[x,y]     #B
            else:
                demosaic[x,y,0] = Bayer[x+1,y]
                demosaic[x,y,1] = Bayer[x,y]
                demosaic[x,y,2] = Bayer[x,y-1]
        else:
            if (y%2==0):
                demosaic[x,y,0] = Bayer[x,y+1]
                demosaic[x,y,1] = Bayer[x,y]
                demosaic[x,y,2] = Bayer[x-1,y]                
            else:
                demosaic[x,y,0] = Bayer[x,y]
                demosaic[x,y,1] = Bayer[x+1,y]
                demosaic[x,y,2] = Bayer[x-1,y-1]

print("RED:")
print(demosaic[:,:,0])
print("GREEN:")
print(demosaic[:,:,1])
print("BLUE:")
print(demosaic[:,:,2])
import cv2 as cv
import numpy as np

##Bayer image 4x4
#Bayer = np.array([[100, 10, 110, 11],
#                 [9, 50, 8, 49],
#                 [105, 12, 112, 9],
#                 [14, 52, 15, 54]])


Bayer = np.array([[10, 20, 12, 23],
                 [17, 100, 25, 95],
                 [9, 17, 8, 22],
                 [16, 89, 19, 92]])

#Three by three array of placeholder zeroes
demosaic = np.zeros((3,3,3))

for x in (range(len(demosaic))):
    for y in (range(len(demosaic))):
        if (x%2==0):
            if (y%2==0):
                #If the square is guaranteed to be blue
                demosaic[x,y,0] = Bayer[x+1,y+1] #R
                demosaic[x,y,1] = Bayer[x+1,y]   #G
                demosaic[x,y,2] = Bayer[x,y]     #B
            else:
                #Else it MUST be GR
                demosaic[x,y,0] = Bayer[x+1,y]
                demosaic[x,y,1] = Bayer[x,y]
                demosaic[x,y,2] = Bayer[x,y-1]
        elif (x%2==1):
            if (y%2==0):
                # This square will always correspond to GB
                demosaic[x,y,0] = Bayer[x,y+1]
                demosaic[x,y,1] = Bayer[x,y]
                demosaic[x,y,2] = Bayer[x-1,y]                
            # Else it must be red
            else:
                demosaic[x,y,0] = Bayer[x,y]
                demosaic[x,y,1] = Bayer[x-1,y]
                demosaic[x,y,2] = Bayer[x-1,y-1]

print("RED:")
print(demosaic[:,:,0])
print("GREEN:")
print(demosaic[:,:,1])
print("BLUE:")
print(demosaic[:,:,2])

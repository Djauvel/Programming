import cv2 as cv
import numpy as np
import time
image = cv.imread("lion.jpg",0)
#thresholded = np.zeros((image.shape[0],image.shape[1]),np.uint8)

#Make an output array
dilatedImg = np.zeros((image.shape[0],image.shape[1]),np.uint8)
erodedImg = np.zeros((image.shape[0],image.shape[1]),np.uint8)

#Threshold image
ret,thresh1 = cv.threshold(image,150,255,cv.THRESH_BINARY)

#StructureElement
SE = np.ones((3,3))

kernelFactor = np.sum(SE)
kernelRadius = int((SE.shape[0] - 1) / 2)

threshHeight = int(thresh1.shape[0] - (kernelRadius*2))
threshWidth = int(thresh1.shape[1] - (kernelRadius*2))
temp = 0
start = time.time()
#Hit/dilation
for y in range(threshHeight):
    for x in range(threshWidth):
        for i in range(SE.shape[0]):
            for j in range(SE.shape[1]):
                if thresh1[y+i,x+j] == 255:
                    dilatedImg[y+1,x+1] = 255
end = time.time()

#Fit/eroded - skal fikses
for y in range(threshHeight):
    for x in range(threshWidth):
        for i in range(SE.shape[0]):
            for j in range(SE.shape[1]):
                if thresh1[y+i,x+j] == 255:
                    temp = temp + 1
                if temp == kernelFactor:
                    erodedImg[y+1,x+1] = 255
        temp = 0                


 
print(end-start)
cv.imshow("Original",image)
cv.imshow("Tresholded",thresh1)
cv.imshow("Dilated",dilatedImg)
cv.imshow("Eroded",erodedImg)
cv.waitKey(0)
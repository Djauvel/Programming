import time
import numpy as np
import cv2 as cv
start = time.time()

#Load image
image = cv.imread("lion.jpg",0)

#Make an output array depending on the dimension
if np.ndim(image) == 2:
    outputImg = np.zeros((image.shape[0],image.shape[1]),np.uint8)
elif np.ndim(image) == 3:
    outputImg = np.zeros((image.shape[0],image.shape[1],image.shape[2]),np.uint8)

#Smoothing Kernel
#kernel = np.array([[1,1,1],
#                  [1,1,1],
#                  [1,1,1]])

#kernel = np.ones((5,5))
#Gaussian Kernel
kernel = np.array([[1,2,1],
                  [2,4,2],
                  [1,2,1]])

# Computing the kernel factor for normalization:
kernelFactor = np.sum(kernel)
kernelRadius = int((kernel.shape[0] - 1) / 2)

imageHeight = int(image.shape[0] - (kernelRadius*2))
imageWidth = int(image.shape[1] - (kernelRadius*2))
TEMP = 0
TEMP1 = 0
TEMP2 = 0
if np.ndim(image) == 2:
    for y in range(imageHeight):
        for x in range(imageWidth):
            for i in range(kernel.shape[0]):
                for j in range(kernel.shape[1]):
                    TEMP += (kernel[i,j]*image[y+i,x+j])
            outputImg[y+kernelRadius,x+kernelRadius] = TEMP / kernelFactor
            TEMP = 0

elif np.ndim(image) == 3:
    for y in range(imageHeight):
        for x in range(imageWidth):
            for i in range(kernel.shape[0]):
                for j in range(kernel.shape[1]):
                    TEMP += (kernel[i,j]*image[y+i,x+j,0])
                    TEMP1 += (kernel[i,j]*image[y+i,x+j,1])
                    TEMP2 += (kernel[i,j]*image[y+i,x+j,2])
            outputImg[y+kernelRadius,x+kernelRadius,0] = TEMP / kernelFactor
            outputImg[y+kernelRadius,x+kernelRadius,1] = TEMP1 / kernelFactor
            outputImg[y+kernelRadius,x+kernelRadius,2] = TEMP2 / kernelFactor
            TEMP = 0
            TEMP1 = 0
            TEMP2 = 0

#Show image
cv.imshow("Original",image)
cv.imshow("BlurredImage",outputImg)
end = time.time()
print(end - start)
cv.waitKey(0)
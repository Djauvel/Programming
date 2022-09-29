import cv2 as cv
import numpy as np
import time
start = time.time()

image = cv.imread("lion.jpg",1)
#Make an output array depending on the dimension
if np.ndim(image) == 2:
    outputImg = np.zeros((image.shape[0],image.shape[1]),np.uint8)
elif np.ndim(image) == 3:
    outputImg = np.zeros((image.shape[0],image.shape[1],image.shape[2]),np.uint8)

#Smoothing Kernel
kernel = np.array([[1,1,1],
                  [1,1,1],
                  [1,1,1]])

#Gaussian Kernel
#kernel = np.array([[1,2,1],
#                  [2,4,2],
#                  [1,2,1]])

# Computing the kernel factor for normalization:
kernelFactor = np.sum(kernel)
kernelRadius = (kernel.shape[0] - 1) / 2

imageHeight = int(image.shape[0] - (kernelRadius*2))
imageWidth = int(image.shape[1] - (kernelRadius*2))

if np.ndim(image) == 2:
    for y in range(imageHeight):
        for x in range(imageWidth):
            for i in range(kernel.shape[0]):
                for j in range(kernel.shape[1]):
                    outputImg[y+1,x+1] = outputImg[y+1,x+1] + (kernel[i,j]*image[y+i,x+j])/kernelFactor

elif np.ndim(image) == 3:
    for z in range(image.shape[2]):
        for y in range(imageHeight):
            for x in range(imageWidth):
                for i in range(kernel.shape[0]):
                    for j in range(kernel.shape[1]):
                        outputImg[y+1,x+1,z] = outputImg[y+1,x+1,z] + (kernel[i,j]*image[y+i,x+j,z])/kernelFactor


cv.imshow("Original",image)
cv.imshow("BlurredImage",outputImg)
end = time.time()
print(end - start)
cv.waitKey(0)
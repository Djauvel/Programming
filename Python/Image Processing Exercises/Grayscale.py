import cv2 as cv
import numpy as np

Image = cv.imread("pepe.jpg",1)

GrayScale = np.zeros((Image.shape[0],Image.shape[1]),np.uint8)

h = range(Image.shape[0])
w = range(Image.shape[1])

Wr = 0.299
Wg = 0.587
Wb = 0.114

for y in h:
    for x in w:
        b = Image[y,x,0]
        g = Image[y,x,1]
        r = Image[y,x,2]

        I = Wr * r + Wg * g + Wb * b

        GrayScale[y,x] = I

cv.imshow("GrayScale",GrayScale)
cv.imshow("Original",Image)
cv.waitKey(0)
import cv2 as cv
import numpy as np

Image = cv.imread("monkey.jpg",1)

Wr = 0.299
Wg = 0.587
Wb = 0.114

GrayScale = np.array(Image @ [Wb,Wg,Wr], np.uint8)

cv.imshow("GrayScale",GrayScale)
cv.imshow("Original",Image)
cv.waitKey(0)
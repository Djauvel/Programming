import cv2 as cv
import numpy as np

Image = cv.imread("Python/Image Processing Exercises/monkey.jpg",1)

HSI = np.zeros((Image.shape[0],Image.shape[1],3),np.uint8)
h = range(Image.shape[0])
w = range(Image.shape[1])

for y in h:
    for x in w:
        b = int(Image[y,x,0])
        g = int(Image[y,x,1])
        r = int(Image[y,x,2])

        H = np.arccos(0.5 * ((r-g)+(r-b))/np.sqrt((r-g)*(r-g)+(r-b)*(g-b)+0.001))

        if g >= b:
            H = H
        else:
            H = ((360*np.pi)/180) - H

        S = 1 - 3 * (min(r,g,b)/(r+g+b+0.001))
        
        I = (r + g + b)/3

        HSI[y,x,2] = I
        HSI[y,x,1] = S
        HSI[y,x,0] = (H * 180)/np.pi
        

cv.imshow("REGULAR",Image)
cv.imshow("HSI",HSI)
cv.waitKey(0)
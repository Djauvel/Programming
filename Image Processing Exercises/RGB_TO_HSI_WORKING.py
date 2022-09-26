import cv2 as cv
import numpy as np

Image = cv.imread("monkey.jpg",1)

HSI = np.zeros((Image.shape[0],Image.shape[1],Image.shape[2]),np.uint8)
h = range(Image.shape[0])
w = range(Image.shape[1])

for y in h:
    for x in w:
        b = int(Image[y,x,0])
        g = int(Image[y,x,1])
        r = int(Image[y,x,2])
        # Determining if Green or Blue has the greatest value, computing the hue and saving in the appropriate index of the array:
        H = np.arccos(0.5 * ((r-g)+(r-b))/np.sqrt((r-g)*(r-g)+(r-b)*(g-b)+0.001))

        if g >= b:
            H = H
        else:
            H = ((360*np.pi)/180) - H

        # Determining the smallest value of R G and B, and saving as "min":
        mini = min(r,g,b)

        # Computing the saturation and saving in the array:
        S = 1 - 3 * (mini/(r+g+b+0.001))
        
        # Computing the intensity and saving it accordingly:
        I = (r + g + b)/3

        HSI[y,x,2] = I
        HSI[y,x,1] = S
        HSI[y,x,0] = (H * 180)/np.pi
        

cv.imshow("REGULARPEPE",Image)
cv.imshow("HSIPEPE",HSI)
#cv.imshow("HPEPE",HSI[:,:,0])
#cv.imshow("SPEPE",HSI[:,:,1])
#cv.imshow("IPEPE",HSI[:,:,2])
cv.waitKey(0)



#def smallestVal(a,b,c):
#    min = 0
#
#    if a < b and a < c:
#        min = a
#    if b < a and b < c:
#        min = b
#    if c < a and c < b:
#        min = c
#    return min

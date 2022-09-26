from cmath import pi
import cv2 as cv
import numpy as np

Image = cv.imread("Monke.jpg",1)

h = Image.shape[0]
w = Image.shape[1]
NoC = Image.shape[2]

HSI = np.zeros((h,w,3))

def smallestVal(a,b,c):
    min = 0

    if a < b and a < c:
        min = a
    if b < a and b < c:
        min = b
    if c < a and c < b:
        min = c
    return min

for y in range(h):
    for x in range(w):
        b = Image[y,x,0]
        g = Image[y,x,1]
        r = Image[y,x,2]
        # Determining if Green or Blue has the greatest value, computing the hue and saving in the appropriate index of the array:
        if g >= b:
            HSI[y,x,0] = np.arccos(1/2 * ((r-g)+(r-b))/np.sqrt((r-g)*(r-g)+(r-b)*(g-b)))
        else:
            HSI[y,x,0] = (360*np.pi)/180 - np.arccos(1/2 * ((r-g)+(r-b))/np.sqrt((r-g)*(r-g)+(r-b)*(g-b)))
        # Convert radians back to degrees
        HSI[y,x,0] = HSI[y,x,0]*180/np.pi
        # Determining the smallest value of R G and B, and saving as "min":
        min = smallestVal(b,g,r)

        # Computing the saturation and saving in the array:
        HSI[y,x,1] = 1 - 3 * (min/(b+g+r))
        
        #Round up/down large floats
        if HSI[y,x,1] > 0.99:
            HSI[y,x,1] = 1
        elif HSI[y,x,1] < 0.001:
            HSI[y,x,1] = 0
        
        # Computing the intensity and saving it accordingly:
        HSI[y,x,2] = (b + g + r)/3
        
cv.imshow("HSIPEPE",HSI)
#cv.imshow("HPEPE",HSI[:,:,0])
#cv.imshow("SPEPE",HSI[:,:,1])
#cv.imshow("IPEPE",HSI[:,:,2])
cv.waitKey(0)
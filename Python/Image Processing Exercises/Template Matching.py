import cv2 as cv
import numpy as np

# Reading image and template as grayscale
image = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/1.jpg",0)
template = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/Haar Cascade/Crown1.PNG",0)


# Apply template matching
res = abs(cv.matchTemplate(image,template,cv.TM_CCOEFF_NORMED))
res = np.ndarray.astype(res*255,np.uint8)

ret,thresh1 = cv.threshold(res,250,255,cv.THRESH_BINARY)
# If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum

print(3!)

#cv.imshow("matchTemplate",res)
cv.imshow("Thresholded",thresh1)
cv.imshow("Neon Text Original",image)
cv.waitKey(0)
import cv2 as cv
import numpy as np

#read image and convert to HSV
board = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/1.jpg",1)
board = cv.medianBlur(board,7)
HSVboard = cv.cvtColor(board,cv.COLOR_RGB2HSV)

#Determine dimensions of tiles
tileDimW = int(board.shape[0]/5)
tileDimH = int(board.shape[1]/5)

#Thresholding coefficients
lower = np.array([84,178,118])
upper = np.array([100,255,225])
high = 90
low = 255

#Apply mask to HSV image
mask = cv.inRange(HSVboard, lower, upper)
imageResult = cv.bitwise_and(board,board,mask=mask)

#Convert image with mask to grayscale
MaskT = cv.cvtColor(imageResult,cv.COLOR_BGR2GRAY)
kernel = np.ones((3,3)) 

MaskT = cv.morphologyEx(MaskT,cv.MORPH_OPEN,kernel)
MaskT = cv.morphologyEx(MaskT,cv.MORPH_CLOSE,kernel,iterations=2)
# MaskT = cv.erode(MaskT,kernel=kernel,iterations=1)
#MaskT = cv.dilate(MaskT,kernel=kernel,iterations=3)
#MaskT = cv.erode(MaskT,kernel=kernel,iterations=4)
#MaskT = cv.bitwise_not(MaskT)
#Threshhold
#kernel = np.ones((5,5))
#thresholded, thresh1 = cv.threshold(MaskT,low,high,cv.THRESH_BINARY)
#dilated = cv.dilate(thresh1,kernel,iterations=1)
#thresholded = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_MEAN_C,\
#            cv.THRESH_BINARY,11,2)

threshSliceList = []

#Slice image into 25 squares
for h in range(5):
    threshSliceList.append([])
    for w in range(5):
        threshSliceList[h].append(MaskT[tileDimH*h+5:tileDimH*(h+1)-5,tileDimW*w+5:tileDimW*(w+1)-5])

cv.imshow("Original",board)
cv.imshow("MaskT",MaskT)
cv.imshow("Tile",threshSliceList[3][1])
cv.waitKey(0)
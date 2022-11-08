import cv2 as cv
import numpy as np

def empty(a):
    pass
#0,164,0,78,119,170

#Other good one
#0 179 0 48 140 144

#Gaussian T
#83 117 21 48 98 176

#85 97 9 166 111 255
cv.namedWindow("TrackBars")
cv.resizeWindow("TrackBars",640,340)
cv.createTrackbar("Hue Min","TrackBars",83,179,empty)
cv.createTrackbar("Hue Max","TrackBars",117,179,empty)
cv.createTrackbar("Sat Min","TrackBars",21,255,empty)
cv.createTrackbar("Sat Max","TrackBars",115,255,empty)
cv.createTrackbar("Val Min","TrackBars",98,255,empty)
cv.createTrackbar("Val Max","TrackBars",176,255,empty)
cv.createTrackbar("Thresh Min","TrackBars",130,255,empty)
cv.createTrackbar("Thresh Max","TrackBars",255,255,empty)



while True:
    board = cv.imread("C:/Users/Petaa/Desktop/Skole/Programming/Python/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/1.jpg",1)
    HSV = cv.cvtColor(board,cv.COLOR_RGB2HSV)
    h_min = cv.getTrackbarPos("Hue Min","TrackBars")
    h_max = cv.getTrackbarPos("Hue Max","TrackBars")
    s_min = cv.getTrackbarPos("Sat Min","TrackBars")
    s_max = cv.getTrackbarPos("Sat Max","TrackBars")
    v_min = cv.getTrackbarPos("Val Min","TrackBars")
    v_max = cv.getTrackbarPos("Val Max","TrackBars")
    t_min = cv.getTrackbarPos("Thresh Min","TrackBars")
    t_max = cv.getTrackbarPos("Thresh Max","TrackBars")

    low = t_min
    high = t_max

    print(h_min,h_max,s_min,s_max,v_min,v_max)
    lower = np.array([h_min,s_min,v_min])
    upper = np.array([h_max,s_max,v_max])
    mask = cv.inRange(HSV, lower, upper)

    imageResult = cv.bitwise_and(board,board,mask=mask)
    imageResult = cv.medianBlur(imageResult,3)
    MaskT = cv.cvtColor(imageResult,cv.COLOR_BGR2GRAY)

    
    Thresholded, thresh1 = cv.threshold(MaskT,low,high,cv.THRESH_BINARY)
    th2 = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv.THRESH_BINARY,11,2)
    th3 = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_MEAN_C,\
            cv.THRESH_BINARY,11,2)
    cv.imshow("MaskT",MaskT)
    #cv.imshow("Blur",blur)
    #cv.imshow("Original",board)
    #cv.imshow("HSV",HSV)
    #cv.imshow("Mask",mask)
    cv.imshow("ImageResult",imageResult)
    #cv.imshow("erodedResult",erode)
    #cv.imshow("dilated",dilate)
    cv.imshow("Thresholded",thresh1)
    cv.imshow("Thresholded2",th2)
    cv.imshow("Thresholded3",th3)
    cv.waitKey(1)
#Other good one
#0 179 0 48 140 144

#0,179,95,255,0,255

#BEST ONE:
#0,164,0,78,119,170

#For Dilation:
#0,164,0,56,143,150
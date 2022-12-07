import cv2 as cv
import numpy as np

def empty(a):
    pass


#90 94 171 225 126 255
#84 100 178 255 118 190
#90 100 56 255 152 190
cv.namedWindow("TrackBars")
cv.resizeWindow("TrackBars",640,340)
cv.createTrackbar("Hue Min","TrackBars",90,179,empty)
cv.createTrackbar("Hue Max","TrackBars",100,179,empty)
cv.createTrackbar("Sat Min","TrackBars",133,255,empty)
cv.createTrackbar("Sat Max","TrackBars",255,255,empty)
cv.createTrackbar("Val Min","TrackBars",118,255,empty)
cv.createTrackbar("Val Max","TrackBars",190,255,empty)
cv.createTrackbar("Thresh Min","TrackBars",90,255,empty)
cv.createTrackbar("Thresh Max","TrackBars",255,255,empty)

kernel = np.ones((3,3))

while True:
    board = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/1.jpg",1)
    board = cv.medianBlur(board,3)
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
    imageResult = cv.morphologyEx(imageResult,cv.MORPH_OPEN,kernel,iterations=2)

    MaskT = cv.cvtColor(imageResult,cv.COLOR_BGR2GRAY)
    Thresholded, thresh1 = cv.threshold(MaskT,low,high,cv.THRESH_BINARY)

    #th2 = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            #cv.THRESH_BINARY,11,2)
    #th3 = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_MEAN_C,\
            #cv.THRESH_BINARY,11,2)
    #cv.imshow("MaskT",MaskT)
    cv.imshow("Original",board)
    #cv.imshow("HSV",HSV)
    cv.imshow("Mask",mask)
    cv.imshow("ImageResult",imageResult)
    #cv.imshow("erodedResult",erode)
    #cv.imshow("dilated",dilate)
    cv.imshow("Thresholded",thresh1)
    #cv.imshow("Thresholded2",th2)
    #cv.imshow("Thresholded3",th3)
    cv.waitKey(1)

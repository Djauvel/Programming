import cv2 as cv
import numpy as np

def empty(a):
    pass


#90 94 171 225 126 255
#84 100 178 255 118 190
#90 100 56 255 152 190
cv.namedWindow("TrackBars")
cv.resizeWindow("TrackBars",750,500)
cv.createTrackbar("Cyan Min","TrackBars",0,255,empty)
cv.createTrackbar("Cyan Max","TrackBars",255,255,empty)
cv.createTrackbar("Magenta Min","TrackBars",0,255,empty)
cv.createTrackbar("Magenta Max","TrackBars",255,255,empty)
cv.createTrackbar("Yellow Min","TrackBars",0,255,empty)
cv.createTrackbar("Yellow Max","TrackBars",255,255,empty)
cv.createTrackbar("Key Min","TrackBars",0,255,empty)
cv.createTrackbar("Key Max","TrackBars",255,255,empty)
cv.createTrackbar("Thresh Min","TrackBars",0,255,empty)
cv.createTrackbar("Thresh Max","TrackBars",255,255,empty)

kernel = np.ones((3,3))

board = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/1.jpg",1)

#CMYK conversion
img = board.astype(np.float64)/255.
K = 1 - np.max(img, axis=2)
C = (1-img[...,2] - K)/(1-K)
M = (1-img[...,1] - K)/(1-K)
Y = (1-img[...,0] - K)/(1-K)

CMYK_image= (np.dstack((C,M,Y,K)) * 255).astype(np.uint8)

while True:
    c_min = cv.getTrackbarPos("Cyan Min","TrackBars")
    c_max = cv.getTrackbarPos("Cyan Max","TrackBars")
    m_min = cv.getTrackbarPos("Magenta Min","TrackBars")
    m_max = cv.getTrackbarPos("Magenta Max","TrackBars")
    y_min = cv.getTrackbarPos("Yellow Min","TrackBars")
    y_max = cv.getTrackbarPos("Yellow Max","TrackBars")
    k_min = cv.getTrackbarPos("Key Min","TrackBars")
    k_max = cv.getTrackbarPos("Key Max","TrackBars")
    t_min = cv.getTrackbarPos("Thresh Min","TrackBars")
    t_max = cv.getTrackbarPos("Thresh Max","TrackBars")

    low = t_min
    high = t_max

    print(c_min,c_max,m_min,m_max,y_min,y_max,k_min,k_max)
    lower = np.array([c_min,m_min,y_min,k_min])
    upper = np.array([c_max,m_max,y_max,k_max])
    mask = cv.inRange(CMYK_image, lower, upper)
    mask = cv.morphologyEx(mask,cv.MORPH_OPEN,kernel,iterations=1)
    
    imageResult = cv.bitwise_and(board,board,mask=mask)
    imageResult = cv.morphologyEx(imageResult,cv.MORPH_OPEN,kernel,iterations=1)

    MaskT = cv.cvtColor(imageResult,cv.COLOR_BGR2GRAY)
    Thresholded, thresh1 = cv.threshold(MaskT,low,high,cv.THRESH_BINARY)

    th2 = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv.THRESH_BINARY,11,2)
    th3 = cv.adaptiveThreshold(MaskT,255,cv.ADAPTIVE_THRESH_MEAN_C,\
            cv.THRESH_BINARY,11,2)
    #cv.imshow("MaskT",MaskT)
    cv.imshow("Original",board)
    #cv.imshow("HSV",HSV)
    cv.imshow("Mask",mask)
    cv.imshow("ImageResult",imageResult)
    #cv.imshow("erodedResult",erode)
    #cv.imshow("dilated",dilate)
    cv.imshow("Thresholded",thresh1)
    cv.imshow("Thresholded2",th2)
    cv.imshow("Thresholded3",th3)
    cv.waitKey(1)

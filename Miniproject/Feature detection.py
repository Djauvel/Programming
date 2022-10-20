import cv2 as cv
import numpy as np

img1 = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/18.jpg",0)
img2 = cv.imread("Train1Crown1.PNG",0)
#img2 = cv.imread("BigCrown.PNG",0)

orb = cv.ORB_create(nfeatures=10000)

kp1, des1 = orb.detectAndCompute(img1,None)
kp2, des2 = orb.detectAndCompute(img2,None)

imKp1 = cv.drawKeypoints(img1,kp1,None)
imKp2 = cv.drawKeypoints(img2,kp2,None)

bf = cv.BFMatcher()
matches = bf.knnMatch(des1,des2, k=2)

good = []
for m,n in matches:
    if m.distance < 0.6*n.distance:
        good.append([m])

print(len(good))
img3 = cv.drawMatchesKnn(img1,kp1,img2,kp2,good,None,flags=2)

cv.imshow("Kp1",imKp1)
cv.imshow("Kp2",imKp2)
cv.imshow("img3",img3)
#cv.imshow("Query Image",img1)
#cv.imshow("Training image",img2)
cv.waitKey(0)
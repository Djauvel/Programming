import cv2 as cv
tinypic = cv.imread("tinypic.png",0)


#Range af arrayet tinypic, giver antallet af rækker- For hver række op til antallet af rækker
#skal vi for hver kolonne i rækken printe værdien af hver arrayværdi
for y in range (tinypic.shape[0]):
    for x in range (tinypic.shape[1]):
        print(tinypic[y,x])

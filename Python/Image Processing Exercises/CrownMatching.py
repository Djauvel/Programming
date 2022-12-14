import cv2 as cv
import numpy as np
from imutils.object_detection import non_max_suppression

board = cv.imread("Python/Miniproject/King Domino dataset/Cropped and perspective corrected boards/73.jpg",1)
GrassTemp = cv.imread("Python/Miniproject/GrassCrown.jpg",1)
ForestTemp = cv.imread("Python/Miniproject/ForestCrown.jpg",1)
LakeTemp = cv.imread("Python/Miniproject/LakeCrown.jpg",1)
SwampTemp = cv.imread("Python/Miniproject/SwampCrown.jpg",1)
WheatTemp = cv.imread("Python/Miniproject/WheatCrown.jpg",1)
MountainTemp = cv.imread("Python/Miniproject/MountainCrown.jpg",1)

tileDimW = int(board.shape[0]/5)
tileDimH = int(board.shape[1]/5)

crownCount = np.zeros((5,5))
boxes = list()

def detectCrown(tile,template,t):
    #Import templates and rotate to account for all possible orientations
    t1 = template
    t2 = cv.rotate(t1,cv.ROTATE_90_CLOCKWISE)
    t3 = cv.rotate(t2,cv.ROTATE_90_CLOCKWISE)
    t4 = cv.rotate(t3,cv.ROTATE_90_CLOCKWISE)
    templates = [t1, t2, t3, t4]

    global boxes

    for i in range(4):
        #Perform template matching
        res = cv.matchTemplate(tile,templates[i],cv.TM_CCOEFF_NORMED)

        #Determine coordinates where
        (y_coords,x_coords) = np.where(res >= t)

        #Determine size of crown for box dimensions
        w, h = templates[i].shape[:2]

        for (y,x) in zip(x_coords,y_coords):
            boxes.append((x,y,x+h,y+w))
def drawCrown():
    # Not ideal, will be fixed for final project
    detectCrown(board,ForestTemp,0.6)
    detectCrown(board,GrassTemp,0.6)
    detectCrown(board,LakeTemp,0.6)
    detectCrown(board,SwampTemp,0.6)
    detectCrown(board,WheatTemp,0.6)
    detectCrown(board,MountainTemp,0.7)
    global boxes
    boxes = non_max_suppression(np.array(boxes))

    for (y1,x1,y2,x2) in boxes:
        cv.rectangle(board,(x1,y1),(x2,y2),(0,255,0),2)
        x = x1//100
        y = y1//100
        crownCount[y,x] += 1
        
drawCrown()
print(crownCount)
cv.imshow("board",board)
cv.waitKey(0)
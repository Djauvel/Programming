import cv2 as cv
import numpy as np

board = cv.imread("King Domino dataset/Cropped and perspective corrected boards/32.jpg",1)
HSVFULL = cv.cvtColor(board,cv.COLOR_RGB2HSV)
tileDimW = int(board.shape[0]/5)
tileDimH = int(board.shape[1]/5)

sliceList = []
tileCol = np.chararray((5,5))

def returnCol(y,x):
    MeanV=np.mean(HSVFULL[:,:,2])
    bonana = np.array(sliceList[y][x])
    B = np.mean(bonana[:,:,0])
    G = np.mean(bonana[:,:,1])
    R = np.mean(bonana[:,:,2])
    hsv = cv.cvtColor(bonana, cv.COLOR_BGR2HSV)
    H = np.mean(hsv[:,:,0])
    S = np.mean(hsv[:,:,1])
    V = np.mean(hsv[:,:,2])
    VRel = V/MeanV
    #S = np.mean(hsv[y,x,1])
    #V = np.mean(hsv[y,x,2])
    BG = B/G
    BR = B/R
    GB = G/B
    GR = G/R
    RB = B/R
    RG = R/G
    AVG = (B+G+R)/3
    print("-----------------")
    print("Tile: (",y,",",x,")")
    print("BG: ",BG, " BR: ", BR)
    print("GB: ",GB, " GR: ", GR)
    print("RB: ",RB, " RG: ", RG)
    print("AVG: ",AVG, " MeanV: ", VRel)
    print("HSV: ", H, ", ", S, ",", V )
    if GB > 1 and GR >0.9 and VRel<0.65:
        tileCol[y,x] = "F"
    if G > R and G > B and VRel >0.8:
        tileCol[y,x] = "P"
    if B > R and B > G:
        tileCol[y,x] = "L"
    if R > B and R > G and 0.65<VRel<1.1:
        tileCol[y,x] = "S"
    if R > B and R > G and 0.6<VRel<0.7 and GB<1.5:
        tileCol[y,x] = "C"
    if R > B and RG > 1 and 1.5 <GB< 2 and VRel<0.70:
        tileCol[y,x] = "M"
    if GB > 3.8 and RG > 1:
        tileCol[y,x] = "W"
    #print("Slice: ",y, ",", x)
    print("R: ", R)
    print("G: ", G)
    print("B: ", B)
    print("Expected Tile: ", tileCol[y,x])
    print("-----------------")


for h in range(5):
    sliceList.append([])
    for w in range(5):
        sliceList[h].append(board[tileDimH*h:tileDimH*(h+1),tileDimW*w:tileDimW*(w+1)])
        returnCol(h,w)
print(tileCol)
cv.imshow("HSV",HSVFULL)
#cv.imshow("",sliceList[y][x])
cv.imshow("board",board)
cv.waitKey(0)

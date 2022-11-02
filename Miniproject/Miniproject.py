import cv2 as cv
import numpy as np

board = cv.imread("C:/Users/Petaa/Documents/GitHub/Python-Programming/Miniproject/King Domino dataset/Cropped and perspective corrected boards/41.jpg",1)
HSV= cv.cvtColor(board,cv.COLOR_RGB2HSV)
tileDimW = int(board.shape[0]/5)
tileDimH = int(board.shape[1]/5)

sliceList = []
tileCol = np.array((["","","","",""],
                    ["","","","",""],
                    ["","","","",""],
                    ["","","","",""],
                    ["","","","",""]))

def returnCol(y,x):
    MeanV=np.mean(HSV[:,:,2])
    bonana = np.array(sliceList[y][x])
    B = np.mean(bonana[:,:,0])
    G = np.mean(bonana[:,:,1])
    R = np.mean(bonana[:,:,2])
    hsv = cv.cvtColor(bonana, cv.COLOR_BGR2HSV)
    H = np.mean(hsv[:,:,0])
    S = np.mean(hsv[:,:,1])
    V = np.mean(hsv[:,:,2])
    VRel = V/MeanV
    BG = B/G
    BR = B/R
    GB = G/B
    GR = G/R
    RB = B/R
    RG = R/G
    HS = H/S
    HV = H/V
    SH = S/H
    SV = S/V
    VH = V/H
    VS = V/S
    print("-----------------")
    print("Tile: (",y,",",x,")")
    print("BG: ",BG, " BR: ", BR)
    print("GB: ",GB, " GR: ", GR)
    print("RB: ",RB, " RG: ", RG)
    print(" VRel: ", VRel)
    print("HS: ", HS," HV: ", HV)
    print("SH: ", SH," SV: ", SV)
    print("VH: ", VH," VS: ", VS)

    tileCol[y,x] = "U"
    if GB > 1.55 and GR >0.9 and VRel<0.65 and SV >1.6  and SH > 1.8:
        tileCol[y,x] = "F"
    if G > R and G > B and VRel >0.8 and GB > 1.5:
        tileCol[y,x] = "P"
    if B > R and B > G:
        tileCol[y,x] = "L"
    if R > B and R > G and 0.65<VRel<1.1 and 7.8>SH>2.85 and GB < 3:
        tileCol[y,x] = "S"
    if R > B and R > G and 0.6<VRel<0.7 and GB<1.5:
        tileCol[y,x] = "C"
    if R > B and RG > 1 and 1.5 <GB< 2.3 and VRel<0.70 and SH<5 and VH <3.2:
        tileCol[y,x] = "M"
    if GB > 3.4 and RG > 1 and SH<10.5:
        tileCol[y,x] = "W"
    
    #print("R: ", R)
    #print("G: ", G)
    #print("B: ", B)
    print("Expected Tile: ", tileCol[y,x])
    print("-----------------")

for h in range(5):
    sliceList.append([])
    for w in range(5):
        sliceList[h].append(board[tileDimH*h+5:tileDimH*(h+1)-5,tileDimW*w+5:tileDimW*(w+1)-5])
        returnCol(h,w)

print(tileCol)
#cv.imshow("HSV",HSV)
cv.imshow("Test",sliceList[4][2])
cv.imshow("board",board)
cv.waitKey(0)

#Tile: ( 1 , 2 )
#BG:  0.29104441041347623  BR:  0.2620724423763853
#GB:  3.4359017532043863  GR:  0.9004551642275778
#RB:  0.2620724423763853  RG:  1.110549464012251
# VRel:  0.9215744425479827
#HS:  0.32567508934195843  HV:  0.45590528566413857
#SH:  3.0705449471758683  SV:  1.3998776712867915
#VH:  2.1934380483070144  VS:  0.7143481323484379
#Expected Tile:  U

#Tile: ( 4 , 0 )
#BG:  0.289262744103759  BR:  0.21865812073809548
#GB:  3.4570646250984134  GR:  0.7559152541941677
#RB:  0.21865812073809548  RG:  1.322899616658795
 #VRel:  1.555349202187981
#HS:  0.10373646097666647  HV:  0.1421365556391341
#SH:  9.639812179682233  SV:  1.370169700228206
#VH:  7.035487777956767  VS:  0.729836603329826
#Expected Tile:  U
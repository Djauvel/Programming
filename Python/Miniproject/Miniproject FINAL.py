import cv2 as cv
import numpy as np
from imutils.object_detection import non_max_suppression
from collections import deque

#Load board and convert to HSV
board = cv.imread("Miniproject/King Domino dataset/Cropped and perspective corrected boards/1.jpg",1)
HSV= cv.cvtColor(board,cv.COLOR_RGB2HSV)

#Loading crown templates for each tile type
GrassTemp = cv.imread("Miniproject/GrassCrown.jpg",1)
ForestTemp = cv.imread("Miniproject/ForestCrown.jpg",1)
LakeTemp = cv.imread("Miniproject/LakeCrown.jpg",1)
SwampTemp = cv.imread("Miniproject/SwampCrown.jpg",1)
WheatTemp = cv.imread("Miniproject/WheatCrown.jpg",1)
MountainTemp = cv.imread("Miniproject/MountainCrown.jpg",1)
#Templates and their threshold
templates =[(GrassTemp,0.6),(ForestTemp,0.6),(LakeTemp,0.7),(SwampTemp,0.7),(WheatTemp,0.7),(MountainTemp,0.7)]

#Calculate individual tile dimensions:
tileDimW, tileDimH = int(board.shape[0]/5), int(board.shape[1]/5)

#Containers for crown detection and bounding box
crownCount = np.zeros((5,5))
boxes = list()

#Containers for tile type detection:
sliceList = []
tileCol = np.array((["","","","",""],
                    ["","","","",""],
                    ["","","","",""],
                    ["","","","",""],
                    ["","","","",""]))

#Grassfire Containers:
Islands = np.zeros((5,5))
tileTypes = ["L","P","S","M","W","F","U"]
#Final score
score = 0

#Function for determining tile type:
def returnCol(y,x):
    #Extract a ton of descriptive features of each type of tile:
    #like mean values for HSV, RGB and value
    MeanV=np.mean(HSV[:,:,2])
    Tile = np.array(sliceList[y][x])
    B = np.mean(Tile[:,:,0])
    G = np.mean(Tile[:,:,1])
    R = np.mean(Tile[:,:,2])
    hsv = cv.cvtColor(Tile, cv.COLOR_BGR2HSV)
    H = np.mean(hsv[:,:,0])
    S = np.mean(hsv[:,:,1])
    V = np.mean(hsv[:,:,2])
    VRel = V/MeanV
    GB = G/B
    GR = G/R
    RG = R/G
    SH = S/H
    SV = S/V
    VH = V/H

    #Determine tile type based on features and storing in tileCol array
    tileCol[y,x] = "U"
    if GB > 1.4 and GR >0.9 and VRel<0.65 and SV >1.6  and SH > 1.8:
        tileCol[y,x] = "F"
    if G > R and G > B and VRel >0.70 and GB > 1.5:
        tileCol[y,x] = "P"
    if B > R and B > G:
        tileCol[y,x] = "L"
    if R > B and R > G and 0.65<VRel<1.1 and 7.8>SH>1.75 and GB < 3:
        tileCol[y,x] = "S"
    if R > B and RG > 1 and 1.5 <GB< 2.3 and VRel<0.70 and SH<5 and VH <3.2:
        tileCol[y,x] = "M"
    if GB > 3.4 and RG > 1 and SH<12:
        tileCol[y,x] = "W"
#----------------------------------
#Functions for detecting and drawing crowns
def detectCrown(tile,template,t):
    #Import templates and rotate to account for all possible orientations
    t1 = template
    t2 = cv.rotate(t1,cv.ROTATE_90_CLOCKWISE)
    t3 = cv.rotate(t2,cv.ROTATE_90_CLOCKWISE)
    t4 = cv.rotate(t3,cv.ROTATE_90_CLOCKWISE)
    templates = [t1, t2, t3, t4]

    #Access global list, "boxes"
    global boxes
    
    #For each template, run template matching.
    for i in range(4):
        #Perform template matching
        res = cv.matchTemplate(tile,templates[i],cv.TM_CCOEFF_NORMED)
        #Determine coordinates of "hotspots" in the template matched image- extract these coordinates.
        (y_coords,x_coords) = np.where(res >= t)

        #Determine size of crown for box dimensions
        w, h = templates[i].shape[:2]

        
        #Generate coordinates for bounding box placement
        for (y,x) in zip(x_coords,y_coords):
            boxes.append((x,y,x+h,y+w))
#Draw crowns on board
def drawCrown():
    #Detect all matches of each type of template and save in boxes
    for (template,threshold) in templates:
        detectCrown(board,template,threshold)
    global boxes
    #Determine best matching box for each crown using non_max_suppression
    boxes = non_max_suppression(np.array(boxes))

    #Draw bounding boxes on board and save the detected crown in crownCount array, in its respective index
    for (y1,x1,y2,x2) in boxes:
        cv.rectangle(board,(x1,y1),(x2,y2),(0,255,0),2)
        x = x1//tileDimW
        y = y1//tileDimH
        crownCount[y,x] += 1
#---------------------------------
#Grassfire implementation:
def ignite_fire(image,coordinates,current_id):
    burn_queue = deque([])
    something_burned = False

    if image[coordinates[0],coordinates[1]] == 1:
        burn_queue.append(coordinates)
        something_burned = True
    
    while len(burn_queue) > 0:
        current_pos = burn_queue.pop()
        y, x = current_pos
        # Burn current_pos with current id
        image[y, x] = current_id
        # Add connections to burn_queue
        if y - 1 >= 0 and image[y - 1, x] == 1:
            burn_queue.append((y - 1, x))
        if x - 1 >= 0 and image[y, x - 1] == 1:
            burn_queue.append((y, x - 1))
        if y + 1 < image.shape[0] and image[y + 1, x] == 1:
            burn_queue.append((y + 1, x))
        if x + 1 < image.shape[1] and image[y, x + 1] == 1:
            burn_queue.append((y, x + 1))
    
    if something_burned:
        current_id +=1
    
    return current_id, image
#Function takes a matrix and a letter corresponding to a tile type to generate a matrix of 1's
#for every specified letter, and 0 for everything else (To detect islands with grassfire)    
def retCmap(type,letter):
    for y in range(5):
        for x in range(5):
            if tileCol[y,x] == letter:
                type[y,x] = 1
            else:
                type[y,x] = 0
    next_id = 2
    for y in range(5):
        for x in range(5):
            next_id, type = ignite_fire(type, (y, x), next_id)
    
    return type
#---------------------------------
#Calculate total points for specified tiletype - CAN ONLY BE RUN WHEN TILETYPES, CROWNS AND ISLANDS HAVE BEEN COMPUTED
def calcPoints(arr):
    sum = 0
    #If sum of array is 0, it means that no tiles of this type exists on the board
    #Return 0
    if np.sum(arr) == 0:
        return 0
    #For each separate "island", count the number of tiles, check if any crowns have been detected on these tiles
    for z in range(2,(len(np.unique(arr))+1)):
        tiles = 0
        crowns = 0
        for y in range(5):
            for x in range(5):
                if arr[y,x] == z:
                    tiles += 1
                if crownCount[y,x] > 0 and arr[y,x] == z:
                    crowns += crownCount[y,x]
        #Determine the points for each island and add to sum
        sum += crowns * tiles
    return sum           


#_-----------------------------------ACTUALLY RUNNING THE PROGRAM---------------------------------
#For loop to slice board into tiles and draw tile type on board
for h in range(5):
    sliceList.append([])
    for w in range(5):
        sliceList[h].append(board[tileDimH*h+5:tileDimH*(h+1)-5,tileDimW*w+5:tileDimW*(w+1)-5])
        returnCol(h,w)
        cv.putText(board,f"{tileCol[h,w]}",((w)*100+40,(h)*100+60),cv.FONT_ITALIC,1,(255,255,150),4,cv.LINE_AA)

#Find and draw crowns on the board
drawCrown()

#When all crowns and tiles have been determined, run grassfire for each type and calculate the points contained
for type in tileTypes:
    retCmap(Islands,type)
    score += calcPoints(Islands)

#Draw final score on board and show board
cv.putText(board,f"Score: {score}",(20,(board.shape[1]-20)),cv.FONT_HERSHEY_COMPLEX,1,(0,0,255),2)
print(f"This board has a total score of: {score}")
cv.imshow("board",board)
cv.waitKey(0)
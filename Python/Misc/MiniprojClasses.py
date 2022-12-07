class Tray:
    def __init__(self,height, width, length, cellsPerRow, numRows):
        self.height = height
        self.width = width
        self.length = length
        self.cellsPerRow = cellsPerRow
        self.numRows = numRows
        self.numCells = numRows*cellsPerRow
        pass
    #Set height, width and length
    def setHWL(self,h, w, l):
        self.height = h
        self.width = w
        self.length = l
    #Set cellsPerRow and numRows
    def set_CPR_and_numRows(self, CPR, NR):
        self.cellsPerRow = CPR
        self.numRows = NR
    #Calculate first cell position (x,z)
    def calcCellData(self):
        cellSpacing = self.width/self.cellsPerRow
        self.xList = []
        self.zList = []
        #Making lists of x and z coordinates for the center of tray cells
        for n in range(1,self.cellsPerRow+1,1):
            self.xList.append((cellSpacing * n - 0.5 * cellSpacing)*0.001)
        for m in range(1,self.numRows+1,1):
            self.zList.append((cellSpacing * m -0.5 * cellSpacing)*0.001)

class trayWell(object):
    def __init__(self,x,z):
        self.xPos = x
        self.zPos = z
    def setPos(self,x,z):
        self.xPos = x
        self.zPos = z

#Instantiating the tray with measurements; 50mm x 325mm x 525mm with 8 wells per row and 13 rows.
#Calls the method in the object, to calculate two lists of all x and z coordinates.
my_Tray = Tray(50,325,525,8,13)
my_Tray.calcCellData()
#Making a list to contain objects that represent each tray well, with a unique set of (x,z) coordinates
#The For-Loop underneath generates said list, which in this case generates 104 objects. 
cellCoordList = []
for b in range(0,my_Tray.cellsPerRow):
    for c in range (0,my_Tray.numRows):
        cellCoordList.append(trayWell(my_Tray.xList[b],my_Tray.zList[c]))

#Access a specific entry in the list (first entry is at 0, last entry is at 103):
#print(cellCoordList[103].xPos, cellCoordList[103].zPos)

#Print entire list of coordinates:
for obj in cellCoordList:
    print(obj.xPos, obj.zPos)

class SeedType:
    def __init__(self, name, size, sowDepth):
        self.name = name
        self.size = size
        self.sowDepth = sowDepth
    def changeName(self, newName):
        self.name = newName
    def changeSize(self, newSize):
        self.size = newSize
    def changeSowDepth(self, newSowDepth):
        self.sowdepth = newSowDepth

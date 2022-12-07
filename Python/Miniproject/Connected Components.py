from collections import deque
import numpy as np
tileCol = np.array((["L","L","L","L","L"],
                    ["L","P","P","S","L"],
                    ["P","W","P","M","L"],
                    ["P","W","M","M","L"],
                    ["W","W","W","W","W"]))

lakes = np.zeros((5,5))
pastures = np.zeros((5,5))
swamps = np.zeros((5,5))
mountains = np.zeros((5,5))
wheats = np.zeros((5,5))
forests = np.zeros((5,5))

#Return componentmap
def retCmap(type,letter):
    for y in range(5):
        for x in range(5):
            if tileCol[y,x] == letter:
                type[y,x] = 1
            else:
                type[y,x] = 0 
    print(type)   

retCmap(lakes,"L")
retCmap(pastures,"P")
retCmap(swamps,"S")
retCmap(mountains,"M")
retCmap(wheats,"W")
retCmap(forests,"F")

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

def returnGroups(image):
    next_id = 2
    for y in range(5):
        for x in range(5):
            next_id, image = ignite_fire(image, (y, x), next_id)
    
    return image

components = [lakes,pastures,swamps,mountains,wheats,forests]

for x in range(len(components)):
    returnGroups(components[x])

print(tileCol)
print(pastures)

#print("pastures:", pastures)
#print("swamps:", swamps)
#print("mountains:", mountains)
#print("wheats:", wheats)
#print("forests:", forests)
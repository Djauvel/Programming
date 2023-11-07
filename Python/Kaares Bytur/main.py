import pygame
from random import randrange
import numpy as np

# Setup
pygame.init()
screen = pygame.display.set_mode((1320,800))
clock = pygame.time.Clock()
pygame.display.set_caption("Kaare's Bytur")
running = True
dt = 0
FPS = 60

# Settings
height = screen.get_height()
width = screen.get_width()


gameSpeed = 1
parallaxSpeed = 1


# Game score handling
score = 0
font = pygame.font.Font(None,36)


# Loading Textures
doobie_tex = pygame.image.load("Textures/Doob.png")
dice_tex = pygame.image.load("Textures/dice.png")
bass_tex = pygame.transform.scale(pygame.image.load("Textures/Animations/Bassrun/bass_idle.png"),(width*0.1,height*0.2))
carrow_tex = pygame.transform.scale_by(pygame.image.load("Textures/Carrow.png"),0.3)
hole_tex = pygame.image.load("Textures/hole.png")

# Loading platform Textures
platTex = []
for i in range(1,5):
    image = pygame.transform.scale_by(pygame.image.load(f"Textures/Platforms/{i}.png").convert_alpha(),0.3)
    platTex.append(image)

# Running Character Animation Import
bassRun = []
for i in range(1,9):
    image = pygame.transform.scale(pygame.image.load(f"Textures/Animations/Bassrun/{i}.png").convert_alpha(),(width*0.1,height*0.2))
    bassRun.append(image)

# Scrolling Parallax Background Implementation
bg_images = []
for i in range(1,7):
    #FIX road texture (6) gap der kommer. +11 er et temporary fix, der også bliver applied til alle andre billeder i baggrunden
    bg_image = pygame.transform.scale(pygame.image.load(f"Textures/Animations/Background/{i}.png").convert_alpha(),(width,height))
    bg_images.append(bg_image)

x1, y1 = 0, width
x2, y2 = 0, width
x3, y3 = 0, width
x4, y4 = 0, width
x5, y5 = 0, width
x6, y6 = 0, width

# Storing rects for things we want to check collision on. e.g. platforms and borders
tiles = []
platforms = []

all_sprites = pygame.sprite.Group()

# Classes
class doobie(pygame.sprite.Sprite):
    # Class for doobie item
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = doobie_tex
        self.image = pygame.transform.scale_by(self.image,0.1)
        self.pos = pygame.Vector2(width + randrange(300), randrange(100, height-200))

    # Newitem currently just teleports the object to appear again on screen after leaving the screen - Will be fixed
    def newitem(self):
        self.pos.x, self.pos.y = width + randrange(300), randrange(100, height-200)

    def update(self):
        screen.blit(self.image, self.pos)
        self.dist_to_player = np.sqrt((self.pos.y-bass.player_rect.y)**2 + (self.pos.x-bass.player_rect.x)**2)
        
        if self.dist_to_player < 75:
            self.newitem()
            global score 
            score += 100
        
        # Move item to left
        self.pos[0] -= 5 * gameSpeed

        # If item goes off-screen, generate new    
        
        if self.pos[0] < 0:
            self.newitem()

class dice(pygame.sprite.Sprite):
    # Class for dice item
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = dice_tex
        self.image = pygame.transform.scale_by(self.image,0.2)
        self.pos = pygame.Vector2(width + randrange(300), randrange(100, height-200))

    def newitem(self):
        self.pos.x, self.pos.y = width + randrange(300), randrange(100, height-200)

    def update(self):
        screen.blit(self.image, self.pos)
        self.dist_to_player = np.sqrt((self.pos.y-bass.player_rect.y)**2 + (self.pos.x-bass.player_rect.x)**2)
        if self.dist_to_player < 75:
            self.newitem()
            global score
            score += 250

        # Move item to left
        self.pos[0] -= 5 * gameSpeed

        # If item goes off-screen, generate new    
        
        if self.pos[0] < 0:
            self.newitem()

class character(pygame.sprite.Sprite):
    # Class for character
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        
        # Spawn location
        self.player_pos = pygame.Vector2((width / 2, height / 2))
        self.frames = bassRun
        self.animationID = 1
        self.frameID = 0
        self.image = bass_tex
        self.height = self.frames[0].get_height()
        self.width = self.frames[0].get_width()
        self.player_rect = pygame.Rect(self.player_pos.x, self.player_pos.y, self.width, self.height)
        self.flying = False

        self.movingR = False
        self.movingL = False

        self.move_speed = width * 0.01
        self.gravity = height * 0.005
        self.jumpStrength = height * 0.05
        self.maxYVel = 15
        self.player_y_momentum = 0
        self.jumpCount = 2

    def runAnimation(self):
        if self.flying == False:
            #Flips Texture depending on moving direction
                #FIX ANIMATION SPEED CHOICE
                if self.frameID*10 % 50 == 0:
                    if self.movingL == True:
                        self.image = pygame.transform.flip(self.frames[self.animationID], True, False)
                    elif self.movingR == True:
                        self.image = self.frames[self.animationID]
                    else:
                    #   Run Idle animation 
                        self.image = bass_tex
                    if self.animationID >= 7:
                        self.animationID = 0
                    else:
                        self.animationID += 1

                if self.frameID >= FPS:
                    self.frameID = 0
                else:
                    self.frameID += 1

        else:
            # flip sprite according to movement direction mid air
            if self.movingL == True:
                self.image = pygame.transform.flip(self.frames[7], True, False)
            else:
                self.image = self.frames[7]

    def update(self):
            self.runAnimation()
            screen.blit(self.image, (self.player_rect.x,self.player_rect.y))
            keys = pygame.key.get_pressed()

            # Reset intended movement every iteration
            self.player_movement = [0, 0]
            
            # Reset move direction booleans
            self.movingL = False
            self.movingR = False
            
            # Side movement and jumping
            if keys[pygame.K_a]:
                self.player_movement[0] -= self.move_speed
                self.movingL = True
            if keys[pygame.K_d]:
                self.player_movement[0] += self.move_speed
                self.movingR = True
            if keys[pygame.K_SPACE]:
                self.player_y_momentum -= self.jumpStrength
                
            # Gravity and player moving with road
            self.player_y_momentum += self.gravity
            self.player_movement[0] -= 5 * gameSpeed 

            # Limiting max fall and jump momentum
            if self.player_y_momentum > self.maxYVel:
                self.player_y_momentum = self.maxYVel 
            elif self.player_y_momentum < -self.maxYVel:
                self.player_y_momentum = -self.maxYVel 

            self.player_movement[1] += self.player_y_momentum
            

            # Sending move command
            self.player_rect, self.collisions = move(self.player_rect, self.player_movement, tiles, platforms)

            # Check whether the player is flying
            if self.player_y_momentum != 0 and (self.collisions["bottom"] or self.collisions["platform"]):
                self.flying = False
            else:
                self.flying = True

class platform(pygame.sprite.Sprite):
    def __init__(self) -> None:
        #FIX platform Collision
        pygame.sprite.Sprite.__init__(self)
        if randrange(0,11) >= 8:
            self.icy = True
        else:
            self.icy = False

        if self.icy:
            self.image = platTex[randrange(2,4)]
        else:
            self.image = platTex[randrange(0,2)]
        
        self.width = self.image.get_width()
        self.height = self.image.get_height()
        self.pos = pygame.Vector2(width + randrange(300),randrange(30,height-300))
        self.rect = pygame.Rect(self.pos.x, self.pos.y, self.width , self.height)
        
        platforms.append(self)
        all_sprites.add(self)

    def update(self):
        screen.blit(self.image, (self.rect.x, self.rect.y))
        self.rect.x -= 5 * gameSpeed

        if self.rect.x < 0-self.width:
            self.kill()

class hole(pygame.sprite.Sprite):
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = hole_tex
        self.image = pygame.transform.scale_by(self.image,0.5)
        self.pos = pygame.Vector2(width + randrange(300), height-100)
        self.width = self.image.get_width()

    def newitem(self):
        self.pos.x, self.pos.y = width + randrange(300), height - 100

    def update(self):
        screen.blit(self.image, self.pos)
        self.dist_to_player = np.sqrt((self.pos.y-bass.player_rect.y)**2 + (self.pos.x-bass.player_rect.x)**2)
        if self.dist_to_player < 75:
            global score
            score -= 250

        # Move item to left
        self.pos.x -= 5 * gameSpeed

        # If item goes off-screen, generate new    
        
        if self.pos.x < 0-self.width:
            self.newitem()

# Functions
# FIX CODE (IS UGLY AS)
def draw_bg_inf():
    global x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6, gameSpeed
    screen.blit(bg_images[0],(x1,0))
    screen.blit(bg_images[0],(y1,0))

    screen.blit(bg_images[1],(x2,0))
    screen.blit(bg_images[1],(y2,0))

    screen.blit(bg_images[2],(x3,0))
    screen.blit(bg_images[2],(y3,0))

    screen.blit(bg_images[3],(x4,0))
    screen.blit(bg_images[3],(y4,0))

    screen.blit(bg_images[4],(x5,0))
    screen.blit(bg_images[4],(y5,0))

    screen.blit(bg_images[5],(x6,0))
    screen.blit(bg_images[5],(y6,0))

    x1 -= 0.25 * parallaxSpeed * gameSpeed
    y1 -= 0.25 * parallaxSpeed * gameSpeed
    x2 -= 0.5 * parallaxSpeed * gameSpeed
    y2 -= 0.5 * parallaxSpeed * gameSpeed
    x3 -= 0.75 * parallaxSpeed * gameSpeed
    y3 -= 0.75 * parallaxSpeed * gameSpeed
    x4 -= 1 * parallaxSpeed * gameSpeed
    y4 -= 1 * parallaxSpeed * gameSpeed
    x5 -= 1.25 * parallaxSpeed * gameSpeed
    y5 -= 1.25 * parallaxSpeed * gameSpeed
    x6 -= 5 * parallaxSpeed * gameSpeed
    y6 -= 5 * parallaxSpeed * gameSpeed

    if x1 < -width:
        x1 = width
    if x2 < -width:
        x2 = width
    if x3 < -width:
        x3 = width
    if x4 < -width:
        x4 = width
    if x5 < -width:
        x5 = width
    if x6 < -width:
        x6 = width
    if y1 < -width:
        y1 = width
    if y2 < -width:
        y2 = width
    if y3 < -width:
        y3 = width
    if y4 < -width:
        y4 = width
    if y5 < -width:
        y5 = width
    if y6 < -width:
        y6 = width

def collision_test(rect, tiles, platforms):
    collisions = []
    plat_collisions = []
    for tile in tiles:
        if rect.colliderect(tile):
            collisions.append(tile)

    for plat in platforms:
        if rect.colliderect(plat.rect):
            plat_collisions.append(plat.rect)
            
    return collisions, plat_collisions

def move(rect, movement, tiles, platforms):
    # Dictionary storing type of collision in the movement
    collision_types = {"top" : False, "bottom" : False, "right" : False, "left" : False, "platform" : False}
    # Perform movement
    rect.x += movement[0]

    # Check for collisions with any objects in tiles
    collisions, platform_collisions = collision_test(rect, tiles, platforms)

    # For each collision on the x-axis, if moving right, set position of rect's right side to the left side of the colliding object and vice versa.
    for collision in collisions:
        if movement[0] > 0:
            rect.right = collision.left
            collision_types["right"] = True
        elif movement[0] < 0:
            rect.left = collision.right
            collision_types["left"] = True

    # Platforms are full collision
    for collision in platform_collisions:
        if movement[0] > 0:
            rect.right = collision.left
            collision_types["right"] = True
        elif movement[0] < 0:
            rect.left = collision.right
            collision_types["left"] = True
    
    # Same but for y-axis
    rect.y += movement[1]
    collisions, platform_collisions = collision_test(rect, tiles, platforms)
    for collision in collisions:
        if movement[1] > 0:
            rect.bottom = collision.top
            collision_types["bottom"] = True
        elif movement[1] < 0:
            rect.top = collision.bottom
            movement[1] = 0
            collision_types["top"] = True

    # Platforms are full collision
    for collision in platform_collisions:
        if movement[1] > 0:
            rect.bottom = collision.top
            collision_types["bottom"] = True
        elif movement[1] < 0:
            rect.top = collision.bottom
            movement[1] = 0
            collision_types["top"] = True
    
    # Platforms have Terraria Platform behavior
    #for collision in platform_collisions:
    #    if movement[1] > 0:
    #        rect.bottom = collision.top
    #        collision_types["platform"] = True
    #        # Allow player to clip through platform if pressing s
    #        if keys[pygame.K_s]:
    #            rect.top = collision.bottom
            
    # Reset falling momentum if the player is standing on something
    if collision_types["top"]:
        movement[1] = 0
        

    return rect, collision_types

def carrow():
    if bass.player_rect.y < 0:
        screen.blit(carrow_tex, (bass.player_rect.x, 0))


# Instatiate object classes
doob = doobie()
die = dice()
bass = character()
hullet = hole()

all_sprites.add(doob)
all_sprites.add(die)
all_sprites.add(bass)
all_sprites.add(hullet)

ground_rect =       pygame.Rect(0, height - 50 , width, 1000)
top_rect =          pygame.Rect(0,-5000+height,width,100)
right_wall_rect =   pygame.Rect(width, -5000 + height, 100, 5000)
left_wall_rect =    pygame.Rect(-100, -5000 + height , 100, 5000)

frameCount = 0

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        
    # Display Background
    draw_bg_inf()

    #Making world borders
    tiles.append(ground_rect)
    tiles.append(top_rect)
    tiles.append(right_wall_rect)
    tiles.append(left_wall_rect)

    # Roll to make new platform
    if frameCount == 140:
        platform()
        frameCount = 0
    else:
        frameCount += 1

    # Update all objects
    all_sprites.update()


    # Draw Score
    score_text = font.render(f"Score: {score}", True, (255,255,255), (0,0,0))
    screen.blit(score_text, (10,10))
    

    keys = pygame.key.get_pressed()

    # Draw Carrow
    carrow()
    

    # Update display with changes made above
    pygame.display.update()

    # limits FPS to 60
    # dt is delta time in seconds since last frame, used for framerate-
    # independent physics.
    dt = clock.tick(FPS) / 1000

    # Enables closing game on ESC
    if keys[pygame.K_ESCAPE]:
        running = False

pygame.quit()
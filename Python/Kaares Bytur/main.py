import pygame
from random import randrange
import numpy as np

# Setup
pygame.init()
screen = pygame.display.set_mode((1920,1080))
clock = pygame.time.Clock()
running = True
dt = 0
FPS = 60

# Settings
height = screen.get_height()
width = screen.get_width()
move_speed = width * 0.25
isJump = False
jumpCount = 10
gravity = 10


# Game score handling
score = 0
font = pygame.font.Font(None,36)


player_pos = pygame.Vector2((width / 2, height - height*0.9))


# Loading Textures
doobie_tex = pygame.image.load("Textures/Doob.png")
dice_tex = pygame.image.load("Textures/dice.png")
bass_tex = pygame.transform.scale_by(pygame.image.load("Textures/bass.png"), 0.1)
background_tex = pygame.transform.scale(pygame.image.load("Textures/grass.png"),(width,height))

# Scrolling Background Implementation
bg_images = []
for i in range(1,6):
    bg_image = pygame.transform.scale(pygame.image.load(f"Textures/Background/{i}.png").convert_alpha(),(width,height))
    bg_images.append(bg_image)

x1, y1 = 0, width
x2, y2 = 0, width
x3, y3 = 0, width
x4, y4 = 0, width
x5, y5 = 0, width


# Classes
class doobie(pygame.sprite.Sprite):
    # Class for doobie item
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = doobie_tex
        self.image = pygame.transform.scale_by(self.image,0.1)
        self.pos = [width + 300,randrange(height)]

    def newitem(self):
        self.pos = [width + 300,randrange(height)]

    def update(self):
        screen.blit(self.image, self.pos)
        self.dist_to_player = np.sqrt((self.pos[1]-player_pos[1])**2 + (self.pos[0]-player_pos[0])**2)
        
        if self.dist_to_player < 75:
            self.newitem()
            global score 
            score += 100
        
        # Move item to left
        self.pos[0] -= 5

        # If item goes off-screen, generate new    
        
        if self.pos[0] < 0:
            self.newitem()

class dice(pygame.sprite.Sprite):
    # Class for dice item
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = dice_tex
        self.image = pygame.transform.scale_by(self.image,0.2)
        self.pos = [width + 300,randrange(height)]

    def newitem(self):
        self.pos = [width + 300,randrange(height)]

    def update(self):
        screen.blit(self.image, self.pos)
        self.dist_to_player = np.sqrt((self.pos[1]-player_pos[1])**2 + (self.pos[0]-player_pos[0])**2)
        if self.dist_to_player < 75:
            self.newitem()
            global score
            score += 250

        # Move item to left
        self.pos[0] -= 5

        # If item goes off-screen, generate new    
        
        if self.pos[0] < 0:
            self.newitem()

class character(pygame.sprite.Sprite):
    # Class for character
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = bass_tex
        self.image = pygame.transform.scale_by(self.image,0.2)
        self.height = self.image.get_height()
        self.width = self.image.get_width()

    def update(self):
            global move_speed, isJump, jumpCount, gravity
            screen.blit(self.image, player_pos)
            keys = pygame.key.get_pressed()

            # Player Control
            if keys[pygame.K_a] and player_pos.x > 0:
                player_pos.x -= move_speed * dt
            if keys[pygame.K_d] and player_pos.x < width - bass.width:
                player_pos.x += move_speed * dt
            if not (isJump):
                if player_pos.y < height*0.8:
                    player_pos.y += gravity
                if keys[pygame.K_s] and player_pos.y < height - bass.height:
                    player_pos.y += move_speed * dt
                if keys[pygame.K_SPACE]:
                    isJump = True
            else:
                if jumpCount >= -10:
                    player_pos.y -= (jumpCount * abs(jumpCount) * 0.5)
                    jumpCount -= 1
                else: 
                    isJump = False
                    jumpCount = 10

# Functions
# FIX CODE (IS UGLY AS)
def draw_bg_inf():
    global x1, x2, x3, x4, x5, y1, y2, y3, y4, y5
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

    x1 -= 3
    y1 -= 3
    x2 -= 3.5
    y2 -= 3.5
    x3 -= 4
    y3 -= 4
    x4 -= 4.5
    y4 -= 4.5
    x5 -= 5
    y5 -= 5

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



# Instatiate object classes
doob = doobie()
die = dice()
bass = character()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Display Background
    draw_bg_inf()

    die.update()
    doob.update()
    bass.update()
    
    # Draw Score
    score_text = font.render(f"Score: {score}", True, (255,255,255), (0,0,0))
    screen.blit(score_text, (10,10))
    
    keys = pygame.key.get_pressed()
    if keys[pygame.K_ESCAPE]:
        running = False

    # flip() the display to put your work on screen
    pygame.display.update()

    # limits FPS to 60
    # dt is delta time in seconds since last frame, used for framerate-
    # independent physics.
    dt = clock.tick(FPS) / 1000

pygame.quit()
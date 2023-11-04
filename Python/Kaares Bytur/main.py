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
jumpCount = 20
gravity = 10
gameSpeed = 1


# Game score handling
score = 0
font = pygame.font.Font(None,36)


player_pos = pygame.Vector2((width / 2, height*0.9))


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
        self.can_jump = True
        self.can_doubleJump = True
        self.gravityModifier = 1

    def update(self):
            global move_speed, jumpCount, gravity
            screen.blit(self.image, player_pos)
            keys = pygame.key.get_pressed()

            # Player Control
            if keys[pygame.K_a] and player_pos.x > 0:
                player_pos.x -= move_speed * dt
            if keys[pygame.K_d] and player_pos.x < width - bass.width:
                player_pos.x += move_speed * dt
            
            # Gravity
            if player_pos.y < height*0.85 and not self.can_jump:
                player_pos.y += gravity * self.gravityModifier
                self.gravityModifier += 0.05

            if player_pos.y >= height*0.85:
                self.gravityModifier = 1

           
   
class Character(pygame.sprite.Sprite):
    def __init__(self) -> None:
        pygame.sprite.Sprite.__init__(self)
        self.image = bass_tex
        self.image = pygame.transform.scale(self.image, (int(0.2 * self.image.get_width()), int(0.2 * self.image.get_height())))
        self.rect = self.image.get_rect()
        self.rect.center = (width // 2, height * 0.85)
        self.can_jump = True
        self.can_double_jump = True
        self.gravity = 1
        self.jump_strength = 15

    def update(self):
        global move_speed
        screen.blit(self.image, player_pos)
        keys = pygame.key.get_pressed()

        # Player Control
        if keys[pygame.K_a] and self.rect.left > 0:
            self.rect.x -= move_speed * dt
        if keys[pygame.K_d] and self.rect.right < width:
            self.rect.x += move_speed * dt

        # Jumping Mechanics
        if self.can_jump:
            if keys[pygame.K_SPACE]:
                self.jump()

        # Apply gravity
        if self.rect.y < height * 0.85:
            self.rect.y += self.gravity
            self.gravity += 0.5

        if self.rect.y >= height * 0.85:
            self.gravity = 1
            self.can_jump = True
            self.can_double_jump = True

    def jump(self):
        if self.can_jump:
            self.gravity = 0
            self.can_jump = False
            self.gravity -= self.jump_strength
        elif self.can_double_jump:
            self.gravity = 0
            self.can_double_jump = False
            self.gravity -= self.jump_strength


# Functions
# FIX CODE (IS UGLY AS)
def draw_bg_inf():
    global x1, x2, x3, x4, x5, y1, y2, y3, y4, y5, gameSpeed
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

    x1 -= 3 * gameSpeed
    y1 -= 3 * gameSpeed
    x2 -= 3.5 * gameSpeed
    y2 -= 3.5 * gameSpeed
    x3 -= 4 * gameSpeed
    y3 -= 4 * gameSpeed
    x4 -= 4.5 * gameSpeed
    y4 -= 4.5 * gameSpeed
    x5 -= 5 * gameSpeed
    y5 -= 5 * gameSpeed

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

all_sprites = pygame.sprite.Group()
all_sprites.add(doob)
all_sprites.add(die)
all_sprites.add(bass)


while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Display Background
    draw_bg_inf()

    # Update all objects
    all_sprites.update()

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
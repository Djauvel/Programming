# Rolling two six sided die
import random

#random.random(0,6)

n = 100000
samesies = 0
no_samesies = 0

for i in range(n):
    Dice1 = random.randrange(1,7)
    Dice2 = random.randrange(1,7)
    if Dice1 == Dice2:
        samesies += 1
    else:
        no_samesies += 1

print(f"The probability of two die rolling the same number is {samesies/n}")
print(f"The probability of two die NOT rolling the same number is {no_samesies/n}")

print(f"1/6 = {1/6} and 5/6 = {5/6}")
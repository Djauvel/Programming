import random as R

repeats = 1000
i = 0
head = 0
tail = 0

while i < repeats:
    num = R.randint(1,2)
    if num == 1:
        head += 1
    else:
        tail += 1
    i += 1

Phead = head/repeats
Ptail = tail/repeats

print(f"This experiment was repeated {repeats} times.")
print(f"The probability of head in this experiment is: {Phead}")
print(f"The Probability of tails is: {Ptail}")
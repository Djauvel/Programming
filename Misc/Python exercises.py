#Exercise 1: Given two integer numbers return their product only if the product is greater than 1000, else return their sum.
#Case 1:
#Number1 = 20
#Number2 = 30

#Case 2:
Number1 = 40
Number2 = 30

def Exercise1():
    if(Number1 * Number2 > 1000):
        return Number1 + Number2
    else:
        return Number1 * Number2

print(Exercise1())

#Exercise 2: Print the sum of the current number and the previous number
#Write a program to iterate the first 10 numbers and in each iteration, print the sum of the current and previous number:
numbers = [1,2,3,4,5,6,7,8,9,10]

for x in numbers:
    print("Previous Number: " , x-1 , "Current Number: ", x, "Sum: " ,2*x-1)
    if(x==10):
        break

#Exercise 3: Print characters from a string that are present at an even index number
#Write a program to accept a string from the user and display characters that are present at an even index number.

letterstring = input()
size = len(letterstring)
print("Original String is: " , letterstring)
print("Original String contains ", size, "letters")

for i in range(0,size,2):
    print("index[",i,"]",letterstring[i])



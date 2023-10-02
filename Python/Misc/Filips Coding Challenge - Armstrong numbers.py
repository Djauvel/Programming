#Filips Coding Challange
#An Armstrong number is a number that is the sum of its own digits each raised to the power of the number of digits.

#Take an input of a natural number
#Count the number of digits
#Raise individual numbers to the power of the count
#Compare output to input number
#Return True or false

def IsArmstrong(number):
    #Take an input from the user via console, integer variable will be a string
    integer = str(number)
    #Make a list of the individual digits in the user input number
    digits = [int(number) for number in integer]
    #Find the number of digits in the number for the power
    pow = len(digits)
    summ = 0
    #Loop through the numbers and raise them to the power of the length of the number. Find the sum of all iterations and compare to the input number
    for i in range(len(digits)):
        summ += digits[i]**pow

    if (summ == int(integer)):
        return True
        #print(f"{integer} is an Armstrong number")
    else:
        return False
        # print(f"{integer} is not an Armstrong number")

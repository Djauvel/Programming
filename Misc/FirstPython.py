#input

an_integer = 4
a_float = 2.5
smol_bicep = "gay christoffer"

#Output
print(smol_bicep)

#Functions
def addTwoNumbers(a,b):
    '''Adds two numbers. Smol PP'''
    return a + b

print(addTwoNumbers(an_integer, a_float))

#Classes
class pp:
    name = "cock"
    age = 99
    length = "big"
    weight = "huge"

    def returnPP():
        print("pp Stats:" + "pp length: " + pp.length + " PP mass: " + pp.weight)
        if (pp.age>100):
            print("pp is very old.")
        else:
            print("pp is still young (and hard)")

pp.returnPP()

#"self" entry implies that the function operates within itself, and does not set global values
class Catapult:
    def __init__(self,initial_tension):
        self.__tension = (initial_tension)
        pass

    def setTargetDistance(self, target_distance):
        pass
    def fire(self):
        pass
    def __setArmTension(self,tension):
        self.__tension = tension

#An object is an instance of a class
my_catapult = Catapult(2)

#If you make an object Doggo, and add tricks, the tricks will be changed for the class- 
#and all doggos will have those tricks
class Doggo:
    tricks = []

#We've distinguished the tricks to be personalized for each dog here.
class Dog:
    #Empty list of tricks
    def __init__(self):
        self.tricks = []

fido = Dog()
#Adding tricks - can only take one argument
fido.tricks.append("Roll over")

print(fido.tricks)

#Inherency
class Rectangle:
    def __init__(self, length, width):
        self.length = length
        self.width = width

    def area(self):
        return self.length * self.width

    def perimeter(self):
        return 2*self.length + 2*self.width

#Here a square is a rectangle, but a rectangle is not a square,
#So we reuse the parent (super) class by this:
class Square(Rectangle):
    def __init__(self, length):
        super().__init__(length, length)


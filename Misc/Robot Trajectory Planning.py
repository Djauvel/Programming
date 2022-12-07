import numpy as np
import matplotlib.pyplot as plt


print("Trajectory Planning: Choose type")
print("Cubic poly - no via : 1", " Cubic poly - w/ 1 Via point : 2", "Linear function w/ parabolic bend : 3")
typeChoice = int(input())

print("Input Theta0 (degrees):")
thetaZero = float(input())

print("Input ThetaF (degrees):")
thetaF = float(input())

#If its a cubic polynomial with no via points
if(typeChoice == 1):
    print("Input Time Constraint (seconds):")
    timeConstraint = int(input())
    #Calculate polynomial
    a1 = 0
    a2 = 3/(timeConstraint)**2 * (thetaF - thetaZero)
    a3 = -2/(timeConstraint)**3 * (thetaF - thetaZero)
    print("Cubic Polynomial: θ(t) = ", thetaZero, " + ", a1 , "t + ", a2 , "t^2 + ", a3 , "t^3 ")

    plt.style.use('_mpl-gallery')
    # make data
    t = np.linspace(0,thetaF,num=100)
    y = thetaZero + a1 * t + a2 * t**2 + a3 * t**3

    # plot
    fig, ax = plt.subplots()
#    fig, ac = plt.subplots()

#   p = np.poly1d([a3,a2,a1,thetaZero])
#    pp = np.polyder(p)

    ax.plot(t, y, linewidth=2.0)
#    ac.plot(t,pp,linewidth=2.0)

    ax.set(xlim=(0, timeConstraint), xticks=np.arange(0,timeConstraint),
       ylim=(thetaZero, thetaF), yticks=np.arange(0, thetaF))
    
#    ac.set(xlim=(0, timeConstraint), xticks=np.arange(0,timeConstraint),
#       ylim=(thetaZero, thetaF), yticks=np.arange(0, thetaF))

    plt.show()

#If its a cubic polynomial with a single via point
if(typeChoice == 2):
    print("Input start velocity (seconds): Input 0 if movement starts from standstill")
    startVel = float(input())
    print("Input Viapoint (degrees):")
    viaPoint = float(input())
    print("Input ViaVelocity (degrees/s):")
    viaVel = float(input())
    print("Input final velocity (degrees/s):")
    finalVel = float(input())
    print("Input first segment time constraint (seconds):")
    timeConstraint1 = float(input())
    print("Input second segment time constraint (seconds):")
    timeConstraint2 = float(input())
    #Calculate polynomial values for first poly
    a2 = 3/(timeConstraint1)**2 * (viaPoint - thetaZero) - 2/(timeConstraint1)*startVel - 1/(timeConstraint1)*viaVel
    a3 = -2/(timeConstraint1)**3 * (viaPoint - thetaZero) + 1/(timeConstraint1)**2 * (startVel + viaVel)
    #Calculate polynomial values for second poly
    b2 = 3/(timeConstraint2)**2 * (thetaF - viaPoint) - 2/(timeConstraint2)*viaVel - 1/(timeConstraint1)*finalVel
    b3 = -2/(timeConstraint2)**3 * (thetaF - viaPoint) + 1/(timeConstraint2)**2 * (finalVel + viaVel)
    print("Cubic Polynomial 1: θ(t) = ", thetaZero, " + ", startVel, "t + ", a2 , "t^2 + ", a3 , "t^3")
    print("Cubic Polynomial 2: θ(t) = ", viaPoint, " + ", viaVel , "t + ", b2 , "t^2 + ", b3 , "t^3" )

#If its a cubic polynomial with a single via point
if(typeChoice == 3):
    print()

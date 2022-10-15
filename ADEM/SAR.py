import numpy as np

print("Input current supply (V)")
Vcc = int(input())
print("Input signal (V)")
Vsignal = float(input())
print("Input number of bits:")
bits = int(input())

Smax = 2**bits
##The brute force way:
#for x in range(Smax):
#    Vin = ((Smax-x)/Smax)*Vcc
#    if Vin < Vsignal:
#        Vin1 = ((Smax-x+1)/Smax)*Vcc
#        diff1 = Vsignal - Vin
#        diff2 = Vsignal - Vin1
#        if diff1 > diff2:
#            print("The input signal represents: ", Smax-x)
#            break
#        else:
#            print("The input signal represents: ", Smax-x+1)
#            break

binNum = [0]*bits
#The "Real" way:
for bit in range(bits):
    binNum[bit] = 1
    binVal = int("".join(str(x) for x in binNum),2)

    if ((binVal/Smax)*Vcc) <= Vsignal:
        binNum[bit] = 1
    else:
        binNum[bit] = 0

print(binNum)
binVal = int("".join(str(x) for x in binNum),2)
voltageApprox = ((binVal/Smax)*Vcc)
print(voltageApprox)
print("Signal represents number: ", binVal)
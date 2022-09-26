print("Enter a number of seconds")
Number = int(input())

print("The result for ", Number, " seconds is:")
if(Number%60!=0):
    print(Number%60, " second(s)")

if((Number//60%60)!=0):
    print((Number//60)%60, " minute(s)")

if((Number//3600)%24!=0):
    print((Number//3600)%24, " hour(s)")

if((Number//86400)%7!=0):
    print((Number//86400)%7, " day(s)")

if(Number//86400*7!=0):
    print((Number//86400*7), " week(s)")

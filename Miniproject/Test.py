import string
uppercase = [x for x in string.ascii_uppercase]
lowercase = [x for x in string.ascii_lowercase]
def rot13(message):
    msglst = [str(x) for x in message]
    print(msglst)
    cipher = []
    for letter in msglst:
        if letter in uppercase:
            place = uppercase.index(letter)
            if place + 13 > 25:
                place = (place + 13) - 26
                cipher.append(uppercase[place])
            else:
                cipher.append(uppercase[place + 13])
                
        elif letter in lowercase:
            place = lowercase.index(letter)
            if place + 13 > 25:
                place = (place + 13) - 26
                cipher.append(lowercase[place])
            else:
                print(cipher)
                cipher.append(lowercase[place + 13])
        else:
            cipher.append(letter)
    return ''.join(cipher)

print(rot13("do i return original"))
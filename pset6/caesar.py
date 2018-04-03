# encryption program which takes an int (key) from the command line, prompts
# the user for a secret message (aka plaintext) and returns the ciphertext

import cs50
import sys

# get the key
sys.argv[1]

key = int(sys.argv[1])

# if 2 command line arguments are not provided,
# print an error and exit the program
if (len(sys.argv) != 2):
    print("Error: You must provide 2 command line arguments.")
    exit(1)

# prompt the user for the secret message (aka plaintext)
print("What is the secret message?")
plaintext = cs50.get_string()

print("plaintext: ", end="")
print(plaintext)

# print "ciphertext" followed by the actual ciphertext
# which is created using the remaining code
print("ciphertext: ", end="")

# encipher the plaintext:
# for each character in the plaintext string
# if it is alphabetic, shift character by the key
# while preserving the case: caps stay caps,
# lower stay lower, symbols remain unchanged 

for i in plaintext:
    
    # rotate alphabetical characters by key positions
    
    # check to see if the character is a letter 
    if i.isalpha():
        
        # check to see if the letter is uppercase
        # if so, rotate the ASCII char by 65 and print it as a string
        if i.isupper():
            print(chr(((ord(i) - 65 + key) % 26) + 65), end="")
    
        # else if the letter is lowercase,
        # if so, rotate the ASCII char by 97 and print it as a string
        elif i.islower():
            print(chr(((ord(i) - 97 + key) % 26) + 97), end="")
    
    else:
        print(i, end="")

# print a new row and exit the program, indicating success    
print()
exit(0)
    

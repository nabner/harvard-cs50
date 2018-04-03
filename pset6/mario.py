# This program takes user input, validates it,
# and then builds a pyramid out of hashes

import cs50

def main():

    # intiailize a variable to count the number of attempts the user
    # has to provide a valid input (i.e. user gets 3 attempts)
    attempt = 0

    # prompt user for input (pyramid height) and validate that the input
    # is valid (must be between greater than 0 and less than 23)
    while attempt < 3:
        print("How tall is the pyramid? ")
        height = cs50.get_int()

        # increment the attempt variable with each user input
        attempt += 1

        # if user has guessed 3 times and the height is still invalid
        # print an error and exit the program
        if (attempt == 3) and (height < 0 or height > 23):
            print("Error: Input must be between 0 and 23")
            exit(1)

        # if the user input is valid, draw the pyramid
        if (height >= 0 and height <= 23):
            break

    # initialize a variable to keep track of the number of hashes drawn
    # the pyramid always starts with 2 hashes at the top
    hashtag = 2

    # for each row, draw spaces (no new line) and then hashes (with a new line)
    # continue drawing until the number of hashes is equal to the user input (height)
    for i in range(height):
        print(" " * (height - hashtag + 1), end="")
        print("#" * hashtag)
        hashtag += 1

    # success
    exit(0)

# Call main to run the program
if __name__ == "__main__":
    main()



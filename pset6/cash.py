# this program takes a user input, dollar value,
# and returns the minimum number of coins that
# could be used to make up that dollar value

import cs50

# to eliminate the use of "magic numbers"
# intialize variables that represent the value
# of various coins
quarter = 25
dime = 10
nickle = 5
penny = 1

# intiailize a variable to count the number of attempts the user
# has to provide a valid input (i.e. user gets 3 attempts)
attempt = 0

while attempt < 3:

    # prompt the user for an amount of change
    print("O hai! How much change is owed? ")
    change = cs50.get_float()

    # increment the attempt variable with each user input
    attempt += 1

    # if user has guessed 3 times and the input is still invalid
    # print an error and exit the program
    if (attempt == 3) and (change < 0.00):
        print("Error: Input must be greater than 0.00")
        exit(1)

    if (change > 0.00):
        break
        
# convert value to cents
change = round(change * 100)

# initialize variable to keep track of the number of coins
coins = 0

# always use the largest coin possible
while change > 0:
    
    # if change >= the value of a quarter, give a quarter and reduce change by 25
    if change >= quarter:
        coins += 1
        change -= quarter
    
    # if change >= the value of a dime, give a dime and reduce change by 10
    elif change >= dime:
        coins += 1
        change -= dime
    
    # if change >= the value of a nickle, give a nickle and reduce change by 5
    elif change >= nickle:
        coins +=1 
        change -= nickle
        
    # if change >= the value of a penny, give a peny and reduce change by 1
    elif change >= penny:
        coins +=1 
        change -= penny

# print the final number of coins
print(coins)

# success
exit(0)
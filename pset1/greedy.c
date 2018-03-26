/* Greedy: prompts the user for the amount of change that is owed
and returns the minimum number of coins that add up to that amount */ 

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    
    float change;
    int minCoins = 0;
    
    // prompt the user for amount of change
    do {
        printf("O hai! How much change is owed? ");
        change = get_float();
        // printf("Let me get %f cents for you!\n", change); print function used to debug
    }
    //continue to prompt user while change value is invalid
    while (change < 0.00);
    
    change = round(change * 100);

    //loop through the various coins and give change until change = 0.00
    while (change != 0) 
    {
    
        //if change >= 25 give a quarter and reduce change by 25
        if (change >= 25) 
            {
            change = (change - 25);
            minCoins++;
            // printf("here's a quarter\n"); print function used to debug
            }
        
        //if change > 10 give a dime and reduce change by 10
        else if (change >= 10)
            {
            change = (change - 10);
            minCoins++;
            // printf("here's a dime\n"); print function used to debug
            }
        
        //if change >= 5 give a nickel and reduce change by 5
        else if (change >= 5)
            {
            change = (change - 5);
            minCoins++;
            // printf("here's a nickle\n"); print function used to debug
            }
       
       //if change >= 1 give a penny and reduce change by 1    
        else if (change >= 1)
            {
            change = (change - 1);
            minCoins++;
            // printf("here's a penny for your thoughts\n "); //print function used to debug
            }
        
        //break out of the loop when change = 0.00
        else
            {
            break;
            }
    }
    
    //output = minimum number of coins used for change
    printf("%d\n", minCoins);

}
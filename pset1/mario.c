/* Mario: an extremely simple program that took all of my 
confidence and in return, builds a hashtag pyramid based on user input */

#include <stdio.h>
#include <cs50.h>

int height;

int main(void)
{
    do
    {
        // user assigns value to height
        printf("How tall is the pyramid? "); 
        height = get_int();
    }
    // continue to prompt user input while height is invalid
    while (height < 0 || height > 23); 

    
    // initialize line at 1 and loop through height, printing a line for each int in height value   
    for (int line = 1; line <= height; line++)
    {
        
        // initiatlize the number of spaces to 0 and increase the number of spaces used each time
        for (int spaces = 0; spaces < (height - line); spaces++) 
        {
            putchar(' '); //print spaces
        }
        
        // initialize the hash value to zero and print a hash as long as more lines are needed
        for (int hash = 0; hash <= line; hash++)
        {
            putchar('#'); 
        }
        
        printf("\n");
    }
        
    return 0;    
        
}

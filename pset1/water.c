#include <stdio.h>
#include <cs50.h>


int main(void)
{
    printf("Minutes in shower: ");
    int minutes = get_int(); //user assigns value to minutes variable
    int bottles = (minutes * 12); //initialize bottles variable
   
    printf("Minutes: %i\n", minutes);
    printf("Bottles: %i\n", bottles);
}
 



#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
    ///DEBUGGER printf("What is your full name? \n");
    string name = get_string();
    
    ///always print the first char of the string in CAPS
    printf("%c", toupper(name[0]));
    
    /*iterate through the string and check if each char
    is a blank; if it is, print the next char in CAPS*/
    for (int i = 0; i < strlen(name); i++) 
    {
        if isblank(name[i])
        {
            printf ("%c", toupper(name[i+1]));
        }
    } 
    
    ///print a new line at the end of the program
    printf("\n");
}


/*encryption program which takes an int (key) from the command line, prompts
the user for a secret message (aka plaintext) and returns the ciphertext*/


#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


int main (int argc, string argv[])
{
    /*if user input is anything other than 2 arguments, print an error
    and return a 1 which indicates an issue and exits the program*/
    if (argc != 2) {
        printf("You must provide exactly 2 command line arguments -\
        and remember that the name of the program counts as an argument.\n");
        return 1;
    }
    
    //else if the user inputs exactly 2 command line arguments, run this encryption program
    else {
    
        //change argv from a string to an int (in order to perform mathmatical calculations per ASCII)
        int k = atoi(argv[1]);
        
        //prompt the user for the secret message (aka plaintext)
        printf("plaintext: ");
        string p = get_string();
        
        //print "ciphertext" followed by the actual ciphertext which is created using the remaining code
        printf("ciphertext: ");
        
        //iterate through each char in the plaintext
        for (int i = 0, n = strlen(p); i < n; i++) {
            
            //rotate alphabetical characters by k positions: caps stay caps,
            //lower stay lower, symbols remain unchanged
            if isalpha(p[i]) {
                
                //uppercase chars 
                if isupper(p[i]) {
                    p[i] -= 65; //go from ASCII to alphabetical
                    p[i] = (p[i] + k) % 26; //go from origial alphetical letter to new one using key
                    p[i] += 65; //now convert from alphabetical to ASCII
                    printf("%c", p[i]);
                }
                
                else if islower(p[i]) {
                    p[i] -= 97; //go from ASCII to alphabetical
                    p[i] = (p[i] + k) % 26; //go from origial alphetical letter to new one using key
                    p[i] += 97; //now convert from alphabetical to ASCII
                    printf("%c", p[i]);
                    
                }
                //printf("%c%i", p[i]);
            }
            
            //else if it is not an alphabetical character, it will remain unchanged
            else {
                printf("%c", p[i]);
            }
        }
       
       //print a new line at the end of the program and return a 1 to indicate success
       printf("\n");
       return 0; 
    }
}

/*encryption program which takes an alpha-key from the command line, prompts
the use for a secret message (aka plaintext) and returns the ciphertext*/


#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



int main (int argc, string argv[]) {


    /*if user input is anything other than 2 arguments, print an error
    and return a 1 which indicates an issue and exits the program*/
    if (argc != 2) {
        printf("Yo, provide exactly 2 command line arguments: one is the name of the program and one is a keyword.\n");
        return 1;
    }
    
    //change the key from an array element to a string called key
    string key = argv[1];
    int keyLength = strlen(key);
    
    //iterate through the user's key to make sure it is all alphabetical
    for (int j = 0; j < keyLength; j++) {
        
        //if the user input is not alphabetical, return 1 and exit program
        if (!isalpha(key[j])) {
            printf("Yo, provide exactly 2 command line arguments: one is the name of the program and one is a keyword.\n");
            return 1;
        }
    }
    
        //prompt the user for the secret message (aka plaintext)
        printf("plaintext: ");
        string p = get_string();
        
        //print "ciphertext" followed by the actual ciphertext which is created using the remaining code
        printf("ciphertext: ");
        
        int plainLength = strlen(p);
            
        //iterate through each char in the plaintext
        for (int i = 0, j = 0; i < plainLength; i++, j++) {
            
            //wrap around to beginning of key if j reaches key length
            if (j == keyLength) {
                j = 0;
            }
            
            //treatment of j if it is a capital letter
            if isupper(key[j]) {
                key[j] -= 'A'; //go from ASCII to alphabetical
            }
        
            //treatment of j if it is a capital letter
            else if islower(key[j]) {
                key[j] -= 'a'; //go from ASCII to alphabetical
            }
            
            //rotate alphabetical characters in the plaintext using the key
            //caps stay caps, lower stay lower; symbols remain unchanged
            if isalpha(p[i]) {
                
                //uppercase plaintext chars 
                if isupper(p[i]) {
                    p[i] -= 'A'; //go from ASCII to alphabetical 65 via 'A'
                    p[i] = (p[i] + key[j]) % 26; //go from origial alphetical letter to new one using key %26
                    p[i] += 'A'; //now convert from alphabetical to ASCII 65 via 'A'
                    printf("%c", p[i]);
                }
                
                //lowercase plaintext chars
                else if islower(p[i]) {
                    p[i] -= 'a'; //go from ASCII to alphabetical 97 via 'a'
                    p[i] = (p[i] + key[j]) % 26; //go from origial alphetical letter to new one using key
                    p[i] += 'a'; //now convert from alphabetical to ASCII 97 via 'a'
                    printf("%c", p[i]);
                }
                
            }
        
            //else if the plaintext is not an alphabetical character, it will remain unchanged and j will not increment
            else {
                j -= 1; 
                printf("%c", p[i]);
            }
            
        
        } //end of for loop iterating through plaintext 
   
        //print a new line at the end of the program and return a 0 to indicate success
        printf("\n");
        return 0;
    
} //end of main

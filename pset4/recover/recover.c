/**
 * This is a program called "Recover" that recovers JPEGs
 * that were recently deleted from a memory card file / forensic image.
 * 
 */ 

#include <stdio.h>
#include <stdlib.h>

// these JPEGs have a file system
// whose "block size" is 512 bytes (B)
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{

    // ensure correct number of command line argumets are given
    // if not, return 1 and quit the program
    if (argc != 2)
    {
        fprintf(stderr, "Please provide the name of the memory card file.\n");
        return 1;
    }
    
    // create a char pointer to hold the name 
    // and location of the memory card file,
    // which was given as argv[1]
    char *infile = argv[1];

    // create a FILE pointer to the location of argv[1]
    // open the memory card file in read mode
    FILE *inptr = fopen(infile, "r");
    
    // ensure that the infile pointer did not
    // lead to a blank memory space (i.e. NULL)
    // if infile pointer does point to NULL,
    // return 2 and quit the program
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // create an array called "buffer" to hold 
    // 512 bytes of the infile
    unsigned char buffer[BLOCK_SIZE];

    // create a new file to store the recovered JPEGs
    // initialize it to point to NULL
    FILE *img = NULL;
    
    // create a char for the name of the new files
    char newFile[8];

    // initialize a variable, fileNumber, to name each file
    // filenames should be ###.jpg starting at 000
    int fileNumber = 0;
 
    // run this loop until we reach the end of the file        
    while (!feof(inptr)) {
        
        // becuase the last block will be smaller than 
        // BLOCK_SIZE (512), create an integer, small_block, 
        // to hold the size of the data that is read; this value 
        // will always be 512 until near the end of file
        
        // read infile as 512 elements of size 1
        int small_block = fread(&buffer, 1, BLOCK_SIZE, inptr);
        
        // determine if we are at the start of a new JPEG
        // each JPEG starts with a distinct header
        // the first 3 bytes of any JPEG are 0xff 0xd8 0xff
        // the 4th byte of a JPEG is 0xe0, 0xe1, 0xe2, ...0xef
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0) 
            {
                
                // if true, we've found a lost JPEG!
                // now we need to make a new JPEG file


                // is this the first JPEG file we've found?
                // if img is no longer pointing to NULL, 
                // we have already found our first JPEG
                // and must close the previous file
                if (img != NULL) 
                    fclose(img);

                
                // sprintf sends formatted output to a string
                // %3i ensures that 3 chars are printed 
                // %03i adds leading 0s if there aren't 3 chars
                // filenames should be ###.jpg starting at 000
                sprintf(newFile, "%03i.jpg", fileNumber);
                
                // open the new JPEG in appedning (a) mode
                img = fopen(newFile, "a");
            
                // increment the file number by 1 each time
                fileNumber++;  
            
                // write 512 bytes until new JPEG is found
                // indicated by 3 bytes in a row being 0xff 0xd8 0xff 
                // and the fourth being 0xe0, 0xe1, 0xe2, ...0xef
                fwrite(&buffer, 1, small_block, img);
            
                
            } // end of checking for start of JPEG file
        
        // else, if we are already writing to a JPEG
        // and have not yet found another one, 
        // keep writing to the current file
        else if (fileNumber > 0)
            fwrite(&buffer, 1, small_block, img);           
        
        
    } // end of while loop that runs until end of file is reached

    // close any remaining files
    fclose(inptr);
    fclose(img);
    
    
    // success
    return 0;
    
} //end of main

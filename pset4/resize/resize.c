/**
 * Resizes a BMP file piece by piece.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // store argv[1] as "n" by which to resize the bmp file
    // define file pointers to house
    // the locations of argv[2] and [3]
    //char *factor = argv[1];
    char *infile = argv[2];
    char *outfile = argv[3];

    // store argv[1] as "n" by which to resize the bmp file
    int n = atoi(argv[1]);
    
    // n must be between 0 and 100, else return 1 with error statement
    if (n < 0 || n > 100) {
        fprintf(stderr, "n must be between 0 and 100\n");
        return 2;
    }  
    
    // create a FILE pointer to house
    // the location of the infile
    // then, open infile 
    FILE *inptr = fopen(infile, "r");
    
    // if upon opening the infile, it returns a NULL
    // value, quit the program and return 2
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // create a FILE pointer to house
    // the location of the outfile
    // then, open outfile
    FILE *outptr = fopen(outfile, "w");
    
    // if upon opening the outfile, it returns a NULL
    // value, close the infile, quit the program and return 3
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    // NEED COMMENT
    BITMAPFILEHEADER bf, bf_new;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    // NEED COMMENT
    BITMAPINFOHEADER bi, bi_new;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    
    // create new structs to hold resized picture data
    bf_new = bf;
    bi_new = bi;
    
    // new width and heigh
    bi_new.biWidth *= n;
    bi_new.biHeight *= n;
    
    // determine padding for scanlines
    // figure out how much padding you need with the new image size
    // save that value in a new variable, newPadding
    int original_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // change the size of the bitmap image
    bi_new.biSizeImage = ((sizeof(RGBTRIPLE) * bi_new.biWidth) + new_padding) * abs(bi_new.biHeight);
    
    // change the size of the bitmap file
    bf_new.bfSize = bi_new.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's new BITMAPFILEHEADER
    // &bf means "at the address of bf"
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's new BITMAPINFOHEADER
    // &bi means "at the address of bf"
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    /* used to debug
    printf("Width: %d, %d\n", bi.biWidth, bi_new.biWidth);
    printf("Height: %d, %d\n", bi.biHeight, bi_new.biHeight);
    printf("Size Image: %d, %d\n", bi.biSizeImage, bi_new.biSizeImage);
    printf("Size: %d, %d\n", bf.bfSize, bf_new.bfSize);
    printf("Padding: %d, %d\n", original_padding, new_padding);
    */ 

    // iterate over infile's scanlines (aka rows)
    for (int row = 0, biHeight = abs(bi_new.biHeight); row < biHeight; row++)
    {
        
        // for n times
        for (int j = 0; j < n; j++ )
        {
            
            // iterate over pixels in scanline
            for (int pixels = 0; pixels < bi.biWidth; pixels++)
            {
                
                // temporary storage
                RGBTRIPLE triple;
                
                // read RGB triple from infile
                // &triple means to write at the location of triple
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                // for n times, resize it vertically
                for (int l = 0; l < n; l++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            
                
            }
            
            // then add new padding to new file's scanlines (aka outfile)
            for (int i = 0; i < new_padding; i++)
            {
                fputc(0x00, outptr);
            }
            
            
            // COMMENT
            if (j < n - 1)
            {
                fseek(inptr, -(bi.biWidth * (int)sizeof(RGBTRIPLE)), SEEK_CUR);
            }
            
        } // end of j
        
        // continue to iterate over infile rows, 
        // skipping over any padding from infile
        fseek(inptr, original_padding, SEEK_CUR);
    
        
    } // end of iterating over infile's scanlines

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
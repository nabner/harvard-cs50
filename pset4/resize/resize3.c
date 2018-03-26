/**
 * Copies a BMP piece by piece, just because.
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
    char *factor = argv[1];
    char *infile = argv[2];
    char *outfile = argv[3];

    // convert the pointer char *n to an integer
    int number = atoi(factor);
    
    // n must be between 0 and 100, else return 1 with error statement
    if (number < 0 || number > 100) {
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
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
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
    // old width
    LONG biWidthOld = bi.biWidth;
    
    // new width and heigh
    bi.biWidth *= number;
    bi.biHeight *= number;
    
    // determine padding for scanlines
    // figure out how much padding you need with the new image size
    // save that value in a new variable, newPadding
    int paddingOld = (4 - (biWidthOld * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // change the size of the bitmap image
    bi.biSizeImage = ((3 * bi.biWidth) + padding) * abs(bi.biHeight);
    
    // change the size of the bitmap file
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's new BITMAPFILEHEADER
    // &bf means "at the address of bf"
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's new BITMAPINFOHEADER
    // &bi means "at the address of bf"
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    /* used to debug
    printf("Width: %d, %d\n", bi.biWidth, new_bi.biWidth);
    printf("Height: %d, %d\n", bi.biHeight, new_bi.biHeight);
    printf("Size Image: %d, %d\n", bi.biSizeImage, new_bi.biSizeImage);
    printf("Size: %d, %d\n", bf.bfSize, new_bf.bfSize);
    printf("Padding: %d, %d\n", originalPadding, newPadding);
    */ 

    // iterate over infile's scanlines (aka rows)
    for (int row = 0, biHeight = abs(bi.biHeight); row < biHeight; row++)
    {
        
        for (int j = 0; j < number; j++ )
        {
            // iterate over pixels in scanline
            for (int k = 0; k < biWidthOld; k++)
            {
                // temporary storage
                RGBTRIPLE triple;
                
                // read RGB triple from infile
                // &triple means to write at the location of triple
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                for (int l = 0; l < number; l++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // then add new padding to new file's scanlines (aka outfile)
            for (int m = 0; m < padding; m++)
            {
                fputc(0x00, outptr);
            }
            if (j < number - 1)
                fseek(inptr, -(biWidthOld * (int)sizeof(RGBTRIPLE)), SEEK_CUR);
            
        } 
        // skip over any infile padding
        fseek(inptr, paddingOld, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
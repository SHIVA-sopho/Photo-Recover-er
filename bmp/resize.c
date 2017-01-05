/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }
    
    int fact = atoi(argv[1]);
    
     // ensure proper usage
    if (fact < 1 || fact > 100  )
    {
        printf("Usage: ./copy infile outfile\n");
        return 5;
    }


    
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_new;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
   
    bf_new = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_new;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    bi_new = bi;
    
   

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    bi_new.biWidth = bi.biWidth * fact;
    bi_new.biHeight = bi.biHeight * fact;
   int Padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newPadding = (4 - (bi_new.biWidth * sizeof(RGBTRIPLE) ) % 4) % 4;
     
    bf_new.bfSize = 54 + bi_new.biWidth * abs(bi_new.biHeight) * sizeof(RGBTRIPLE) + newPadding * abs(bi_new.biHeight);
     
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);
  
  
    
   // bi.biSizeImage = fact * (bi.biWidth* sizeof(RGBTRIPLE) + newPadding)*fact  * abs(bi.biHeight);
    bi_new.biSizeImage = ((((bi_new.biWidth * bi_new.biBitCount) + 31) & ~31) / 8) * abs(bi_new.biHeight);
    
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        RGBTRIPLE arr[bi_new.biWidth];
        
        // iterate over pixels in scanline
        for (int j = 0, J = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for(int k = 0; k < fact ; k++)
            {
                arr[J] = triple;
                J++;
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
        }
         // skip over padding, if any
        fseek(inptr, Padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < newPadding; k++)
        {
            fputc(0x00, outptr);
        }
        
        for(int f = 1;f < fact; f++)
        {
       
            for(int i1 = 0 ; i1 < bi_new.biWidth; i1++)
                fwrite(&arr[i1], sizeof(RGBTRIPLE), 1, outptr);
         // skip over padding, if any
        
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
        
        }
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
 
 
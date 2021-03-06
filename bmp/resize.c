/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * resize a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // increment times
    int n = atoi(argv[1]);
    // remember filenames
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
    BITMAPFILEHEADER bf, bfn;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bin;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // coping whole data first
    bin = bi;
    bfn = bf;
    // changes being made info header
    bin.biWidth = bi.biWidth * n;
    
    // calucalting necceray things for a formula
    int pad1, pad2, multi, pad11, pad22;
    pad11 = (4 - (bi.biWidth ) % 4) % 4;
    pad22 = (4 - (bin.biWidth) % 4) % 4;
    multi = (n * (bin.biWidth + pad22)) / (bi.biWidth + pad11);
    pad1 = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    pad2 = (4 - (bin.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // changes being made info header and bfsize(biSizeImage+54)
    // bin.biSizeImage = multi*bi.biSizeImage;
    bin.biHeight = n * bi.biHeight;
    // bfn.bfSize = bin.biSizeImage + 54;
    bfn.bfSize = 54 + bin.biWidth * abs(bin.biHeight)
    * 3 + abs(bin.biHeight) * pad2;
    bin.biSizeImage = ((((bin.biWidth * bin.biBitCount) + 31) & ~31) / 8)
	* abs(bin.biHeight);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bfn, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bin, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bin.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
   
    
    // declaring a variable for c verticall coping thing
    int vert=1;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        vert = 1;
        LABEL:
        // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int arbi=0;arbi < n;arbi++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

        // skip over padding, if any
        fseek(inptr, pad1, SEEK_CUR);
        
        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
        if (vert < n)
        {
            fseek(inptr, -(bi.biWidth * 3 + pad1),SEEK_CUR);
            vert++;
            goto LABEL;
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

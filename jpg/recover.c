/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

int main()
{
    // opening the card file 
    FILE* inptr = fopen("card.raw","r");
    
    // checking if the file opened correctly
    if(inptr==NULL)
    {
        printf("couldn't open card.raw");
        return 1;
    }
    
    // jpg signatures are 8 bit integer
    uint8_t chkjpg1[4]={0xff,0xd8,0xff,0xe0};
    uint8_t chkjpg2[4]={0xff,0xd8,0xff,0xe1};
    
    //a counter to check jpg number
    int count=0;
    
    // initialise a pointer for output file
    FILE* outf;
    
    // buffer created
    uint8_t buf[512];
    
    char title[8];
    
    // start reading 512 bytes chunks 
    for(int i=0;;i++)
    {
        fread(buf,512,1,inptr);
        // if we read eof then close infile and outfile
        if(buf[0]==EOF)
        {
            fclose(outf);
            fclose(inptr);
            return 0;
        }
        
        // check if the read starting of new jpg
        if(memcmp(buf,chkjpg1,4)==0||memcmp(buf,chkjpg2,4)==0)
        {
            fprintf(title,"%03d.jpg",count);
            
            fopen(title,"w");
            fwrite(buf,512,1,outf);
            
        }
        else
        {
            fwrite(buf,512,1,outf);
        }
    }
    
    return 0;
}

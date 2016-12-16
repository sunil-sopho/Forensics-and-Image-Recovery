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
    FILE* outf=NULL;
    
    // buffer created
    uint8_t buf[512];
    
    //char title[8]; it should be iitaialise some where else
    
    // start reading 512 bytes chunks 
    for(int i=0;fread(buf,512,1,inptr)>0;i++)
    {
        //fread(buf,512,1,inptr);
        // if we read eof then close infile and outfile
        // if((char)buf[0]==EOF)
        /*{
            fclose(outf);
            fclose(inptr);
            return 0;
        }*/
        
        // check if the read starting of new jpg
        if(memcmp(buf,chkjpg1,4)==0||memcmp(buf,chkjpg2,4)==0)
        {
            char title[8];
            sprintf(title, "%03d.jpg",count);
            
            // if a file is already opened then close that and open a new file esle open a file
            if(outf)
            {
                fclose(outf);
                count++;
                outf = fopen(title,"w");
                fwrite(buf,512,1,outf);
            }
            else
            {
                outf=fopen(title,"w");
                fwrite(buf,512,1,outf);
            }
        }
        else
        {
            if(outf)
            fwrite(buf,512,1,outf);
        }
    }
    // closeing all open files 
    if(outf)
    {
        fclose(outf);
    }
    if(inptr)
    {
        fclose(inptr);
    }
    return 0;
}
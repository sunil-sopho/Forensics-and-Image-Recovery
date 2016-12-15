/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

int main(int argc, char* argv[])
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
    unit8_t chkjpg2[4]={0xff,0xd8,0xff,0xe01};
    
    //a counter to check jpg number
    int count=0;
    
    // initialise a pointer for output file
    FILE* outf;
    
    
    
    
}

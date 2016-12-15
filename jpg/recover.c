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
    
    
}

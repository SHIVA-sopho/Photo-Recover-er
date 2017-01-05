/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
# include<stdio.h>
# include<math.h>
# include<stdlib.h>

int main(int argc , char* argv[])
{
    //printf("program started\n");
    argv[0] = argv[0];
    int jpeg = 0;
    int hundred = 0;
    int tens =0;
    int unit =0;
   // char* format = ".jpg";
    char filename[8] ;
    
    if(argc != 1)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }
    char* InFile = "card.raw";
    //printf("card.raw in file created\n");
    FILE* inptr = fopen(InFile , "r" );
    FILE* outptr = NULL;
    if(inptr == NULL)
    {
        printf("Usage: ./copy infile outfile\n");
        fclose(inptr);
        return 2;
    }

      unsigned char bytes[512];

    while(fread(&bytes,512,1,inptr) )
    {
        
        if(bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && ((bytes[3] & 0xe0) == 0xe0))
        {   
            if(jpeg == 0)
            {
                sprintf(filename,"00%d.jpg",(jpeg));
                outptr = fopen(filename,"a");
                if(outptr == NULL)
                {
                    printf("Usage: ./copy infile outfile\n");
                    fclose(inptr);
                    fclose(outptr);
                   return 3;
                }
                jpeg++;
            }
            else
            {
                unit = jpeg % 10;
                tens = (jpeg/10) % 10;
                hundred = (jpeg/100) % 10;
                fclose(outptr);
                sprintf(filename,"%d%d%d.jpg",hundred,tens,(unit));
                outptr = fopen(filename,"a");
                if(outptr == NULL)
                {
                    printf("Usage: ./copy infile outfile\n");
                    fclose(inptr);
                    fclose(outptr);
                   return 3;
                }
                jpeg++;
            }
        }
        if(jpeg > 0)
        {
        fwrite(&bytes,512,1,outptr);
        }
        
    }
    fclose(inptr);
    fclose(outptr);
    return 0;
}

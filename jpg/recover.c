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

    //int number=1;
    while(fread(&bytes,512,1,inptr) )
    {
        if(bytes == NULL)
        {
            break;
        } 
        //printf("entered while \n");
        if(bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && ((bytes[3] & 0xe0) == 0xe0))
        {   
            //printf("got the star \n");
            if(jpeg == 0)
            {
               // printf("first star \n");
                sprintf(filename,"00%d.jpg",(jpeg));
                //printf("sprintf worked  \n");
                outptr = fopen(filename,"a");
                //printf("outptr file opened \n");
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
                //printf("next star \n");
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
        //printf("writeen in outptr \n");
        fwrite(&bytes,512,1,outptr);
        }
        /*else
        printf(" not written in file \n");*/
        
    }
    //printf("reached end \n");
    //free(bytes);
    fclose(inptr);
    fclose(outptr);
    return 0;
}

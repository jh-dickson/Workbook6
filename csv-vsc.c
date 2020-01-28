#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void seekWord(FILE *fpIN, int size, FILE *fpOUT, int offset)
{
    if (offset >= size)
    {
        return;
    }
    
    //initialise a "dynamic" (ish) array
    char *wordtoprint;
    wordtoprint = malloc(25 * sizeof(char));

    //seek back until we find a comma, adding each char to the array
    int i;
    if (offset = 0)
    {
        i = 1;
    }
    else
    {
        i = offset + 1;
    }
    

    char tmpchar;
    while (tmpchar != ',' && i > -size)
    {
        fseek(fpIN, -i, SEEK_END);
        fscanf(fpIN, "%c", &tmpchar);
        wordtoprint[-1-i] = tmpchar;
        i--;
    }

    //print it out to the file
    for (int j = (sizeof(char)*sizeof(wordtoprint)); j > 0; j--)
    {
        if (wordtoprint[j] == ',')
        {
            //ignore
        }
        else
        {
            printf("%c", wordtoprint[j]);
            fprintf(fpOUT, "%c", wordtoprint[j]);
        }
    }
    fprintf(fpOUT, ",");

    //recurse, passing the increased offset
    offset += (sizeof(char)*sizeof(wordtoprint));
    seekWord(fpIN, size, fpOUT, offset);
}

int main(int argc, char *argv[])
{
    //parse command line switches -i : ignore first line
    char *filename;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            /* TODO */
        }
        else
        {
            //assume this is the filename
            filename = malloc(sizeof(argv[i]));
            strcpy(filename, argv[i]);
            printf("Filename to open: %s\n", filename);
        }
    }

    //load file pointer and seek to end
    FILE *fpIN = fopen(filename, "r");
    fseek(fpIN, 0, SEEK_END);
    int size = ftell(fpIN);

    //work out if it has .csv at end - this isn't an elegent solution!
    char *outfilename;
    int szFileName = sizeof(filename);
    if (filename[szFileName-4]=='.' && filename[szFileName-3]=='c' && filename[szFileName-2]=='s' && filename[szFileName-1]=='v')
    {
        outfilename = malloc(sizeof(filename) * sizeof(char) + 8);
        strcpy(outfilename, filename);
        outfilename[szFileName-4] = '_';
        outfilename[szFileName-3] = 'r';
        outfilename[szFileName-2] = 'e';
        outfilename[szFileName-1] = 'v';
        strcat(outfilename, "erse.csv");
        printf("Output file: %s\n", outfilename);
    }
    else
    {
        outfilename = malloc(sizeof(filename) * sizeof(char) + 8);
        strcpy(outfilename, filename);
        strcat(outfilename, "_reverse.csv");
        printf("Output file: %s\n", outfilename);
    }
    
    FILE *fpOUT = fopen(outfilename, "w");
    seekWord(fpIN, size, fpOUT, 0);
    
}
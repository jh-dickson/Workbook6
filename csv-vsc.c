#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int offset = 0;

int seekWord(FILE *fpIN, long int size, FILE *fpOUT)
{
    if (offset >= size)
    {
        return -1;
    }
    
    //initialise a "dynamic" (ish) array
    char *wordtoprint;
    wordtoprint = calloc(10,  sizeof(char *));

    //seek back until we find a comma, adding each char to the array
    int i = 0;
    char tmpchar = ' ';
    while (tmpchar != ',' && offset <= size)
    {
        fseek(fpIN, -offset, SEEK_END);
        tmpchar = getc(fpIN);
        wordtoprint[i] = tmpchar;
        i++;
        offset++;
        //allocate more memory to the array for the next char
        wordtoprint = realloc(wordtoprint, i*sizeof(char *));
    }

    //reverse the string and print to the output file
    int wordlen = strlen(wordtoprint);
    for (int j = wordlen; j >= 0; j--)
    {
        //filter out commas
        if (wordtoprint[j] != ',')
        {
            fputc(wordtoprint[j], fpOUT);
        }
    }
    fputc(',', fpOUT);

    wordtoprint = NULL;
    free(wordtoprint);
    return 0;
}

int main(int argc, char *argv[])
{
    char *filename;
    int ignoreFirstLine = 0;
    int showDebugOutput = 0;
    long int size;

    //parse command line switches -i : ignore first line, -d : show debug output
    if (argc == 1)
    {
        printf("Usage: [executable name] [file name] [options]\n");
        printf("Options: -i ignore first line, -d show debug output\n");
        exit(-1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            /* TODO */
            printf("Ignoring first line\n");
            ignoreFirstLine = 1;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            printf("Showing debug output\n");
            showDebugOutput = 1;
        }
        else
        {
            //assume this is the filename
            filename = calloc(strlen(argv[i]), sizeof(char));
            strcpy(filename, argv[i]);
            printf("Filename to open: %s\n", filename);
        }
    }

    //load file pointer and seek to end
    FILE *fpIN = fopen(filename, "r");
    if (ignoreFirstLine == 1)
    {   
        //count until the first newline char
        char tmpchar = ' ';
        int count = 0;
        while (tmpchar != '\n')
        {
            tmpchar = getc(fpIN);
            count++;
        }
        
        fseek(fpIN, 0, SEEK_END);
        size = ftell(fpIN) - count;
    }
    else
    {
        fseek(fpIN, 0, SEEK_END);
        size = ftell(fpIN);
    }

    //work out if it has .csv at end then rename - this isn't an elegent solution!
    char *outfilename;
    int szFileName = strlen(filename);
    if (strstr(filename, ".csv"))
    {
        outfilename = calloc(szFileName + 9, sizeof(char));
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
        outfilename = calloc(szFileName + 8, sizeof(char));
        strcpy(outfilename, filename);
        strcat(outfilename, "_reverse.csv");
        printf("Output file: %s\n", outfilename);
    }
    
    //loop until all of the words have been copied and reversed
    FILE *fpOUT = fopen(outfilename, "w");
    while (offset <= size )
    {
        if (showDebugOutput == 1)
        {
            printf("\rFile size: %ld Reading at offset: %ld", size, offset);
            fflush(stdout);
        }
        seekWord(fpIN, size, fpOUT);
    }
    printf("\nReversing Complete\n");
    fclose(fpIN);
    fclose(fpOUT);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void seekWord(FILE *fpIN, long double size, FILE *fpOUT, long double offset)
{
    if (offset >= size)
    {
        return;
    }
    
    //initialise a "dynamic" (ish) array
    char *wordtoprint;
    wordtoprint = calloc(10, sizeof(char));

    //seek back until we find a comma, adding each char to the array
    int i = 0;
    char tmpchar;
    while (tmpchar != ',' && offset <= size)
    {
        fseek(fpIN, -offset, SEEK_END);
        fscanf(fpIN, "%c", &tmpchar);
        wordtoprint[i] = tmpchar;
        i++;
        offset++;

        //check if memory is getting tight
        if (i % 11 == 0 && i != 0)
        {
            wordtoprint = realloc(wordtoprint, (2*i)*sizeof(char));
        }
    }

    //reverse the string and print to the output file
    for (int j = strlen(wordtoprint); j >= 0; j--)
    {
        //filter out unwanted outputs
        if (wordtoprint[j] != ',' && wordtoprint[j] != '\n')
        {
            fprintf(fpOUT, "%c", wordtoprint[j]);
        }
    }
    fprintf(fpOUT, ",");

    free(wordtoprint);
    //recurse, passing the increased offset
    seekWord(fpIN, size, fpOUT, offset);
}

int main(int argc, char *argv[])
{
    char *filename;
    int ignoreFirstLine;
    long double size;

    //parse command line switches -i : ignore first line
    if (argc == 1)
    {
        printf("Usage: [executable name] [file name] [options]\n");
        printf("Options: -i ignore first line\n");
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
        else
        {
            //assume this is the filename
            filename = malloc(strlen(argv[i]));
            strcpy(filename, argv[i]);
            printf("Filename to open: %s\n", filename);
        }
    }

    //load file pointer and seek to end
    FILE *fpIN = fopen(filename, "r");
    if (ignoreFirstLine == 1)
    {   
        //count until the first newline char
        char tmpchar;
        int count = 0;
        while (tmpchar != '\n')
        {
            fscanf(fpIN, "%c", &tmpchar);
            count++;
            fseek(fpIN, count, SEEK_SET);
        }
        fseek(fpIN, 0, SEEK_END);
        size = ftell(fpIN) - count;
    }
    else
    {
        fseek(fpIN, 0, SEEK_END);
        size = ftell(fpIN);
    }
    printf("Filesize: %.0Lf chars\n", size);

    //work out if it has .csv at end then rename - this isn't an elegent solution!
    char *outfilename;
    int szFileName = strlen(filename);
    if (strstr(filename, ".csv"))
    {
        outfilename = malloc(szFileName + 9);
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
        outfilename = malloc(szFileName + 8);
        strcpy(outfilename, filename);
        strcat(outfilename, "_reverse.csv");
        printf("Output file: %s\n", outfilename);
    }
    
    FILE *fpOUT = fopen(outfilename, "w");
    seekWord(fpIN, size, fpOUT, 0);

}
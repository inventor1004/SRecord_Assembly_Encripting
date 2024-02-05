/*
* Filename      : main.c
* Project       : Assignment 2
* By            : Jongeon Lee & Honggyu Park
* Student number: 8790144 & 8857884
* Date          : Feb 4, 2024
* Description   :
*/
#include "fileIO.h"
#include "SRecordEncode.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// #include "../inc/encodeToAssembly.h"
// #include "../inc/encodeToSRecord.h"
#pragma warning(disable: 4996)


#define MAX_FILENAME_LENGTH 256
#define MODE_ENCODE_ASM false
#define MODE_ENCODE_SREC true



void encodeToAssembly(char** fileContents, char** outputFileName); // intput 원본 /output 최종 적용한 이름
char* fileExtensionExtract(char* fileName, bool mode);
void printUsage();


int main(int argc, char* argv[])
{
    char inputFileName[MAX_FILENAME_LENGTH] = "\0";
    char outputFileName[MAX_FILENAME_LENGTH] = "\0";

    char* option_i = "-i";
    char* option_h = "-h";
    char* option_o = "-o";
    char* option_srec = "-srec";
    bool mode = MODE_ENCODE_ASM;
    bool is_Usage = false;


    if (argc <= 5) {
        for (int i = 1; i <= argc - 1; i++) {
            if (strncmp(argv[i], option_i, strlen(option_i)) == 0) {
                strcpy(inputFileName, argv[i]);
            }
            else if (strncmp(argv[i], option_h, strlen(option_h)) == 0) {
                is_Usage = true;
            }
            else if (strncmp(argv[i], option_o, strlen(option_o)) == 0) {
                strcpy(outputFileName, argv[i]);
            }
            else if (strncmp(argv[i], option_srec, strlen(option_srec)) == 0) {
                mode = MODE_ENCODE_SREC;
            }
        }
        strcpy(inputFileName, inputFileName + 2);
        strcpy(outputFileName, outputFileName + 2);

        // If the user does not enter the input file name
        // Ask by stdin
        // If output file name also not entered,
        // Ask by stdin
        if (inputFileName[0] == '\0') {
            printf("Please enter input file name: ");
            fgets(inputFileName, sizeof(inputFileName), stdin);

            if (outputFileName[0] == '\0') {
                printf("Please enter output file name: ");
                fgets(outputFileName, sizeof(outputFileName), stdin);
            }
        }

        // If the user enter the input file name, but not enter the output file name,
        // copy the entered input file name
        // It will be processed later
        if (outputFileName[0] == '\0') {
            strcpy(outputFileName, inputFileName);
        }
    }
    else {
        printf("Error - Invalid option.");
        return -1;
    }


    char *extendedOutputFileName = fileExtensionExtract(outputFileName, mode);
    if(extendedOutputFileName == NULL)
    {
        return -3;
    }

    // Read the input file and store the contents to the fileContents
    // readFile() will return a dynamic allocated
    char *fileContents = readFile(inputFileName);
    if(fileContents == NULL)
    {
        return -4;
    }


    if (is_Usage == true) {
        printUsage();
    }

    if(mode)
    {
        encodeToSRecord(&fileContents, &extendedOutputFileName);
    }
    else
    {
        encodeToAssembly(&fileContents, &extendedOutputFileName);
    }

    // Free allocated memory
    free(extendedOutputFileName);
    free(fileContents);
    return 0;
}

void encodeToAssembly(char** fileContents, char** outputFileName)
{
    // file open and close 만들어야함.

    char *inputdata = *fileContents; // intput example
    char outputdata[256] = "";

    strcat(outputdata, "dc.b\t");

    for(int i = 0; inputdata[i] != '\0'; i++) {
        char hexStr[6]="";
        sprintf(hexStr, "$%02X", inputdata[i]);

        if (i % 16 == 15 || inputdata[i+1] == '\0') {
            strcat(outputdata, hexStr);
            if (inputdata[i+1] != '\0') {
                strcat(outputdata, "\ndc.b\t");
            }
        }
        else {
            strcat(hexStr, ", ");
            strcat(outputdata, hexStr);
        }
    }

    printf("%s\n", outputdata); // final value is outputdata

}


char* fileExtensionExtract(char* fileName, bool mode)
{
    // need to handle empty output fileName
    // if there is no output fileName, we need to make a valid function that it is null or not.
    // if we want to handle that in this function, we add more one parameter for copy input name to output name.
    char extension[6] = "";
    char *dot = strrchr(fileName, '.');

    if (mode) {
        strcpy(extension,".srec");
    } else {
        strcpy(extension,".asm");
    }

    if (dot != NULL) {
        if (strcmp(dot, ".asm") == 0 || strcmp(dot, ".srec") == 0 ) {
            if (strcmp(dot, ".asm") == 0 && mode == MODE_ENCODE_SREC) {
                strcat(fileName, extension);
            }
            else if (strcmp(dot, ".srec") == 0 && mode == MODE_ENCODE_ASM) {
                strcat(fileName, extension);
            }
        }
        else {
            strcat(fileName, extension);
        }
    }
    else {
        strcat(fileName, extension);
    }

    unsigned int fileNameLen = strlen(fileName);
    char *outputFileName = (char *)calloc(fileNameLen + 1, sizeof(char)); // +1 for null termination
    strcat(outputFileName, fileName);
    if (outputFileName == NULL) {
        perror("Memory allocation failed");
        free(outputFileName);
        return NULL;
    }

    return outputFileName;
}

void printUsage() {
    printf("\n");
    printf("Usage: encodeInput [-i INPUTFILENAME] [-o OUTPUTFILENAME] [-srec] [-h]\n\n");
    printf("Description:\n");
    printf("  Convert binary input data into ASCII readable output based on the specified options.\n\n");
    printf("Options:\n");
    printf("  -i INPUTFILENAME      Specify the input binary file.\n");
    printf("                        If not specified, input will be read from standard input.\n\n");
    printf("  -o OUTPUTFILENAME     Specify the output file.\n");
    printf("                        If not specified and an input file is provided:\n");
    printf("                          - If -srec option is not used, output file will be named INPUTFILENAME.asm.\n");
    printf("                          - If -srec option is used, output file will be named INPUTFILENAME.srec.\n");
    printf("                        If not specified and no input file is given, output will be directed to standard output.\n\n");
    printf("  -srec                 Output data in S-Record format.\n");
    printf("                        If not specified, output will be in Assembly File format.\n\n");
    printf("  -h                    Display this help message and exit.\n\n");
}
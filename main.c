/*
* Filename      : main.c
* Project       : Assignment 2
* By            : Jongeon Lee & Honggyu Park
* Student number: 8790144 & 8857884
* Date          : Feb 4, 2024
* Description   :
*/

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



void encodeToAssembly(char inputFileName, char outputFileName); // intput 원본 /output 최종 적용한 이름
void encodeToSRecord(char inputFileName, char outputFileName);
void printUsage();
char* readFile(char* inputFileName);

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

    // todo fileextensionextract (.asm.srec)- jongeon
    // make output final filename



    // todo intput data malloc - honggyu
    char *fileContents = readFile(inputFileName);
    if(fileContents == NULL)
    {
        return -3;
    }
//    printf("intput: %s\n", inputFileName);
//    printf("output: %s\n", outputFileName);
//    printf("mode: %d\n", mode); // 1 is true, 0 is false
//    printf("is_Usage: %d\n", is_Usage);




    if (is_Usage == true) {
        printUsage();
    }

    switch (mode)
    {
        case MODE_ENCODE_SREC:
//        encodeToSRecord(input, output);
            break;
        case MODE_ENCODE_ASM:
//       encodeToAssembly(input, output);
            break;
        default:
            printf("Error - Invalid option.");
            return -2;
    }

    free(fileContents);
    return 0;
}



void encodeToAssembly(char inputFileName, char outputFileName) {
    // output file write - jongeon

}
void encodeToSRecord(char inputFileName, char outputFileName) {
    // output file write -honggyu
}

char* readFile(char* inputFileName)
{
    FILE *file;
    char *fileContents;
    long file_size;

    // open the input file
    file = fopen(inputFileName, "rb"); // "rb": read binary file
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // check the file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // Allocate the memory for storing the file contents
    fileContents = (char *)calloc(file_size + 1, sizeof(char)); // +1 for null termination
    if (fileContents == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file
    fread(fileContents, sizeof(char), file_size, file);
    fileContents[file_size] = '\0'; // Add null termination at the end


    printf("File content:\n%s\n", fileContents);

    // close the file
    if (fclose(file) == EOF) {
        perror("Failed to close file");
        exit(EXIT_FAILURE);
        free(fileContents);
    }

    return fileContents;
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
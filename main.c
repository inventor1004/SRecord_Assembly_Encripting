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
#include <stdlib.h>
#include <stdbool.h>

// #include "../inc/encodeToAssembly.h"
// #include "../inc/encodeToSRecord.h"
#pragma warning(disable: 4996)


#define MAX_FILENAME_LENGTH 256
#define MODE_ENCODE_ASM false
#define MODE_ENCODE_SREC true



void encodeToAssembly(char inputFileName, char outputFileName); // intput 원본 /output 최종 적용한 이름
void encodeToSRecord(char inputFileName, char outputFileName);
void printUsage();


int main(int argc, char* argv[])
{
    char inputFileName[MAX_FILENAME_LENGTH] = "";
    char outputFileName[MAX_FILENAME_LENGTH] = "";

    char* option_i = "-i";
    char* option_h = "-h";
    char* option_o = "-o";
    char* option_srec = "-srec";
    bool mode = MODE_ENCODE_ASM;
    bool is_Usage = false;


    if (1 < argc || argc <= 5) {
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

        if (inputFileName == NULL) {
            // todo add stdin function - honggyu
        }

        if (outputFileName == NULL) {
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
    // make malloc based on input file data


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

    return 0;
}



void encodeToAssembly(char inputFileName, char outputFileName) {
    // output file write - jongeon

}
void encodeToSRecord(char inputFileName, char outputFileName) {
    // output file write -honggyu
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
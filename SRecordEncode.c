//
// Created by chris on 2/4/24.
//

#include "SRecordEncode.h"
#include "fileIO.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define AUTHOR_NAME "JONGEON-HONGGYU"

#define MAX_SREC_LEN 42
#define MAX_BYTES    16
#define SIZE_OF_CHECKSUM 1
#define SIZE_OF_ADDRESS  2

unsigned char calculateChecksum(int count, char* address, char* data)
{
    int sum = 0;
    sum += (char)count;
    sum += strtol(address, NULL, 16);

    for (int i = 0; i < strlen(data); i += 2) {
        char ASCII[3] = "";
        ASCII[0] = data[i];
        ASCII[1] = data[i+1];
        ASCII[2] = '\0';

        sum += (int)strtol(ASCII, NULL, MAX_BYTES);
    }

    int onesComplement = ~sum;
    int lastTwoDigits = onesComplement & 0xFF;

    return (char)lastTwoDigits;
}

char* decimalToHexadecimal(int decimal) {
    // 2자리 16진수 + 널 종료 문자를 위한 공간 할당
    char *hexadecimal = calloc(3, sizeof(char));
    if (hexadecimal == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    // decimal 값을 2자리 16진수로 변환하여 저장
    sprintf(hexadecimal, "%02X", decimal);

    return hexadecimal;
}


char* stringToASCII(char *string)
{
    char *ASCII = calloc(strlen(string) * 2 + 1, sizeof(char)); // ASCII needs double size of memory
    if (ASCII == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < strlen(string); i++) {
        // Transfe char to ASCII and store to the ASCII
        sprintf(ASCII + i * 2, "%02X", (unsigned char)string[i]);
    }

    return ASCII;
}



void encodeToSRecord(char** fileContents, char** outputFileName)
{
    // calculate how many s1 should be generated
    int contentsBytes = strlen(*fileContents) / 2;
    int numberOfS1 = 0;
    int S1TotalSize = 0;
    if (contentsBytes % MAX_BYTES != 0) {
        numberOfS1 = contentsBytes / MAX_BYTES + 1;

    } else {
        numberOfS1 = contentsBytes / MAX_BYTES;
    }

    // Calculate total size of S1
    S1TotalSize = numberOfS1 * (SIZE_OF_ADDRESS + MAX_BYTES + SIZE_OF_CHECKSUM);

    // Prepare s-records
    char S0[MAX_SREC_LEN] = "\0";
    char *S1= calloc(S1TotalSize + numberOfS1, sizeof(char));
    if (S1 == NULL) {
        perror("Memory allocation failed");
        return;
    }
    char S5[MAX_SREC_LEN] = "\0";
    char S9[MAX_SREC_LEN] = "\0";
    char memoryAddress[SIZE_OF_ADDRESS*2 + 1] = {'0', '0', '0', '0'};


    /*----------------------------------------------------------------------*/
    /*-- Process s0 --------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    strcat(S0, "S0"); // add "s0"

    // Calculate file length and change to hexadecimal
    int byteLength = strlen(AUTHOR_NAME) + SIZE_OF_CHECKSUM + SIZE_OF_ADDRESS;
    char *hexadecimal = decimalToHexadecimal(byteLength);
    if(hexadecimal != NULL)
    {
        strcat(S0, hexadecimal); // add data size in hexadecimal
        free(hexadecimal);
    }

    // Accumulate the memory address
    strcat(S0, memoryAddress);

    // Tranfer string to ASCII code
    char *ASCII = stringToASCII(AUTHOR_NAME);
    if(ASCII != NULL)
    {
        strcat(S0, ASCII); // add ASCII code record

        // Calculate & Accumulate Check Sum
        unsigned char checkSum = calculateChecksum(byteLength, memoryAddress, AUTHOR_NAME);
        char checksumStr[3]; // CheckSum = 2 digits + "/0"
        sprintf(checksumStr, "%02X", checkSum);
        strcat(S0, checksumStr);
        strcat(S0, "\n");

        free(ASCII);
    }


    /*----------------------------------------------------------------------*/
    /*-- Process s1 --------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    int sizeCount = strlen(*fileContents);
    int indexCount = 0;
    int i = 0;
    bool newLine = false;
    while (sizeCount > 0)
    {
        // Accumulate "S1"
        strcat(S1, "S1");

        unsigned int address;
        // Check whether the data size is larger than 16 bytes, and
        // if so, truncate it to 16 bytes and save it.

        // Prepare to update the memory address
        // Convert address in string to integer
        sscanf(memoryAddress, "%x", &address);

        if(sizeCount > MAX_BYTES) {
            byteLength = MAX_BYTES + (SIZE_OF_CHECKSUM + SIZE_OF_ADDRESS);
            // Add 16 bytes
            address += MAX_BYTES;
        }
        else {
            byteLength = sizeCount + (SIZE_OF_CHECKSUM + SIZE_OF_ADDRESS);
            address += sizeCount;
        }

        // Accumulate LL code to S1
        char *hexadecimal = decimalToHexadecimal(byteLength);
        if(hexadecimal != NULL)
        {
            strcat(S1, hexadecimal); // add data size in hexadecimal
            free(hexadecimal);
        }

        // Accumulate & Update memory address
        strcat(S1, memoryAddress);

        char *inputdata = *fileContents;
        char tmp[MAX_BYTES+1] = "\0";
        int j = 0;
        while(i <= strlen(*fileContents))
        {
            char hexStr[3]="";
            hexStr[0] = inputdata[i];
            hexStr[1] = inputdata[i+1];
            hexStr[2] = '\0';


            if ( ( i % (MAX_BYTES*2) == 0) && i != 0 && newLine == false)
            {
                newLine = true;
                tmp[j] = '\0';
                break;
            }

            strcat(S1, hexStr);
            tmp[j] = inputdata[i];
            tmp[j+1] = inputdata[i+1];

            i += 2;
            j += 2;
            newLine = false;
        }
        // Calculate & Accumulate Check Sum
        if (sizeCount < MAX_BYTES*2)
        {
            byteLength = SIZE_OF_ADDRESS + sizeCount/2 + SIZE_OF_CHECKSUM;
        }
        unsigned char checkSum = calculateChecksum(byteLength, memoryAddress, tmp);
        char checksumStr[3]; // CheckSum = 2 digits + "/0"
        sprintf(checksumStr, "%02X", checkSum);
        strcat(S1, checksumStr);
        strcat(S1, "\n");

        snprintf(memoryAddress, SIZE_OF_ADDRESS*2 + 1, "%04X", address);
        sizeCount -= MAX_BYTES*2;
    }


    /*----------------------------------------------------------------------*/
    /*-- Process s5 --------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    strcat(S5, "S503");
    char hexNumberOfS1[4] = "";
    sprintf(hexNumberOfS1, "%04X", numberOfS1);
    strcat(S5, hexNumberOfS1);


    int sum = 3 + numberOfS1;
    sum = ~sum;
    sum = sum & 0xFF;
    char temp[256];
    sprintf(temp, "%d\n", sum);
    sprintf(hexNumberOfS1, "%02X", sum);

    strcat(S5, hexNumberOfS1);
    strcat(S5, "\n");

    sizeCount -= MAX_BYTES;

    /*----------------------------------------------------------------------*/
    /*-- Process s9 --------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    strcpy(S9, "S9030000FC");

    // Writing a file
    writeFile(S0, *outputFileName);
    appendToFile(S1, *outputFileName);
    appendToFile(S5, *outputFileName);
    appendToFile(S9, *outputFileName);

    free(S1);
}

//
// Created by chris on 2/4/24.
//

#include "SRecordEncode.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define AUTHOR_NAME "JONGEON-HONGGYU"

#define MAX_SREC_LEN 42
#define MAX_CHARS    8
#define MAX_BYTES    16
#define SIZE_OF_CHECKSUM 2
#define SIZE_OF_ADDRESS  4

char calculateChecksum(const char *record, int length)
{
    unsigned int sum = 0;
    while (length > 1) {
        unsigned int value;
        sscanf(record, "%2x", &value);
        sum += value;
        record += 2;
        length -= 2;
    }
    return (unsigned char)(~sum);
}

char* decimalToHexadecimal(int decimal)
{
    char hex[9];   // The maximum hexadecimal length of a 32-bit integer is 8 digits + the null character.
    int index = 0; // Current memory

    while(decimal != 0) {
        int temp = 0;
        temp = decimal % 16;

        if(temp < 10) {
            hex[index] = temp + 48; // ASCII '0' = 48
        } else {
            hex[index] = temp + 55; // ASCII 'A' = 65, A~F = 10~15
        }

        index++;
        decimal = decimal / 16;
    }

    hex[index] = '\0'; // Add NULL termination

    char *hexadecimal = calloc(strlen(hex), sizeof(char));
    if (hexadecimal == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    strcpy(hexadecimal, hex);
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
    int contentsBytes = strlen(*fileContents) * 2;
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
    char *S1= calloc(S1TotalSize, sizeof(char));
    if (S1 == NULL) {
        perror("Memory allocation failed");
        return;
    }
    char S5[MAX_SREC_LEN] = "\0";
    char S9[MAX_SREC_LEN] = "\0";
    char memoryAddress[SIZE_OF_ADDRESS + 1] = {'0', '0', '0', '0'};

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
        unsigned char checkSum = calculateChecksum(ASCII, byteLength);
        char checksumStr[3]; // CheckSum = 2 digits + "/0"
        sprintf(checksumStr, "%02X", checkSum);
        strcat(S0, checksumStr);

        free(ASCII);
    }


    /*----------------------------------------------------------------------*/
    /*-- Process s1 --------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    int sizeCount = strlen(*fileContents);
    int indexCount = 0;
    while (sizeCount > 0)
    {
        // Accumulate "S1" and memory address
        snprintf(S1, S1TotalSize, "S1%s", memoryAddress);

        // Prepare to update the memory address
        // Convert address in string to integer
        unsigned int address;
        sscanf(memoryAddress, "%x", &address);

        // Check whether the data size is larger than 16 bytes, and
        // if so, truncate it to 16 bytes and save it.
        if(sizeCount > MAX_BYTES) {
            byteLength = MAX_BYTES + SIZE_OF_CHECKSUM + SIZE_OF_ADDRESS;
            // Add 16 bytes
            address += MAX_BYTES;
        }
        else {
            byteLength = sizeCount + SIZE_OF_CHECKSUM + SIZE_OF_ADDRESS;
            address += sizeCount;
        }
        snprintf(memoryAddress, SIZE_OF_ADDRESS+1, "%04X", address);

        // Accumulate LL code to S1
        char *hexadecimal = decimalToHexadecimal(byteLength);
        if(hexadecimal != NULL)
        {
            strcat(S1, hexadecimal); // add data size in hexadecimal
            free(hexadecimal);
        }



        sizeCount -= MAX_BYTES;
    }

    free(S1);
}

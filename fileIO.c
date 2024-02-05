//
// Created by chris on 2/4/24.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* readFile(char* inputFileName)
{
    FILE *file;
    unsigned char *buffer;
    long file_size;
    char *hexString;
    int i;

    // 파일을 이진 모드로 열기
    file = fopen(inputFileName, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // 파일 크기 확인
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // 파일 내용을 저장할 메모리 할당
    buffer = (unsigned char *)malloc(file_size * sizeof(unsigned char));
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // 파일 읽기
    fread(buffer, sizeof(unsigned char), file_size, file);

    // 16진수 문자열을 저장할 메모리 할당 (각 바이트는 2자리 16진수 문자열이 됨)
    hexString = (char *)malloc((file_size * 2 + 1) * sizeof(char));
    if (hexString == NULL) {
        perror("Memory allocation failed");
        free(buffer);
        fclose(file);
        return NULL;
    }

    // 각 바이트를 16진수 문자열로 변환
    for (i = 0; i < file_size; i++) {
        sprintf(&hexString[i * 2], "%02X", buffer[i]);
    }
    hexString[file_size * 2] = '\0'; // 문자열 종료

    // 메모리 해제 및 파일 닫기
    free(buffer);
    fclose(file);

    int length = strlen(hexString) / 2; // 16진수 문자열의 길이의 절반은 ASCII 문자열의 길이
    char* asciiString = (char *)malloc(length + 1); // +1은 널 종료 문자를 위함
    if (asciiString == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        sscanf(&hexString[i * 2], "%2hhX", &asciiString[i]);
    }
    asciiString[length] = '\0'; // 널 종료 문자 추가

    return hexString;
}

void writeFile(const char *contents, const char *outputFileName) {
    // Open the file to writing mode
    FILE *file = fopen(outputFileName, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // write a file
    fputs(contents, file);

    // close a file
    fclose(file);
}

void appendToFile(const char *contents, const char *outputFileName) {
    // Open the file to writing mode
    FILE *file = fopen(outputFileName, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // write a file
    fputs(contents, file);

    // close a file
    fclose(file);
}
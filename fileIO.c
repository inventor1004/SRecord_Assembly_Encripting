//
// Created by chris on 2/4/24.
//
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "fileIO.h"

bool checkExtension(const char *filename, const char *extension)
{
    const char *dot = strrchr(filename, '.'); // Find the memory location of '.'
    if (!dot || dot == filename) {
        return false; // If there is no extension
    }
    return strcmp(dot, extension) == 0; // Check whether the extension is the same or not
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
        free(fileContents);
        exit(EXIT_FAILURE);
    }

    return fileContents;
}
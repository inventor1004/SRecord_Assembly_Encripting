//
// Created by chris on 2/4/24.
//
#ifndef SRECORD_ASSEMBLY_ENCRIPTING_FILEIO_H
#define SRECORD_ASSEMBLY_ENCRIPTING_FILEIO_H

#include <stdbool.h>

bool checkExtension(const char *filename, const char *extension);
char* readFile(char* inputFileName);

#endif //SRECORD_ASSEMBLY_ENCRIPTING_FILEIO_H

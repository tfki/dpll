#ifndef DPLLSOLVER_FREAD_H
#define DPLLSOLVER_FREAD_H

#include "sanitize.h"
#include <cnf/cnf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
fReadAll(const char* pFileName, char** pRawStr)
{
  SANITIZE_PARAMETER_POINTER(pFileName);
  SANITIZE_PARAMETER_POINTER(pRawStr);

  const size_t elementSize = 1u;
  const size_t bufferSize = 8192u;
  char buffer[8192u];

  // open the file

  FILE* file = fopen(pFileName, "rb");
  if (!file)
    return 1;

  // find the size of the file

  fseek(file, 0l, SEEK_END);
  const size_t fileSize = ftell(file);
  rewind(file);

  // allocate the string

  const size_t stringSize = fileSize + 1u;
  char* string = malloc(stringSize);
  if (!string)
    return 1;

  // copy the file content into the string

  size_t currentPos = 0u;
  do {
    size_t numRead = fread(buffer, elementSize, bufferSize, file);
    memcpy(&string[currentPos], buffer, numRead);
    currentPos += numRead;
  } while (currentPos < fileSize);

  // add zero termination and write back the result

  string[fileSize] = '\0';
  *pRawStr = string;

  return 0;
}

int
fReadCnf(const char* pFileName, Cnf* pCnf)
{
  SANITIZE_PARAMETER_POINTER(pFileName);
  SANITIZE_PARAMETER_POINTER(pCnf);

  char* dimacs;
  if (fReadAll(pFileName, &dimacs))
    return 1;

  int error = parseDimacs(dimacs, pCnf);

  free(dimacs);
  return error;
}

#endif

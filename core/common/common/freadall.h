#ifndef DPLLSOLVER_FREADALL_H
#define DPLLSOLVER_FREADALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
freadall(const char* fileName, char** raw)
{
  const size_t elementSize = 1u;
  const size_t bufferSize = 16384u;
  char buffer[16384u];

  // open the file

  FILE* file = fopen(fileName, "rb");
  if (!file)
    return 1;

  // find the size of the file

  fseek(file, 0l, SEEK_END);
  const size_t fileSize = ftell(file);
  rewind(file);

  // allocate the string and add zero termination

  const size_t stringSize = fileSize + 1u;
  char* string = malloc(stringSize);
  string[fileSize] = '\0';

  // copy the file content into the string

  size_t currentPos = 0u;
  do {
    size_t numRead = fread(buffer, elementSize, bufferSize, file);
    memcpy(&string[currentPos], buffer, numRead);
    currentPos += numRead;
  } while (currentPos < fileSize);

  // write back the result

  *raw = string;
  return 0;
}

#endif // DPLLSOLVER_FREADALL_H

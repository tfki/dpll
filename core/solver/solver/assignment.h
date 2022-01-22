
#ifndef DPLLSOLVER_ASSIGNMENT_H
#define DPLLSOLVER_ASSIGNMENT_H

#include <stdint.h>
#include <string.h>

typedef struct assignment
{
  uint32_t* pKeys;
  int8_t* pValues;
  size_t capacity;
  size_t count;
} assignment;

int
assignment_create(assignment* pAssignment);

int
assignment_copy(assignment* pDest, const assignment* pSrc);

int
assignment_set(assignment* pAssignment, uint32_t key, int8_t value);

int
assignment_get(const assignment* pAssignment, uint32_t key, int8_t* value);

void
assignment_destroy(assignment* pAssignment);

#endif

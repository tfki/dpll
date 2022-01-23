#ifndef SOLVER_ASSIGNMENT_H
#define SOLVER_ASSIGNMENT_H

#include <stdint.h>
#include <string.h>

typedef struct Assignment
{
  uint32_t* pKeys;
  int8_t* pValues;
  size_t capacity;
  size_t count;
} Assignment;

int
Assignment_create(Assignment* pAssignment);

int
Assignment_copy(Assignment* pDest, const Assignment* pSrc);

int
Assignment_set(Assignment* pAssignment, uint32_t key, int8_t value);

int
Assignment_get(const Assignment* pAssignment, uint32_t key, int8_t* value);

void
Assignment_destroy(Assignment* pAssignment);

#endif

#ifndef DPLLSOLVER_ASSIGNMENT_H
#define DPLLSOLVER_ASSIGNMENT_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct Assignment
{
  uint32_t* pKeys;
  bool* pValues;
  size_t capacity;
  size_t count;
} Assignment;

int
Assignment_create(Assignment* pAssignment);

int
Assignment_copy(Assignment* pDest, const Assignment* pSrc);

int
Assignment_set(Assignment* pAssignment, uint32_t key, bool value);

int
Assignment_get(const Assignment* pAssignment, uint32_t key, bool* value);

void
Assignment_swap(Assignment* a, Assignment* b);

int
Assignment_setAll(Assignment* target, Assignment* src);

void
Assignment_destroy(Assignment* pAssignment);

#endif

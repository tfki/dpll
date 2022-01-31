#ifndef DPLLSOLVER_ASSIGNMENTSTACK_H
#define DPLLSOLVER_ASSIGNMENTSTACK_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct AssignmentStack
{
  uint32_t* pKeys;
  bool* pValues;
  size_t capacity;
  size_t count;
} AssignmentStack;

int
AssignmentStack_create(AssignmentStack* pAssignment);

int
AssignmentStack_copy(AssignmentStack* pDest, const AssignmentStack* pSrc);

int
AssignmentStack_push(AssignmentStack* pAssignment, uint32_t key, bool value);

int
AssignmentStack_pop(AssignmentStack* pAssignment);

int
AssignmentStack_get(const AssignmentStack* pAssignment, uint32_t key, bool* value);

void
AssignmentStack_swap(AssignmentStack* pFirst, AssignmentStack* pSecond);

void
AssignmentStack_destroy(AssignmentStack* pAssignment);

typedef struct AssignmentStackView
{
  const uint32_t* pKeys;
  const bool* pValues;
  size_t count;
} AssignmentStackView;

void
AssignmentStackView_beginView(AssignmentStackView* pAssignmentView, const AssignmentStack* pAssignment);

void
AssignmentStackView_endView(AssignmentStackView* pAssignmentView, const AssignmentStack* pAssignment);

int
AssignmentStackView_get(const AssignmentStackView* pAssignment, uint32_t key, bool* value);

#endif

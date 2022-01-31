#include "solver/assignmentstack.h"

#include <common/sanitize.h>
#include <stdlib.h>

int
AssignmentStack_create(AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pAssignment);

  pAssignment->count = 0u;
  pAssignment->capacity = 1024u;
  pAssignment->pKeys = malloc(pAssignment->capacity * sizeof(*pAssignment->pKeys) + pAssignment->capacity * sizeof(*pAssignment->pValues));

  if (!pAssignment->pKeys)
    return 1;

  pAssignment->pValues = (bool*)(pAssignment->pKeys + pAssignment->capacity);

  return 0;
}

int
AssignmentStack_copy(AssignmentStack* pDest, const AssignmentStack* pSrc)
{
  SANITIZE_PARAMETER_POINTER(pDest);
  SANITIZE_PARAMETER_POINTER(pSrc);

  pDest->capacity = pSrc->capacity;
  pDest->count = pSrc->count;
  pDest->pKeys = malloc(pSrc->capacity * sizeof(*pDest->pKeys) + pSrc->capacity * sizeof(*pDest->pValues));
  pDest->pValues = (bool*)(pDest->pKeys + pSrc->capacity);

  if (!pDest->pKeys)
    return 1;

  memcpy(pDest->pKeys, pSrc->pKeys, pSrc->count * sizeof(uint32_t));
  memcpy(pDest->pValues, pSrc->pValues, pSrc->count * sizeof(bool));

  return 0;
}

int
AssignmentStack_push(AssignmentStack* pAssignment, uint32_t key, bool value)
{
  SANITIZE_PARAMETER_POINTER(pAssignment);
  SANITIZE_PARAMETER_POINTER(key);
  SANITIZE_PARAMETER_BOOL(value);

  if (pAssignment->count + 1 > pAssignment->capacity) {
    uint32_t* pNewKeys = realloc(pAssignment->pKeys, pAssignment->capacity * 2u * sizeof(*pAssignment->pKeys) + pAssignment->capacity * 2u * sizeof(*pAssignment->pValues));
    if (!pNewKeys)
      return 1;

    pAssignment->pKeys = pNewKeys;

    pAssignment->pValues = (bool*)(pAssignment->pKeys + pAssignment->capacity * 2u);
    pAssignment->capacity *= 2;
  }

  // insert key
  pAssignment->pKeys[pAssignment->count] = key;
  pAssignment->pValues[pAssignment->count] = value;
  ++pAssignment->count;

  return 0;
}

int
AssignmentStack_pop(AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pAssignment);

  if (pAssignment->count == 0)
    return 1;

  --pAssignment->count;
  return 0;
}

int
AssignmentStack_get(const AssignmentStack* pAssignment, uint32_t key, bool* value)
{
  SANITIZE_PARAMETER_POINTER(pAssignment);

  // see if key already exists in pKeys
  for (int i = 0; i < pAssignment->count; ++i) {
    if (pAssignment->pKeys[i] == key) {
      *value = pAssignment->pValues[i];
      return 0;
    }
  }

  return 1;
}

void
AssignmentStack_swap(AssignmentStack* pFirst, AssignmentStack* pSecond)
{
  SANITIZE_PARAMETER_POINTER(pFirst);
  SANITIZE_PARAMETER_POINTER(pSecond);
  AssignmentStack third = *pFirst;
  *pFirst = *pSecond;
  *pSecond = third;
}

void
AssignmentStack_destroy(AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pAssignment);
  free(pAssignment->pKeys);
  pAssignment->pKeys = NULL;
  pAssignment->pValues = NULL;
  pAssignment->capacity = 0u;
  pAssignment->count = 0u;
}

void
AssignmentStackView_beginView(AssignmentStackView* pAssignmentView, const AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pAssignmentView);
  SANITIZE_PARAMETER_POINTER(pAssignment);
  pAssignmentView->count = pAssignment->count;
}

void
AssignmentStackView_endView(AssignmentStackView* pAssignmentView, const AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pAssignmentView);
  SANITIZE_PARAMETER_POINTER(pAssignment);
  SANITIZE_ASSERT(pAssignmentView->count <= pAssignment->count, "AssignmentStackView can not watch a negative range!");

  pAssignmentView->pKeys = &pAssignment->pKeys[pAssignmentView->count];
  pAssignmentView->pValues = &pAssignment->pValues[pAssignmentView->count];
  pAssignmentView->count = pAssignment->count - pAssignmentView->count;
}

int
AssignmentStackView_get(const AssignmentStackView* pAssignment, uint32_t key, bool* value)
{
  SANITIZE_PARAMETER_POINTER(pAssignment);

  // see if key already exists in pKeys
  for (int i = 0; i < pAssignment->count; ++i) {
    if (pAssignment->pKeys[i] == key) {
      *value = pAssignment->pValues[i];
      return 0;
    }
  }

  return 1;
}

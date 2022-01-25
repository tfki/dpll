#include "solver/assignmentstack.h"
#include <common/common.h>
#include <stdlib.h>

int
AssignmentStack_create(AssignmentStack* pAssignment)
{
  SANITIZING_ASSERT(pAssignment); // pAssignment must be a valid pointer

  pAssignment->count = 0u;
  pAssignment->capacity = 1024u;
  pAssignment->pKeys = malloc(pAssignment->capacity * sizeof(uint32_t) + pAssignment->capacity * sizeof(bool));

  if (!pAssignment->pKeys)
    return 1;

  pAssignment->pValues = (bool*)(pAssignment->pKeys + pAssignment->capacity);

  return 0;
}

int
AssignmentStack_copy(AssignmentStack* pDest, const AssignmentStack* pSrc)
{
  SANITIZING_ASSERT(pDest); // pDest must be a valid pointer
  SANITIZING_ASSERT(pSrc);  // pSrc must be a valid pointer

  pDest->capacity = pSrc->capacity;
  pDest->count = pSrc->count;
  pDest->pKeys = malloc(pSrc->capacity * sizeof(int32_t) + pSrc->capacity * sizeof(bool));
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
  SANITIZING_ASSERT(pAssignment);              // pAssignment must be a valid pointer
  SANITIZING_ASSERT(key);                      // zero key is not allowed!
  SANITIZING_ASSERT(value == 0 || value == 1); // only "normalized' booleans are allowed

  if (pAssignment->count + 1 > pAssignment->capacity) {
    uint32_t* pNewKeys = realloc(pAssignment->pKeys, pAssignment->capacity * 2u * sizeof(uint32_t) + pAssignment->capacity * 2u * sizeof(bool));
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
  SANITIZING_ASSERT(pAssignment); // pAssignment must be a valid pointer

  if (pAssignment->count == 0)
    return 1;

  --pAssignment->count;
  return 0;
}

int
AssignmentStack_get(const AssignmentStack* pAssignment, uint32_t key, bool* value)
{
  SANITIZING_ASSERT(pAssignment); // pAssignment must be a valid pointer

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
AssignmentStack_swap(AssignmentStack* a, AssignmentStack* b)
{
  SANITIZING_ASSERT(a); // a must be a valid pointer
  SANITIZING_ASSERT(b); // b must be a valid pointer

  uint32_t* pKeysTmp = a->pKeys;
  bool* pValuesTmp = a->pValues;
  size_t countTmp = a->count;
  size_t capacityTmp = a->capacity;

  a->pKeys = b->pKeys;
  a->pValues = b->pValues;
  a->count = b->count;
  a->capacity = b->capacity;

  b->pKeys = pKeysTmp;
  b->pValues = pValuesTmp;
  b->count = countTmp;
  b->capacity = capacityTmp;
}

void
AssignmentStack_destroy(AssignmentStack* pAssignment)
{
  SANITIZING_ASSERT(pAssignment); // pAssignment must be a valid pointer

  free(pAssignment->pKeys);
  pAssignment->pKeys = NULL;
  pAssignment->pValues = NULL;
  pAssignment->capacity = 0u;
  pAssignment->count = 0u;
}

void
AssignmentStackView_beginView(AssignmentStackView* pAssignmentView, const AssignmentStack* pAssignment)
{
  SANITIZING_ASSERT(pAssignmentView); // pAssignmentView must be a valid pointer
  SANITIZING_ASSERT(pAssignment);     // pAssignment must be a valid pointer
  pAssignmentView->count = pAssignment->count;
}

void
AssignmentStackView_endView(AssignmentStackView* pAssignmentView, const AssignmentStack* pAssignment)
{
  SANITIZING_ASSERT(pAssignmentView);                             // pAssignmentView must be a valid pointer
  SANITIZING_ASSERT(pAssignment);                                 // pAssignment must be a valid pointer
  SANITIZING_ASSERT(pAssignmentView->count <= pAssignment->count); // only pushes are allowed between beginView and endView

  pAssignmentView->pKeys = &pAssignment->pKeys[pAssignmentView->count];
  pAssignmentView->pValues = &pAssignment->pValues[pAssignmentView->count];
  pAssignmentView->count = pAssignment->count - pAssignmentView->count;
}

int
AssignmentStackView_get(const AssignmentStackView* pAssignment, uint32_t key, bool* value)
{
  SANITIZING_ASSERT(pAssignment); // pAssignment must be a valid pointer

  // see if key already exists in pKeys
  for (int i = 0; i < pAssignment->count; ++i) {
    if (pAssignment->pKeys[i] == key) {
      *value = pAssignment->pValues[i];
      return 0;
    }
  }

  return 1;
}

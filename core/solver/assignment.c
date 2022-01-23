#include "solver/assignment.h"
#include <stdlib.h>

int
Assignment_create(Assignment* pAssignment)
{
  pAssignment->count = 0u;
  pAssignment->capacity = 1024u;
  pAssignment->pKeys = malloc(pAssignment->capacity * sizeof(uint32_t) + pAssignment->capacity * sizeof(int8_t));

  if (!pAssignment->pKeys)
    return 1;

  pAssignment->pValues = (int8_t*)(pAssignment->pKeys + pAssignment->capacity);

  return 0;
}

int
Assignment_copy(Assignment* pDest, const Assignment* pSrc)
{
  pDest->capacity = pSrc->capacity;
  pDest->count = pSrc->count;
  pDest->pKeys = malloc(pSrc->capacity * sizeof(int32_t) + pSrc->capacity * sizeof(int8_t));
  pDest->pValues = (int8_t*)(pDest->pKeys + pSrc->capacity);

  if (!pDest->pKeys)
    return 1;

  memcpy(pDest->pKeys, pSrc->pKeys, pSrc->count * sizeof(uint32_t));
  memcpy(pDest->pValues, pSrc->pValues, pSrc->count * sizeof(uint8_t));

  return 0;
}

int
Assignment_set(Assignment* pAssignment, uint32_t key, int8_t value)
{
  // see if key already exists in pKeys
  for (int i = 0; i < pAssignment->count; ++i) {
    // if so, set its new value and return
    if (pAssignment->pKeys[i] == key) {
      pAssignment->pValues[i] = value;
      return 0;
    }
  }

  // if key does not already exist
  if (pAssignment->count + 1 > pAssignment->capacity) {
    uint32_t* pNewKeys = realloc(pAssignment->pKeys, pAssignment->capacity * 2u * sizeof(uint32_t) + pAssignment->capacity * 2u * sizeof(uint8_t));
    if (!pNewKeys)
      return 1;

    pAssignment->pKeys = pNewKeys;

    pAssignment->pValues = (int8_t*)(pAssignment->pKeys + pAssignment->capacity * 2u);
    pAssignment->capacity *= 2;
  }

  // insert key
  pAssignment->pKeys[pAssignment->count] = key;
  pAssignment->pValues[pAssignment->count] = value;

  ++pAssignment->count;

  return 0;
}

int
Assignment_get(const Assignment* pAssignment, uint32_t key, int8_t* value)
{
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
Assignment_destroy(Assignment* pAssignment)
{
  free(pAssignment->pKeys);
  pAssignment->pKeys = NULL;
  pAssignment->pValues = NULL;
  pAssignment->capacity = 0u;
  pAssignment->count = 0u;
}

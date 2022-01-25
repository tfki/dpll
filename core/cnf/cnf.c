#include "cnf/cnf.h"

#include <common/common.h>
#include <string.h>

int
Cnf_create(Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnf); // pCnf must be a valid pointer

  pCnf->count = 0u;
  pCnf->capacity = 1024u;
  pCnf->pData = malloc(pCnf->capacity * sizeof(int32_t));

  if (!pCnf->pData)
    return 1;

  return 0;
}

int
Cnf_copy(Cnf* pDest, const Cnf* pSrc)
{
  SANITIZING_ASSERT(pDest); // pDest must be a valid pointer
  SANITIZING_ASSERT(pSrc);  // pSrc must be a valid pointer

  pDest->count = pSrc->count;
  pDest->capacity = pSrc->capacity;
  pDest->pData = malloc(pSrc->capacity * sizeof(int32_t));

  if (!pDest->pData)
    return 1;

  memcpy(pDest->pData, pSrc->pData, pSrc->count * sizeof(int32_t));
  return 0;
}

int
Cnf_pushClause(Cnf* pCnf, const int32_t* pValues, size_t count)
{
  SANITIZING_ASSERT(pCnf); // pCnf must be a valid pointer

  // if Cnf empty, add leading 0
  if (pCnf->count == 0u) {
    pCnf->pData[0u] = 0;
    ++pCnf->count;
  }

  while (pCnf->count + count + 1u > pCnf->capacity) {

    int32_t* pNewData = realloc(pCnf->pData, pCnf->capacity * 2u * sizeof(int32_t));
    if (!pNewData)
      return 1;

    pCnf->pData = pNewData;
    pCnf->capacity *= 2u;
  }

  memcpy(pCnf->pData + pCnf->count, pValues, count * sizeof(int32_t));

  pCnf->count += count;
  pCnf->pData[pCnf->count] = 0;
  ++pCnf->count;

  return 0;
}

void
Cnf_destroy(Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnf); // pCnf must be a valid pointer

  free(pCnf->pData);
  pCnf->pData = NULL;
  pCnf->capacity = 0u;
  pCnf->count = 0u;
}

void
Cnf_reset(Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnf); // pCnf must be a valid pointer
  pCnf->count = 0u;
}

void
Cnf_swap(Cnf* a, Cnf* b)
{
  int32_t* tmpPData = a->pData;
  size_t tmpCapacity = a->capacity;
  size_t tmpCount = a->count;

  a->pData = b->pData;
  a->count = b->count;
  a->capacity = b->capacity;

  b->pData = tmpPData;
  b->count = tmpCount;
  b->capacity = tmpCapacity;
}

void
Cnf_ClauseIterator_create(Cnf_ClauseIterator* pCnfClauseIterator, const Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnfClauseIterator); // pCnfClauseIterator must be a valid pointer
  SANITIZING_ASSERT(pCnf);               // pCnf must be a valid pointer

  pCnfClauseIterator->pData = pCnf->pData;
  pCnfClauseIterator->pDataEnd = pCnf->pData + pCnf->count;
  pCnfClauseIterator->count = 0u;
}

bool
Cnf_ClauseIterator_next(Cnf_ClauseIterator* pCnfClauseIterator)
{
  SANITIZING_ASSERT(pCnfClauseIterator); // pCnfClauseIterator must be a valid pointer

  // + 1u to skip the 0 at pCnfClauseIterator->pData
  const int32_t* pNextData = pCnfClauseIterator->pData + pCnfClauseIterator->count + 1u;
  if (pNextData >= pCnfClauseIterator->pDataEnd)
    return 0;

  for (size_t i = 0u;; ++i) {

    if (pNextData[i] == 0u) {
      pCnfClauseIterator->pData = pNextData;
      pCnfClauseIterator->count = i;
      return 1;
    }
  }
}

int
ClauseBuffer_create(ClauseBuffer* pClauseBuffer)
{
  SANITIZING_ASSERT(pClauseBuffer); // pClauseBuffer must be a valid pointer

  pClauseBuffer->count = 0u;
  pClauseBuffer->capacity = 1024u;
  pClauseBuffer->pData = malloc(pClauseBuffer->capacity * sizeof(int32_t));

  if (!pClauseBuffer->pData)
    return 1;

  return 0;
}

int
ClauseBuffer_push(ClauseBuffer* pClauseBuffer, int32_t literal)
{
  SANITIZING_ASSERT(pClauseBuffer); // pClauseBuffer must be a valid pointer

  if (pClauseBuffer->count + 1u > pClauseBuffer->capacity) {

    int32_t* pNewData = realloc(pClauseBuffer->pData, pClauseBuffer->capacity * 2u * sizeof(int32_t));
    if (!pNewData)
      return 1;

    pClauseBuffer->pData = pNewData;
    pClauseBuffer->capacity *= 2u;
  }

  pClauseBuffer->pData[pClauseBuffer->count] = literal;
  ++pClauseBuffer->count;

  return 0;
}

void
ClauseBuffer_destroy(ClauseBuffer* pClauseBuffer)
{
  SANITIZING_ASSERT(pClauseBuffer); // pClauseBuffer must be a valid pointer

  free(pClauseBuffer->pData);
  pClauseBuffer->pData = NULL;
  pClauseBuffer->capacity = 0u;
  pClauseBuffer->count = 0u;
}

void
ClauseBuffer_reset(ClauseBuffer* pClauseBuffer)
{
  SANITIZING_ASSERT(pClauseBuffer); // pClauseBuffer must be a valid pointer
  pClauseBuffer->count = 0u;
}

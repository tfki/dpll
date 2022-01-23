#include "cnf/cnf.h"
#include <string.h>

int
Cnf_create(Cnf* pCnf)
{
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
  free(pCnf->pData);
  pCnf->pData = NULL;
  pCnf->capacity = 0u;
  pCnf->count = 0u;
}

void
Cnf_reset(Cnf* pCnf)
{
  pCnf->count = 0u;
}

void
Cnf_ClauseIterator_create(Cnf_ClauseIterator* pCnfClauseIterator, const Cnf* pCnf)
{
  pCnfClauseIterator->pData = pCnf->pData;
  pCnfClauseIterator->pDataEnd = pCnf->pData + pCnf->count;
  pCnfClauseIterator->count = 0u;
}

int8_t
Cnf_ClauseIterator_next(Cnf_ClauseIterator* pCnfClauseIterator)
{
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

#include "cnf/cnf.h"

#include <string.h> // memcpy

int
cnf_create(cnf* pCnf)
{
  pCnf->count = 0u;
  pCnf->capacity = 1024u;
  pCnf->pData = malloc(pCnf->capacity * sizeof(int32_t));

  if (!pCnf->pData)
    return 1;

  return 0;
}

int
cnf_copy(cnf* pDest, const cnf* pSrc)
{
  pDest->count = pSrc->count;
  pDest->capacity = pSrc->capacity;
  pDest->pData = malloc(pSrc->capacity * sizeof(int32_t));

  if (!pDest->pData)
    return 1;

  if (memcpy_s(pDest->pData, pDest->capacity * sizeof(int32_t), pSrc->pData, pSrc->count * sizeof(int32_t))) {
    free(pDest->pData);
    return 1;
  }

  return 0;
}

int
cnf_pushClause(cnf* pCnf, const int32_t* pValues, size_t count)
{
  while (pCnf->count + count + 1u > pCnf->capacity) {

    int32_t* pNewData = realloc(pCnf->pData, pCnf->capacity * 2u * sizeof(int32_t));
    if (!pNewData)
      return 1;

    pCnf->pData = pNewData;
    pCnf->capacity *= 2u;
  }

  if (memcpy_s(pCnf->pData + pCnf->count, (pCnf->capacity - pCnf->count) * sizeof(int32_t), pValues, count * sizeof(int32_t)))
    return 1;

  pCnf->count += count;
  pCnf->pData[pCnf->count] = 0;
  ++pCnf->count;

  return 0;
}

void
cnf_destroy(cnf* pCnf)
{
  free(pCnf->pData);
  pCnf->pData = NULL;
  pCnf->capacity = 0u;
  pCnf->count = 0u;
}

void
cnf_reset(cnf* pCnf)
{
  pCnf->count = 0u;
}

void
cnf_clause_iterator_create(cnf_clause_iterator* pCnfClauseIterator, const cnf* pCnf)
{
  pCnfClauseIterator->pData = pCnf->pData;
  pCnfClauseIterator->pDataEnd = pCnf->pData + pCnf->count;
  pCnfClauseIterator->count = 0u;
}

int
cnf_clause_iterator_next(cnf_clause_iterator* pCnfClauseIterator)
{
  const int32_t * pNextData = pCnfClauseIterator->pData + pCnfClauseIterator->count;
  if (pNextData >= pCnfClauseIterator->pDataEnd)
    return 1;

  for (size_t i = 0u;;++i) {

    if (pNextData[i] == 0u) {
      pCnfClauseIterator->pData = pNextData;
      pCnfClauseIterator->count = i;
      return 0;
    }
  }
}

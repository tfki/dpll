#include "cnf/cnf.h"

#include <common/log.h>
#include <common/sanitize.h>

#include <stdio.h>
#include <string.h>

int
Cnf_create(Cnf* pCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnf);

  pCnf->count = 0u;
  pCnf->capacity = 1024u;
  pCnf->pData = malloc(pCnf->capacity * sizeof(*pCnf->pData));

  if (!pCnf->pData) {
    LOGE("Cnf memory could not be allocated during creation!");
    return 1;
  }

  LOGD("Cnf created successfully.");
  return 0;
}

int
Cnf_copy(Cnf* pDest, const Cnf* pSrc)
{
  SANITIZE_PARAMETER_POINTER(pDest);
  SANITIZE_PARAMETER_POINTER(pSrc);

  pDest->count = pSrc->count;
  pDest->capacity = pSrc->capacity;
  pDest->pData = malloc(pSrc->capacity * sizeof(*pDest->pData));

  if (!pDest->pData) {
    LOGE("Cnf memory could not be allocated during copy!");
    return 1;
  }

  memcpy(pDest->pData, pSrc->pData, pSrc->count * sizeof(*pSrc->pData));
  LOGD("Cnf copied successfully. %%s"); // TODO insert Cnf_str(pCnf)
  return 0;
}

int
Cnf_pushClause(Cnf* pCnf, const int32_t* pValues, size_t count)
{
  SANITIZE_PARAMETER_POINTER(pCnf);

  // if Cnf empty, add leading 0
  if (pCnf->count == 0u) { // TODO make leading zero inside creation?
    pCnf->pData[0u] = 0;
    ++pCnf->count;
  }

  while (pCnf->count + count + 1u > pCnf->capacity) {

    int32_t* pNewData = realloc(pCnf->pData, pCnf->capacity * 2u * sizeof(*pCnf->pData));
    if (!pNewData) {
      LOGE("Cnf memory could not be allocated during pushClause!");
      return 1;
    }

    LOGD("Cnf memory was reallocated successfully during pushClause!");
    pCnf->pData = pNewData;
    pCnf->capacity *= 2u;
  }

  memcpy(pCnf->pData + pCnf->count, pValues, count * sizeof(*pCnf->pData));

  pCnf->count += count;
  pCnf->pData[pCnf->count] = 0;
  ++pCnf->count;

  return 0;
}

void
Cnf_destroy(Cnf* pCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnf);

  free(pCnf->pData);
  pCnf->pData = NULL;
  pCnf->capacity = 0u;
  pCnf->count = 0u;
}

void
Cnf_reset(Cnf* pCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  pCnf->count = 0u;
}

void
Cnf_swap(Cnf* pFirst, Cnf* pSecond)
{
  SANITIZE_PARAMETER_POINTER(pFirst);
  SANITIZE_PARAMETER_POINTER(pSecond);

  Cnf third = *pFirst;
  // third.pData = pFirst->pData;
  // third.capacity = pFirst->capacity;
  // third.count = pFirst->count;

  *pFirst = *pSecond;
  // pFirst->pData = pSecond->pData;
  // pFirst->count = pSecond->count;
  // pFirst->capacity = pSecond->capacity;

  *pSecond = third;
  // pSecond->pData = third.pData;
  // pSecond->count = third.capacity;
  // pSecond->capacity = third.count;
}

int
Cnf_toStr(const Cnf* pCnf, char** pStr)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  SANITIZE_PARAMETER_POINTER(pStr);

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, pCnf);

  // pCnf->count * 2 is just a starting point
  // we cannot easily find out how long pStr will need to be
  size_t strLen = 128;
  *pStr = malloc(strLen * sizeof(char));

  if (!*pStr)
    return 1;

  size_t strIndex = 0u;
  while (Cnf_ClauseIterator_next(&iter)) {

    if (strIndex + 1 >= strLen) {
      *pStr = realloc(*pStr, strLen * 2 * sizeof(char));
      strLen *= 2;

      if (!*pStr) {
        free(*pStr);
        return 2;
      }
    }
    (*pStr)[strIndex++] = '(';
    for (size_t i = 0u; i < iter.count; ++i) {

      size_t literalLength;

      literalLength = snprintf(&(*pStr)[strIndex], strLen - strIndex, "%d", iter.pData[i]);

      while (strIndex + literalLength >= strLen) {
        *pStr = realloc(*pStr, strLen * 2 * sizeof(char));
        strLen *= 2;

        if (!*pStr) {
          free(*pStr);
          return 3;
        }

        literalLength = snprintf(&(*pStr)[strIndex], strLen - strIndex, "%d", iter.pData[i]);
      }

      strIndex += literalLength;

      if (strIndex + 4 >= strLen) {
        *pStr = realloc(*pStr, strLen * 2 * sizeof(char));
        strLen *= 2;

        if (!*pStr) {
          free(*pStr);
          return 4;
        }
      }
      (*pStr)[strIndex++] = ' ';
      (*pStr)[strIndex++] = 'O';
      (*pStr)[strIndex++] = 'R';
      (*pStr)[strIndex++] = ' ';
    }
    if (iter.count > 0) {
      // if clause has any literals
      // overwrite the last 'OR'
      (*pStr)[strIndex - 4] = ')';
      strIndex -= 3;
    } else {
      (*pStr)[strIndex++] = ')';
    }

    if (strIndex + 5 >= strLen) {
      *pStr = realloc(*pStr, strLen * 2 * sizeof(char));
      strLen *= 2;

      if (!*pStr) {
        free(*pStr);
        return 4;
      }
    }
    (*pStr)[strIndex++] = ' ';
    (*pStr)[strIndex++] = 'A';
    (*pStr)[strIndex++] = 'N';
    (*pStr)[strIndex++] = 'D';
    (*pStr)[strIndex++] = ' ';
  }

  if (strIndex + 1 >= strLen) {
    *pStr = realloc(*pStr, strLen * 2 * sizeof(char));
    strLen *= 2u;

    if (!*pStr) {
      free(*pStr);
      return 4;
    }
  }

  if (strIndex >= 5) {
    // -5 to overwrite last 'AND'
    (*pStr)[strIndex - 5] = '\0';
  } else {
    // pCnf is empty
    (*pStr)[0] = '\0';
  }
  return 0;
}

void
Cnf_ClauseIterator_create(Cnf_ClauseIterator* pCnfClauseIterator, const Cnf* pCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnfClauseIterator);
  SANITIZE_PARAMETER_POINTER(pCnf);
  pCnfClauseIterator->pData = pCnf->pData;
  pCnfClauseIterator->pDataEnd = pCnf->pData + pCnf->count;
  pCnfClauseIterator->count = 0u;
}

bool
Cnf_ClauseIterator_next(Cnf_ClauseIterator* pCnfClauseIterator)
{
  SANITIZE_PARAMETER_POINTER(pCnfClauseIterator);

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
  SANITIZE_PARAMETER_POINTER(pClauseBuffer);

  pClauseBuffer->count = 0u;
  pClauseBuffer->capacity = 128u;
  pClauseBuffer->pData = malloc(pClauseBuffer->capacity * sizeof(int32_t));

  if (!pClauseBuffer->pData)
    return 1;

  return 0;
}

int
ClauseBuffer_push(ClauseBuffer* pClauseBuffer, int32_t literal)
{
  SANITIZE_PARAMETER_POINTER(pClauseBuffer);

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
  SANITIZE_PARAMETER_POINTER(pClauseBuffer);
  free(pClauseBuffer->pData);
  pClauseBuffer->pData = NULL;
  pClauseBuffer->capacity = 0u;
  pClauseBuffer->count = 0u;
}

void
ClauseBuffer_reset(ClauseBuffer* pClauseBuffer)
{
  SANITIZE_PARAMETER_POINTER(pClauseBuffer);
  pClauseBuffer->count = 0u;
}

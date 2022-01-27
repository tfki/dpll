#include "cnf/cnf.h"

#include <common/common.h>
#include <common/log.h>
#include <stdio.h>
#include <string.h>

int
Cnf_create(Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnf, "Parameter pCnf must be a valid pointer!");

  pCnf->count = 0u;
  pCnf->capacity = 512u;
  pCnf->pData = malloc(pCnf->capacity * sizeof(int32_t));

  if (!pCnf->pData) {
    LOGE("Cnf memory could not be allocated during creation!");
    return 1;
  }

  LOGD("Cnf created successfully. %%s"); // TODO insert Cnf_str(pCnf)
  return 0;
}

int
Cnf_copy(Cnf* pDest, const Cnf* pSrc)
{
  SANITIZING_ASSERT(pDest, "Parameter pDest must be a valid pointer!");
  SANITIZING_ASSERT(pSrc, "Parameter pSrc must be a valid pointer!");

  pDest->count = pSrc->count;
  pDest->capacity = pSrc->capacity;
  pDest->pData = malloc(pSrc->capacity * sizeof(int32_t));

  if (!pDest->pData) {
    LOGE("Cnf memory could not be allocated during copy!");
    return 1;
  }

  memcpy(pDest->pData, pSrc->pData, pSrc->count * sizeof(int32_t));
  LOGD("Cnf copied successfully. %%s"); // TODO insert Cnf_str(pCnf)
  return 0;
}

int
Cnf_pushClause(Cnf* pCnf, const int32_t* pValues, size_t count)
{
  SANITIZING_ASSERT(pCnf, "Parameter pCnf must be a valid pointer!");

  // if Cnf empty, add leading 0
  if (pCnf->count == 0u) { // TODO make leading zero inside creation?
    pCnf->pData[0u] = 0;
    ++pCnf->count;
  }

  while (pCnf->count + count + 1u > pCnf->capacity) {

    int32_t* pNewData = realloc(pCnf->pData, pCnf->capacity * 2u * sizeof(int32_t));
    if (!pNewData) {
      LOGE("Cnf memory could not be allocated during pushClause!");
      return 1;
    }

    LOGD("Cnf memory was reallocated successfully during pushClause!");
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
  SANITIZING_ASSERT(pCnf, "Parameter pCnf must be a valid pointer!");

  free(pCnf->pData);
  pCnf->pData = NULL;
  pCnf->capacity = 0u;
  pCnf->count = 0u;
}

void
Cnf_reset(Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnf, "Parameter pCnf must be a valid pointer!");
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

int
Cnf_toStr(const Cnf* cnf, char** str)
{
  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, cnf);

  // cnf->count * 2 is just a starting point
  // we cannot easily find out how long str will need to be
  size_t strLen = 128;
  *str = malloc(strLen * sizeof(char));

  if (!*str)
    return 1;

  size_t strIndex = 0u;
  while (Cnf_ClauseIterator_next(&iter)) {

    if (strIndex + 1 >= strLen) {
      *str = realloc(*str, strLen * 2 * sizeof(char));
      strLen *= 2;

      if (!*str) {
        free(*str);
        return 2;
      }
    }
    (*str)[strIndex++] = '(';
    for (size_t i = 0u; i < iter.count; ++i) {

      size_t literalLength;

      literalLength = snprintf(&(*str)[strIndex], strLen - strIndex, "%d", iter.pData[i]);

      while (strIndex + literalLength >= strLen) {
        *str = realloc(*str, strLen * 2 * sizeof(char));
        strLen *= 2;

        if (!*str) {
          free(*str);
          return 3;
        }

        literalLength = snprintf(&(*str)[strIndex], strLen - strIndex, "%d", iter.pData[i]);
      }

      strIndex += literalLength;

      if (strIndex + 4 >= strLen) {
        *str = realloc(*str, strLen * 2 * sizeof(char));
        strLen *= 2;

        if (!*str) {
          free(*str);
          return 4;
        }
      }
      (*str)[strIndex++] = ' ';
      (*str)[strIndex++] = 'O';
      (*str)[strIndex++] = 'R';
      (*str)[strIndex++] = ' ';
    }
    if (iter.count > 0) {
      // if clause has any literals
      // overwrite the last 'OR'
      (*str)[strIndex - 4] = ')';
      strIndex -= 3;
    } else {
      (*str)[strIndex++] = ')';
    }

    if (strIndex + 5 >= strLen) {
      *str = realloc(*str, strLen * 2 * sizeof(char));
      strLen *= 2;

      if (!*str) {
        free(*str);
        return 4;
      }
    }
    (*str)[strIndex++] = ' ';
    (*str)[strIndex++] = 'A';
    (*str)[strIndex++] = 'N';
    (*str)[strIndex++] = 'D';
    (*str)[strIndex++] = ' ';
  }

  if (strIndex + 1 >= strLen) {
    *str = realloc(*str, strLen * 2 * sizeof(char));
    strLen *= 2;

    if (!*str) {
      free(*str);
      return 4;
    }
  }

  if (strIndex >= 5) {
    // -5 to overwrite last 'AND'
    (*str)[strIndex - 5] = '\0';
  } else {
    // cnf is empty
    (*str)[0] = '\0';
  }
  return 0;
}

void
Cnf_ClauseIterator_create(Cnf_ClauseIterator* pCnfClauseIterator, const Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnfClauseIterator, "Parameter pCnfClauseIterator must be a valid pointer!");
  SANITIZING_ASSERT(pCnf, "Parameter pCnf must be a valid pointer!");

  pCnfClauseIterator->pData = pCnf->pData;
  pCnfClauseIterator->pDataEnd = pCnf->pData + pCnf->count;
  pCnfClauseIterator->count = 0u;
}

bool
Cnf_ClauseIterator_next(Cnf_ClauseIterator* pCnfClauseIterator)
{
  SANITIZING_ASSERT(pCnfClauseIterator, "Parameter pCnfClauseIterator must be a valid pointer!");

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
  SANITIZING_ASSERT(pClauseBuffer, "Parameter pClauseBuffer must be a valid pointer!");

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
  SANITIZING_ASSERT(pClauseBuffer, "Parameter pClauseBuffer must be a valid pointer!");

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
  SANITIZING_ASSERT(pClauseBuffer, "Parameter pClauseBuffer must be a valid pointer!");

  free(pClauseBuffer->pData);
  pClauseBuffer->pData = NULL;
  pClauseBuffer->capacity = 0u;
  pClauseBuffer->count = 0u;
}

void
ClauseBuffer_reset(ClauseBuffer* pClauseBuffer)
{
  SANITIZING_ASSERT(pClauseBuffer, "Parameter pClauseBuffer must be a valid pointer!");
  pClauseBuffer->count = 0u;
}

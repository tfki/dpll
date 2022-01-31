#ifndef DPLLSOLVER_CNF_H
#define DPLLSOLVER_CNF_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Cnf
{
  int32_t* pData;
  size_t capacity;
  size_t count;
} Cnf;

int
Cnf_create(Cnf* pCnf);

int
Cnf_copy(Cnf* pDest, const Cnf* pSrc);

int
Cnf_pushClause(Cnf* pCnf, const int32_t* pValues, size_t count);

void
Cnf_destroy(Cnf* pCnf);

void
Cnf_reset(Cnf* pCnf);

void
Cnf_swap(Cnf* pFirst, Cnf* pSecond);

int
Cnf_toStr(const Cnf* pCnf, char** pStr);

typedef struct Cnf_ClauseIterator
{
  const int32_t* pData;    /// position of the current clause
  const int32_t* pDataEnd; /// end of Cnf
  size_t count;            /// count of literals in current clause
} Cnf_ClauseIterator;

void
Cnf_ClauseIterator_create(Cnf_ClauseIterator* pCnfClauseIterator, const Cnf* pCnf);

bool
Cnf_ClauseIterator_next(Cnf_ClauseIterator* pCnfClauseIterator);

typedef struct ClauseBuffer
{
  int32_t* pData;
  size_t capacity;
  size_t count;
} ClauseBuffer;

int
ClauseBuffer_create(ClauseBuffer* pClauseBuffer);

int
ClauseBuffer_push(ClauseBuffer* pClauseBuffer, int32_t literal);

void
ClauseBuffer_destroy(ClauseBuffer* pClauseBuffer);

void
ClauseBuffer_reset(ClauseBuffer* pClauseBuffer);

#endif

#ifndef DPLLSOLVER_CNF_H
#define DPLLSOLVER_CNF_H

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

typedef struct Cnf_ClauseIterator
{
  const int32_t* pData;    /// position of the current clause
  const int32_t* pDataEnd; /// end of Cnf
  size_t count;            /// count of literals in current clause
} Cnf_ClauseIterator;

void
Cnf_ClauseIterator_create(Cnf_ClauseIterator* pCnfClauseIterator, const Cnf* pCnf);

int
Cnf_ClauseIterator_next(Cnf_ClauseIterator* pCnfClauseIterator);

#endif

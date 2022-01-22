#ifndef CNF_CNF_H
#define CNF_CNF_H

#include <stdint.h> // int32_t
#include <stdlib.h> // size_t

typedef struct cnf
{
  int32_t* pData;
  size_t capacity;
  size_t count;
} cnf;

int
cnf_create(cnf* pCnf);

int
cnf_copy(cnf* pDest, const cnf* pSrc);

int
cnf_pushClause(cnf* pCnf, const int32_t* pValues, size_t count);

void
cnf_destroy(cnf* pCnf);

void
cnf_reset(cnf* pCnf);

int
cnf_simplify(const cnf* pCnf, cnf* pNextCnf);

typedef struct cnf_clause_iterator
{
  int32_t* pData;    /// position of the current clause
  int32_t* pDataEnd; /// end of cnf
  size_t count;      /// count of literals in current clause
} cnf_clause_iterator;

void
cnf_clause_iterator_create(cnf_clause_iterator* pCnfClauseIterator, cnf* pCnf)
{
  pCnfClauseIterator->pData = pCnf->pData;
  pCnfClauseIterator->pDataEnd = pCnf->pData + pCnf->count;
  pCnfClauseIterator->count = 0u;
}

int
cnf_clause_iterator_next(cnf_clause_iterator* pCnfClauseIterator)
{
  int32_t * pNextData = pCnfClauseIterator->pData + pCnfClauseIterator->count;
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

#endif

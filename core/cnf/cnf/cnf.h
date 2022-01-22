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

typedef struct cnf_clause_iterator
{
  const int32_t* pData;    /// position of the current clause
  const int32_t* pDataEnd; /// end of cnf
  size_t count;            /// count of literals in current clause
} cnf_clause_iterator;

void
cnf_clause_iterator_create(cnf_clause_iterator* pCnfClauseIterator, const cnf* pCnf);

int
cnf_clause_iterator_next(cnf_clause_iterator* pCnfClauseIterator);

#endif

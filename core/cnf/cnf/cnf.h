#ifndef CNF_CNF_H
#define CNF_CNF_H

#include <stdint.h> // int32_t
#include <stdlib.h> // size_t

typedef struct cnf
{
  int32_t * pData;
  size_t capacity;
  size_t count;
} cnf;

int cnf_create(cnf* pCnf);

int cnf_copy(cnf* pDest, const cnf* pSrc);

int cnf_pushClause(cnf* pCnf, const int32_t* pValues, size_t count);

void cnf_destroy(cnf* pCnf);

#endif

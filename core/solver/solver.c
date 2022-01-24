#include "solver/solver.h"

#include <common/common.h>

typedef struct ClauseBuffer
{
  int32_t* pData;
  size_t capacity;
  size_t count;
} ClauseBuffer;

static inline int
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

static inline int
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

static inline void
ClauseBuffer_destroy(ClauseBuffer* pClauseBuffer)
{
  SANITIZING_ASSERT(pClauseBuffer); // pClauseBuffer must be a valid pointer

  free(pClauseBuffer->pData);
  pClauseBuffer->pData = NULL;
  pClauseBuffer->capacity = 0u;
  pClauseBuffer->count = 0u;
}

static inline void
ClauseBuffer_reset(ClauseBuffer* pClauseBuffer)
{
  SANITIZING_ASSERT(pClauseBuffer); // pClauseBuffer must be a valid pointer
  pClauseBuffer->count = 0u;
}

int
Cnf_simplify(const Cnf* pCnf, const Assignment* pAssignment, Cnf* pNextCnf)
{
  SANITIZING_ASSERT(pCnf);        // pCnf must be a valid pointer
  SANITIZING_ASSERT(pAssignment); // pAssignment must be a valid pointer
  SANITIZING_ASSERT(pNextCnf);    // pNextCnf must be a valid pointer

  Cnf_ClauseIterator clauseIterator;
  Cnf_ClauseIterator_create(&clauseIterator, pCnf);

  ClauseBuffer clauseBuffer;
  if (ClauseBuffer_create(&clauseBuffer))
    return 1;

  while (Cnf_ClauseIterator_next(&clauseIterator)) {

    int8_t clauseTrue = 0u;

    for (size_t literalIndex = 0u; literalIndex < clauseIterator.count; ++literalIndex) {

      int32_t literal = clauseIterator.pData[literalIndex];
      uint32_t variable = literal < 0 ? -literal : literal;

      int8_t variableAssignment;
      if (Assignment_get(pAssignment, variable, &variableAssignment)) {
        // Assignment does not specify literal value
        ClauseBuffer_push(&clauseBuffer, literal);
        continue;
      }

      if (!(variableAssignment ^ (literal > 0))) {
        // Assignment makes the whole clause true
        ClauseBuffer_reset(&clauseBuffer);
        clauseTrue = 1u;
        break;
      }

      // Assignment makes the literal false
    }

    if (!clauseTrue) {
      Cnf_pushClause(pNextCnf, clauseBuffer.pData, clauseBuffer.count);
      ClauseBuffer_reset(&clauseBuffer);
    }
  }

  ClauseBuffer_destroy(&clauseBuffer);
  return 0;
}

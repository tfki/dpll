#include "solver/solver.h"

typedef struct clauseBuffer
{
  int32_t* pData;
  size_t capacity;
  size_t count;
} clauseBuffer;

static inline int
clauseBuffer_create(clauseBuffer* pClauseBuffer)
{
  pClauseBuffer->count = 0u;
  pClauseBuffer->capacity = 1024u;
  pClauseBuffer->pData = malloc(pClauseBuffer->capacity * sizeof(int32_t));

  if (!pClauseBuffer->pData)
    return 1;

  return 0;
}

static inline int
clauseBuffer_push(clauseBuffer* pClauseBuffer, int32_t literal)
{
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
clauseBuffer_destroy(clauseBuffer* pClauseBuffer)
{
  free(pClauseBuffer->pData);
  pClauseBuffer->pData = NULL;
  pClauseBuffer->capacity = 0u;
  pClauseBuffer->count = 0u;
}

static inline void
clauseBuffer_reset(clauseBuffer* pClauseBuffer)
{
  pClauseBuffer->count = 0u;
}

int
cnf_simplify(const cnf* pCnf, const assignment* pAssignment, cnf* pNextCnf)
{
  cnf_clause_iterator clauseIterator;
  cnf_clause_iterator_create(&clauseIterator, pCnf);

  clauseBuffer clauseBuffer;
  if (clauseBuffer_create(&clauseBuffer))
    return 1;

  while (!cnf_clause_iterator_next(&clauseIterator)) {

    int8_t clauseTrue = 0u;

    for (size_t literalIndex = 0u; literalIndex < clauseIterator.count; ++literalIndex) {

      int32_t literal = clauseIterator.pData[literalIndex];
      uint32_t variable = literal < 0 ? -literal : literal;

      int8_t variableAssignment;
      if (assignment_get(pAssignment, variable, &variableAssignment)) {
        // assignment does not specify literal value
        clauseBuffer_push(&clauseBuffer, literal);
        continue;
      }

      if (!(variableAssignment ^ (literal > 0))) {
        // assignment makes the whole clause true
        clauseBuffer_reset(&clauseBuffer);
        clauseTrue = 1u;
        break;
      }

      // assignment makes the literal false
    }

    if (!clauseTrue) {
      cnf_pushClause(pNextCnf, clauseBuffer.pData, clauseBuffer.count);
      clauseBuffer_reset(&clauseBuffer);
    }
  }

  clauseBuffer_destroy(&clauseBuffer);
  return 0;
}

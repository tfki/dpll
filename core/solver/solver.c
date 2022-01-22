#include "solver/solver.h"

typedef struct clause_buffer
{
  int32_t* pData;
  size_t count;
} clause_buffer;

static inline int
clause_buffer_create(clause_buffer* pClauseBuffer, size_t worstCaseCapacity)
{
  pClauseBuffer->pData = malloc(worstCaseCapacity * sizeof (int32_t));

  if (!pClauseBuffer->pData)
    return 1;

  pClauseBuffer->count = 0u;
  return 0;
}

static inline void
clause_buffer_push(clause_buffer* pClauseBuffer, int32_t literal)
{
  pClauseBuffer->pData[pClauseBuffer->count] = literal;
  ++pClauseBuffer->count;
}

static inline void
clause_buffer_reset(clause_buffer* pClauseBuffer)
{
  pClauseBuffer->count = 0u;
}

static inline void
clause_buffer_destroy(clause_buffer* pClauseBuffer)
{
  free(pClauseBuffer->pData);
  pClauseBuffer->pData = NULL;
  pClauseBuffer->count = 0u;
}

int
cnf_simplify(const cnf* pCnf, const assignment* pAssignment, cnf* pNextCnf)
{
  cnf_clause_iterator clauseIterator;
  cnf_clause_iterator_create(&clauseIterator, pCnf);

  // allocate worst case buffer
  clause_buffer clauseBuffer;
  if(clause_buffer_create(&clauseBuffer, pAssignment->count))
    return 1;

  while (!cnf_clause_iterator_next(&clauseIterator)) {
    for (size_t literalIndex = 0u; literalIndex < clauseIterator.count; ++literalIndex) {

      int32_t literal = clauseIterator.pData[literalIndex];
      uint32_t variable = literal < 0 ? -literal : literal;

      int8_t variableAssignment;
      assignment_get(pAssignment, variable, &variableAssignment);

      if (!(variableAssignment ^ (literal > 0)))
      {
        clause_buffer_reset(&clauseBuffer);
        break;
      }

      clause_buffer_push(&clauseBuffer, literal);
    }

    if (clauseBuffer.count)
    {
      cnf_pushClause(pNextCnf, clauseBuffer.pData, clauseBuffer.count);
      clause_buffer_reset(&clauseBuffer);
    }
  }

  clause_buffer_destroy(&clauseBuffer);
  return 0;
}

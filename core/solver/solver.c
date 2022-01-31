#include "solver/solver.h"

#include <common/sanitize.h>

int
Cnf_simplify(const Cnf* pCnf, const AssignmentStack* pAssignment, Cnf* pNextCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  SANITIZE_PARAMETER_POINTER(pAssignment);
  SANITIZE_PARAMETER_POINTER(pNextCnf);

  Cnf_ClauseIterator clauseIterator;
  Cnf_ClauseIterator_create(&clauseIterator, pCnf);

  ClauseBuffer clauseBuffer;
  if (ClauseBuffer_create(&clauseBuffer))
    return 1;

  while (Cnf_ClauseIterator_next(&clauseIterator)) {

    bool clauseTrue = 0u;

    for (size_t literalIndex = 0u; literalIndex < clauseIterator.count; ++literalIndex) {

      int32_t literal = clauseIterator.pData[literalIndex];
      uint32_t variable = literal < 0 ? -literal : literal;

      bool variableAssignment;
      if (AssignmentStack_get(pAssignment, variable, &variableAssignment)) {
        // AssignmentStack does not specify literal value
        ClauseBuffer_push(&clauseBuffer, literal);
        continue;
      }

      if (!(variableAssignment ^ (literal > 0))) {
        // AssignmentStack makes the whole clause true
        ClauseBuffer_reset(&clauseBuffer);
        clauseTrue = 1u;
        break;
      }

      // AssignmentStack makes the literal false
    }

    if (!clauseTrue) {
      Cnf_pushClause(pNextCnf, clauseBuffer.pData, clauseBuffer.count);
      ClauseBuffer_reset(&clauseBuffer);
    }
  }

  ClauseBuffer_destroy(&clauseBuffer);
  return 0;
}

int
Cnf_simplifyWithView(const Cnf* pCnf, const AssignmentStackView* pAssignmentView, Cnf* pNextCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  SANITIZE_PARAMETER_POINTER(pAssignmentView);
  SANITIZE_PARAMETER_POINTER(pNextCnf);

  Cnf_ClauseIterator clauseIterator;
  Cnf_ClauseIterator_create(&clauseIterator, pCnf);

  ClauseBuffer clauseBuffer;
  if (ClauseBuffer_create(&clauseBuffer))
    return 1;

  while (Cnf_ClauseIterator_next(&clauseIterator)) {

    bool clauseTrue = 0u;

    for (size_t literalIndex = 0u; literalIndex < clauseIterator.count; ++literalIndex) {

      int32_t literal = clauseIterator.pData[literalIndex];
      uint32_t variable = literal < 0 ? -literal : literal;

      bool variableAssignment;
      if (AssignmentStackView_get(pAssignmentView, variable, &variableAssignment)) {
        // AssignmentStack does not specify literal value
        ClauseBuffer_push(&clauseBuffer, literal);
        continue;
      }

      if (!(variableAssignment ^ (literal > 0))) {
        // AssignmentStack makes the whole clause true
        ClauseBuffer_reset(&clauseBuffer);
        clauseTrue = 1u;
        break;
      }

      // AssignmentStack makes the literal false
    }

    if (!clauseTrue) {
      Cnf_pushClause(pNextCnf, clauseBuffer.pData, clauseBuffer.count);
      ClauseBuffer_reset(&clauseBuffer);
    }
  }

  ClauseBuffer_destroy(&clauseBuffer);
  return 0;
}

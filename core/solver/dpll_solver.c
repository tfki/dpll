#include "solver/dpll_solver.h"

int32_t
dpllTrivialPick(const Cnf* pCnf)
{
  return pCnf->pData[1u];
}

int
dpllSolve(const Cnf* pCnf, int32_t (*pickAndRemove)(const Cnf*), Assignment* pAssignmentResult)
{
  Assignment emptyAssignment;
  Assignment_create(&emptyAssignment);

  int foundSolution = dpllSolvePartial(pCnf, &emptyAssignment, pickAndRemove, pAssignmentResult);
  Assignment_destroy(&emptyAssignment);

  return foundSolution;
}

int
dpllSolvePartial(const Cnf* pCnf, Assignment* pAssignment, int32_t (*pickAndRemove)(const Cnf*), Assignment* pAssignmentResult)
{
  // TODO we should not require pCnf to be const, so we can reset and reuse it.
  //      also we should pass simplified into dpllSolvePartial to reduce memory allocations!

  Cnf simplified;
  Cnf_create(&simplified);

  Cnf_simplify(pCnf, pAssignment, &simplified);

  // if Cnf is empty -> sat
  if (pCnf->count == 0u) {
    Assignment_swap(pAssignmentResult, pAssignment);
    Cnf_destroy(&simplified);
    return 0;
  }

  // if Cnf contains empty clause -> unsat
  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &simplified);

  while (Cnf_ClauseIterator_next(&iter)) {
    if (iter.count == 0) {
      Cnf_destroy(&simplified);
      return 1;
    }
  }

  int32_t nextLiteral = pickAndRemove(pCnf);
  uint32_t nextVariable = nextLiteral > 0 ? nextLiteral : -nextLiteral;

  // Assignment with nextLiteral evaluating to false
  Assignment pFalseAssignment;
  Assignment_copy(&pFalseAssignment, pAssignment);

  // for the Assignment where nextLiteral evaluates to false we just reuse pAssignment

  // set value of nextVariable in pAssignment, so that it evaluates to true
  Assignment_set(pAssignment, nextVariable, (nextLiteral > 0) ? 1 : 0);

  // set value of nextVariable in pFalseAssigment, so that it evaluates to false
  Assignment_set(&pFalseAssignment, nextVariable, (nextLiteral < 0) ? 1 : 0);

  if (!dpllSolvePartial(&simplified, pAssignment, pickAndRemove, pAssignmentResult)) {
    Cnf_destroy(&simplified);
    Assignment_destroy(&pFalseAssignment);
    return 0;
  }

  if (!dpllSolvePartial(&simplified, &pFalseAssignment, pickAndRemove, pAssignmentResult)) {
    Cnf_destroy(&simplified);
    Assignment_destroy(&pFalseAssignment);
    return 0;
  }

  Assignment_destroy(&pFalseAssignment);
  Cnf_destroy(&simplified);
  return 1;
}

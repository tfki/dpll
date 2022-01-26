#include "solver/dpll_solver.h"

#include <cnf/cnf.h>
#include <common/common.h>
#include <stdbool.h>

int32_t
dpllTrivialPick(const Cnf* pCnf)
{
  SANITIZING_ASSERT(pCnf); // pCnf must be a valid pointer
  return pCnf->pData[1u];
}

int
dpllSolve(const Cnf* pCnf, int32_t (*pickAndRemove)(const Cnf*), AssignmentStack* pAssignment)
{
  SANITIZING_ASSERT(pCnf);          // pCnf must be a valid pointer
  SANITIZING_ASSERT(pAssignment);   // pAssignment must be a valid pointer
  SANITIZING_ASSERT(pickAndRemove); // pickAndRemove must be a valid pointer

  // TODO we should not require pCnf to be const, so we can reset and reuse it.
  //      also we should pass simplified into dpllSolvePartial to reduce memory allocations!

  //  _____          _           _           _        _                                   _
  // |_   _|  _ __  (_) __   __ (_)   __ _  | |      / \      ___    ___    ___   _ __   | |_
  //   | |   | '__| | | \ \ / / | |  / _` | | |     / _ \    / __|  / __|  / _ \ | '_ \  | __|
  //   | |   | |    | |  \ V /  | | | (_| | | |    / ___ \  | (__  | (__  |  __/ | |_) | | |_
  //   |_|   |_|    |_|   \_/   |_|  \__,_| |_|   /_/   \_\  \___|  \___|  \___| | .__/   \__|
  //                                                                             |_|

  // if Cnf is empty -> sat
  Cnf simplified;
  if (Cnf_create(&simplified))
    return 1;
  if (Cnf_simplify(pCnf, pAssignment, &simplified)) {
    Cnf_destroy(&simplified);
    return 1;
  }

  if (simplified.count == 0u) {
    Cnf_destroy(&simplified);
    return 0;
  }

  //  dpllUnitPropagation(&simplified, pAssignment);

  if (simplified.count == 0u) {
    Cnf_destroy(&simplified);
    return 0;
  }

  //  _____          _           _           _     ____             _                 _
  // |_   _|  _ __  (_) __   __ (_)   __ _  | |   |  _ \    ___    (_)   ___    ___  | |_
  //   | |   | '__| | | \ \ / / | |  / _` | | |   | |_) |  / _ \   | |  / _ \  / __| | __|
  //   | |   | |    | |  \ V /  | | | (_| | | |   |  _ <  |  __/   | | |  __/ | (__  | |_
  //   |_|   |_|    |_|   \_/   |_|  \__,_| |_|   |_| \_\  \___|  _/ |  \___|  \___|  \__|
  //                                                             |__/

  // if Cnf contains empty clause -> unsat
  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &simplified);

  while (Cnf_ClauseIterator_next(&iter)) {
    if (iter.count == 0) {
      Cnf_destroy(&simplified);
      return 1;
    }
  }

  //   ____                                                  _   _                 _
  //  |  _ \   _ __    ___   _ __     __ _   _ __    ___    | \ | |   ___  __  __ | |_
  //  | |_) | | '__|  / _ \ | '_ \   / _` | | '__|  / _ \   |  \| |  / _ \ \ \/ / | __|
  //  |  __/  | |    |  __/ | |_) | | (_| | | |    |  __/   | |\  | |  __/  >  <  | |_
  //  |_|     |_|     \___| | .__/   \__,_| |_|     \___|   |_| \_|  \___| /_/\_\  \__|
  //                        |_|

  // Choose another variable and try to solve the
  // formula by setting it first to true and then to false.
  // The evaluation is done by a recursive call.

  const int32_t nextLiteral = pickAndRemove(&simplified);
  const uint32_t nextVariable = nextLiteral > 0 ? nextLiteral : -nextLiteral;
  const bool sign = (nextLiteral > 0) ? true : false;

  AssignmentStack_push(pAssignment, nextVariable, sign);
  if (!dpllSolve(&simplified, pickAndRemove, pAssignment)) {
    Cnf_destroy(&simplified);
    return 0;
  }

  AssignmentStack_pop(pAssignment);
  AssignmentStack_push(pAssignment, nextVariable, !sign);
  if (!dpllSolve(&simplified, pickAndRemove, pAssignment)) {
    Cnf_destroy(&simplified);
    return 0;
  }

  Cnf_destroy(&simplified);
  return 1;
}

int
dpllUnitPropagation(Cnf* cnf, AssignmentStack* assignmentStack)
{
  bool foundAtLeasOneUnitClause;

  Cnf simplified;
  if (Cnf_create(&simplified))
    return 1;

  do {
    AssignmentStackView assignmentView;
    AssignmentStackView_beginView(&assignmentView, assignmentStack);
    foundAtLeasOneUnitClause = false;

    for (size_t i = 0u; i + 2 < cnf->count; i++) {
      if (cnf->pData[i] == 0 && cnf->pData[i + 1] != 0 && cnf->pData[i + 2] == 0) {
        // unit clause found
        int32_t literal = cnf->pData[i + 1];
        uint32_t variable = literal > 0 ? literal : -literal;
        bool value = literal > 0;
        if (AssignmentStack_push(assignmentStack, variable, value)) {
          Cnf_destroy(&simplified);
          return 1;
        }

        foundAtLeasOneUnitClause = true;
      }
    }

    AssignmentStackView_endView(&assignmentView, assignmentStack);
    if (Cnf_simplifyWithView(cnf, &assignmentView, &simplified))
      return 1;

    Cnf_swap(cnf, &simplified);
    Cnf_reset(&simplified);
  } while (foundAtLeasOneUnitClause);

  return 0;
}

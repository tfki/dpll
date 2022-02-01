#include "solver/dpll_solver.h"

#include <common/sanitize.h>
#include <common/debugbreak.h>

#include <stdbool.h>

int32_t
dpllTrivialPick(const Cnf* pCnf)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  return pCnf->pData[1u];
}

int
dpllSolve(const Cnf* pCnf, int32_t (*pickAndRemove)(const Cnf*), AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  SANITIZE_PARAMETER_POINTER(pAssignment);
  SANITIZE_PARAMETER_POINTER(pickAndRemove);

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
  if (Cnf_create(&simplified)) {
    return 1;
  }
  if (Cnf_simplify(pCnf, pAssignment, &simplified)) {
    Cnf_destroy(&simplified);
    return 1;
  }

  if (simplified.count == 0u) {
    Cnf_destroy(&simplified);
    return 0;
  }

  AssignmentStackState assignmentStateBeforeUP;
  AssignmentStack_storeState(pAssignment, &assignmentStateBeforeUP);

  dpllUnitPropagation(&simplified, pAssignment);
  dpllPureLiteralElimination(&simplified, pAssignment);

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

  AssignmentStackState assignmentStateBeforeVariablePick;
  AssignmentStack_storeState(pAssignment, &assignmentStateBeforeVariablePick);

  AssignmentStack_push(pAssignment, nextVariable, sign);
  if (!dpllSolve(&simplified, pickAndRemove, pAssignment)) {
    Cnf_destroy(&simplified);
    return 0;
  }

  AssignmentStack_restoreState(pAssignment, &assignmentStateBeforeVariablePick);

  AssignmentStack_push(pAssignment, nextVariable, !sign);
  if (!dpllSolve(&simplified, pickAndRemove, pAssignment)) {
    Cnf_destroy(&simplified);
    return 0;
  }

  AssignmentStack_restoreState(pAssignment, &assignmentStateBeforeUP);
  Cnf_destroy(&simplified);
  return 1;
}

int
dpllUnitPropagation(Cnf* pCnf, AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  SANITIZE_PARAMETER_POINTER(pAssignment);

  bool foundAtLeasOneUnitClause;

  Cnf simplified;
  if (Cnf_create(&simplified)) {
    return 1;
  }

  do {
    AssignmentStackView assignmentView;
    AssignmentStackView_beginView(&assignmentView, pAssignment);
    foundAtLeasOneUnitClause = false;

    for (size_t i = 0u; i + 2 < pCnf->count; i++) {
      if (pCnf->pData[i] == 0 && pCnf->pData[i + 1] != 0 && pCnf->pData[i + 2] == 0) {
        // unit clause found
        int32_t literal = pCnf->pData[i + 1];
        uint32_t variable = literal > 0 ? literal : -literal;
        bool value = literal > 0;
        if (AssignmentStack_push(pAssignment, variable, value)) {
          Cnf_destroy(&simplified);
          return 1;
        }

        foundAtLeasOneUnitClause = true;
      }
    }

    AssignmentStackView_endView(&assignmentView, pAssignment);
    if (Cnf_simplifyWithView(pCnf, &assignmentView, &simplified)) {
      Cnf_destroy(&simplified);
      return 1;
    }

    Cnf_swap(pCnf, &simplified);
    Cnf_reset(&simplified);
  } while (foundAtLeasOneUnitClause);

  Cnf_destroy(&simplified);
  return 0;
}

typedef enum PureValue
{
  PURE_POS = 0b01,
  PURE_NEG = 0b10,
  NON_PURE = 0b11,
} PureValue;

typedef struct PureLiteralStack
{
  uint32_t* pKeys;
  PureValue* pValues;
  size_t capacity;
  size_t count;
} PureLiteralStack;

int
PureLiteralStack_create(PureLiteralStack* pLiteralStack)
{
  SANITIZE_PARAMETER_POINTER(pLiteralStack);

  pLiteralStack->count = 0u;
  pLiteralStack->capacity = 1024u;

  const size_t keysSize = pLiteralStack->capacity * sizeof(*pLiteralStack->pKeys);
  const size_t valuesSize = pLiteralStack->capacity * sizeof(*pLiteralStack->pValues);
  pLiteralStack->pKeys = malloc(keysSize + valuesSize);

  if (!pLiteralStack->pKeys)
    return 1;

  pLiteralStack->pValues = (PureValue*)(&pLiteralStack->pKeys[pLiteralStack->capacity]);

  return 0;
}

void
PureLiteralStack_destroy(PureLiteralStack* pLiteralStack)
{
  SANITIZE_PARAMETER_POINTER(pLiteralStack);
  free(pLiteralStack->pKeys);
  pLiteralStack->pKeys = NULL;
  pLiteralStack->pValues = NULL;
  pLiteralStack->capacity = 0u;
  pLiteralStack->count = 0u;
}

int
PureLiteralStack_push(PureLiteralStack* pLiteralStack, uint32_t key, PureValue value)
{
  SANITIZE_PARAMETER_POINTER(pLiteralStack);
  SANITIZE_PARAMETER_POINTER(key);

  for (size_t i = 0u; i < pLiteralStack->count; ++i) {
    if (pLiteralStack->pKeys[i] == key) {
      pLiteralStack->pValues[i] |= value;
      return 0;
    }
  }

  if (pLiteralStack->count + 1 > pLiteralStack->capacity) {

    const size_t newCapacity = pLiteralStack->capacity << 1;
    const size_t newKeysSize = newCapacity * sizeof(*pLiteralStack->pKeys);
    const size_t newValuesSize = newCapacity * sizeof(*pLiteralStack->pValues);

    uint32_t* pNewKeys = realloc(pLiteralStack->pKeys, newKeysSize + newValuesSize);
    if (!pNewKeys)
      return 1;

    pLiteralStack->pKeys = pNewKeys;
    pLiteralStack->pValues = (PureValue*)(pLiteralStack->pKeys + newCapacity);
    pLiteralStack->capacity = newCapacity;
  }

  // insert key
  pLiteralStack->pKeys[pLiteralStack->count] = key;
  pLiteralStack->pValues[pLiteralStack->count] = value;
  ++pLiteralStack->count;

  return 0;
}

void
PureLiteralStack_purify(PureLiteralStack* pLiteralStack)
{
  for (size_t i = 0u; i < pLiteralStack->count;) {
    if (pLiteralStack->pValues[i] == NON_PURE) {
      pLiteralStack->pKeys[i] = pLiteralStack->pKeys[pLiteralStack->count];
      pLiteralStack->pValues[i] = pLiteralStack->pValues[pLiteralStack->count];
      --(pLiteralStack->count);
      continue;
    }
    ++i;
  }
}

int
dpllPureLiteralElimination(Cnf* pCnf, AssignmentStack* pAssignment)
{
  SANITIZE_PARAMETER_POINTER(pCnf);
  SANITIZE_PARAMETER_POINTER(pAssignment);

  AssignmentStackView assignmentView;
  AssignmentStackView_beginView(&assignmentView, pAssignment);

  Cnf_ClauseIterator clauseIterator;
  Cnf_ClauseIterator_create(&clauseIterator, pCnf);

  PureLiteralStack literalStack;
  PureLiteralStack_create(&literalStack);

  // iterate all literals
  while (Cnf_ClauseIterator_next(&clauseIterator)) {
    for (size_t literalIndex = 0u; literalIndex < clauseIterator.count; ++literalIndex) {

      // get the literal, its variable and value
      int32_t literal = clauseIterator.pData[literalIndex];
      uint32_t variable;
      PureValue value;

      if (literal < 0) {
        variable = -literal;
        value = PURE_NEG;
      } else {
        variable = literal;
        value = PURE_POS;
      }

      // push the variable into the pure literal stack
      if (PureLiteralStack_push(&literalStack, variable, value)) {
        PureLiteralStack_destroy(&literalStack);
        return 1;
      }
    }
  }
  // remove all variables from the stack that where pushed with both
  // a negative and a positive value
  PureLiteralStack_purify(&literalStack);

  // add all pure key value pairs to the assignment and
  // end assignment stack view (which then contains all new assignments)
  for (size_t i = 0u; i < literalStack.count; ++i) {
    if (AssignmentStack_push(pAssignment, literalStack.pKeys[i], literalStack.pValues[i] == PURE_POS)) {
      PureLiteralStack_destroy(&literalStack);
      return 1;
    }
  }
  AssignmentStackView_endView(&assignmentView, pAssignment);

  // the pure literal stack is not needed anymore
  // the important stuff is in the assignment stack view
  PureLiteralStack_destroy(&literalStack);

  // invoking simplify is necessary to ensure correctness
  // we should find a better than to create a temporary cnf object
  Cnf simplified;
  if (Cnf_create(&simplified)) {
    PureLiteralStack_destroy(&literalStack);
    return 1;
  }
  if (Cnf_simplifyWithView(pCnf, &assignmentView, &simplified)) {
    PureLiteralStack_destroy(&literalStack);
    return 1;
  }
  Cnf_swap(&simplified, pCnf);
  Cnf_destroy(&simplified);

  return 0;
}

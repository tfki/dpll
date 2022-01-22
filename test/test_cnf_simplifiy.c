#include <cnf/cnf.h>
#include <solver/solver.h>
#include <assert.h>

void
test_cnf_simplify_with_empty_assignment() {
  assignment emptyAssignment;
  assignment_create(&emptyAssignment);

  int32_t clause1[] = {1, -2, 3};
  size_t clause1Lenght = 3;
  int32_t clause2[] = {5, 3, -1};
  size_t clause2Lenght = 3;
  int32_t clause3[] = {4, 2, -3};
  size_t clause3Lenght = 3;


  cnf formula;
  cnf_create(&formula);
  cnf_pushClause(&formula, clause1, clause1Lenght);
  cnf_pushClause(&formula, clause2, clause2Lenght);
  cnf_pushClause(&formula, clause3, clause3Lenght);

  cnf resultFormula;
  cnf_create(&resultFormula);

  cnf_simplify(&formula, &emptyAssignment, &resultFormula);

  assert(formula.count == resultFormula.count);

  for (int i = 0; i < formula.count; ++i) {
    assert(formula.pData[i] == resultFormula.pData[i]);
  }
}

void
test_cnf_simplify_with_true_clause() {
  assignment assignment;
  assignment_create(&assignment);
  assignment_set(&assignment, 1, 1);

  int32_t clause1[] = {1, -2, 3};
  size_t clause1Length = 3;

  cnf formula;
  cnf_create(&formula);
  cnf_pushClause(&formula, clause1, clause1Length);

  cnf resultFormula;
  cnf_create(&resultFormula);

  cnf_simplify(&formula, &assignment, &resultFormula);

  assert(resultFormula.count == 0);
}

void
test_cnf_simplify_with_false_clause() {
  assignment assignment;
  assignment_create(&assignment);
  assignment_set(&assignment, 1, 0);

  int32_t clause1[] = {1, -2, 3};
  size_t clause1Length = 3;

  cnf formula;
  cnf_create(&formula);
  cnf_pushClause(&formula, clause1, clause1Length);

  cnf resultFormula;
  cnf_create(&resultFormula);

  cnf_simplify(&formula, &assignment, &resultFormula);

  assert(resultFormula.count == (formula.count - 1));
}

void
test_cnf_simplify_with_mixed_clauses() {
  assignment assignment;
  assignment_create(&assignment);
  assignment_set(&assignment, 1, 0);
  assignment_set(&assignment, 5, 1);

  // this clause should have the 1 omitted
  int32_t clause1[] = {1, -2, 3};
  size_t clause1Length = 3;

  // this clause should be absent from the result
  int32_t clause2[] = {3, -4, 5};
  size_t clause2Length = 3;

  cnf formula;
  cnf_create(&formula);
  cnf_pushClause(&formula, clause1, clause1Length);
  cnf_pushClause(&formula, clause2, clause2Length);

  cnf resultFormula;
  cnf_create(&resultFormula);

  cnf_simplify(&formula, &assignment, &resultFormula);

  assert(resultFormula.count == 4);
  assert(resultFormula.pData[0] == 0);
  assert(resultFormula.pData[1] == -2);
  assert(resultFormula.pData[2] == 3);
  assert(resultFormula.pData[0] == 0);
}

int
main() {
  test_cnf_simplify_with_empty_assignment();
  test_cnf_simplify_with_true_clause();
  test_cnf_simplify_with_false_clause();
  test_cnf_simplify_with_mixed_clauses();
}

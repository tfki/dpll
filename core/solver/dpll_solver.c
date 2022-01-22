
#include "solver/dpll_solver.h"


int dpll_solve(const cnf *pCnf, int32_t (*pickAndRemove)(const cnf *), assignment *pAssignmentResult) {
    assignment emptyAssignment;
    assignment_create(&emptyAssignment);

    return dpll_solve_partial(pCnf, &emptyAssignment, pickAndRemove, pAssignmentResult);
}

int
dpll_solve_partial(const cnf *pCnf, assignment *pAssignment, int32_t (*pickAndRemove)(const cnf *), assignment *pAssignmentResult) {
    cnf simplified;
    cnf_create(&simplified);

    cnf_simplify(pCnf, pAssignment, &simplified);

    // if cnf is empty -> sat
    if (pCnf->count == 0u) {
        *pAssignmentResult = *pAssignment;
        return 0;
    }

    // if cnf contains empty clause -> unsat
    cnf_clause_iterator iter;
    cnf_clause_iterator_create(&iter, &simplified);

    while (!cnf_clause_iterator_next(&iter)) {
        if (iter.count == 0){
            return 1;
        }
    }

    int32_t nextLiteral = pickAndRemove(pCnf);
    uint32_t nextVariable = nextLiteral > 0 ? nextLiteral : -nextLiteral;

    // assignment with nextLiteral evaluating to false
    assignment pFalseAssigment;
    assignment_copy(&pFalseAssigment, pAssignment);

    // for the assignment where nextLiteral evaluates to false we just reuse pAssignment

    // set value of nextVariable in pAssignment, so that it evaluates to true
    assignment_set(pAssignment, nextVariable, (nextLiteral > 0) ? 1 : 0);

    // set value of nextVariable in pFalseAssigment, so that it evaluates to false
    assignment_set(&pFalseAssigment, nextVariable, (nextLiteral < 0) ? 1 : 0);

    if (!dpll_solve_partial(&simplified, pAssignment, pickAndRemove, pAssignmentResult)){
        return 0;
    }

    if (!dpll_solve_partial(pCnf, &pFalseAssigment, pickAndRemove, pAssignmentResult)){
        return 0;
    }

    return 1;
}

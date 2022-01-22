#ifndef DPLLSOLVER_DPLL_SOLVER_H
#define DPLLSOLVER_DPLL_SOLVER_H

#include "solver.h"
#include "solver/assignment.h"

#include <cnf/cnf.h>

int dpll_solve(const cnf* pCnf, int32_t (*pickAndRemove)(const cnf *), assignment* pAssignmentResult);

int dpll_solve_partial(const cnf *pCnf, assignment *pAssignment, int32_t (*pickAndRemove)(const cnf *), assignment *pAssignmentResult);

#endif

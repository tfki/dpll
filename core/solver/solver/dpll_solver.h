
#ifndef DPLLSOLVER_DPLL_SOLVER_H
#define DPLLSOLVER_DPLL_SOLVER_H

#include "solver.h"
#include "solver/assignment.h"

#include <cnf/cnf.h>

int dpll_solve(cnf* pCnf, int32_t (*pickAndRemove)(cnf *), assignment* pAssignmentResult);

int dpll_solve_partial(cnf *pCnf, assignment *pAssignment, int32_t (*pickAndRemove)(cnf *), assignment *pAssignmentResult);

#endif

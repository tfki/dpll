#ifndef DPLLSOLVER_SOLVER_H
#define DPLLSOLVER_SOLVER_H

#include "assignment.h"
#include "dpll_solver.h"

#include <cnf/cnf.h>

int
cnf_simplify(const cnf* pCnf, const assignment* pAssignment, cnf* pNextCnf);

#endif

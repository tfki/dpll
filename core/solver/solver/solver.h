#ifndef SOLVER_SOLVER_H
#define SOLVER_SOLVER_H

#include "assignment.h"
#include "dpll_solver.h"
#include <cnf/cnf.h>

int
Cnf_simplify(const Cnf* pCnf, const Assignment* pAssignment, Cnf* pNextCnf);

#endif

#ifndef DPLLSOLVER_SOLVER_H
#define DPLLSOLVER_SOLVER_H

#include "assignmentstack.h"
#include "dpll_solver.h"
#include <cnf/cnf.h>

int
Cnf_simplify(const Cnf* pCnf, const AssignmentStack* pAssignment, Cnf* pNextCnf);

int
Cnf_simplifyWithView(const Cnf* pCnf, const AssignmentStackView* pAssignmentView, Cnf* pNextCnf);

#endif

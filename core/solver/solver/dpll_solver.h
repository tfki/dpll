#ifndef DPLLSOLVER_DPLL_SOLVER_H
#define DPLLSOLVER_DPLL_SOLVER_H

#include "assignmentstack.h"
#include "solver.h"
#include <cnf/cnf.h>

int32_t
dpllTrivialPick(const Cnf* pCnf);

int
dpllSolve(const Cnf* pCnf, int32_t (*pickAndRemove)(const Cnf*), AssignmentStack* pAssignment);

int
dpllUnitPropagation(Cnf* pCnf, AssignmentStack* pAssignment);

#endif

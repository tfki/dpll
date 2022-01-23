#ifndef SOLVER_DPLLSOLVER_H
#define SOLVER_DPLLSOLVER_H

#include "solver.h"
#include "assignment.h"
#include <cnf/cnf.h>

int32_t
dpllTrivialPick(const Cnf* pCnf);

int
dpllSolve(const Cnf* pCnf, int32_t (*pickAndRemove)(const Cnf*), Assignment* pAssignmentResult);

int
dpllSolvePartial(const Cnf* pCnf, Assignment* pAssignment, int32_t (*pickAndRemove)(const Cnf*), Assignment* pAssignmentResult);

#endif

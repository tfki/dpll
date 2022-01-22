#ifndef DPLLSOLVER_SOLVER_H
#define DPLLSOLVER_SOLVER_H

#include <cnf/cnf.h>
#include "assignment.h"

typedef struct solver solver;

int
cnf_simplify(const cnf* pCnf, const assignment* pAssignment, cnf* pNextCnf);

#endif

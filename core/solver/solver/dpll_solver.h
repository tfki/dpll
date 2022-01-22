
#ifndef DPLLSOLVER_DPLL_SOLVER_H
#define DPLLSOLVER_DPLL_SOLVER_H

#include "solver.h"

#include <cnf/cnf.h>

void dpll_solve(solver* solver, cnf* cnf);

#endif

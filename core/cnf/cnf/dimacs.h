#ifndef DPLLSOLVER_DIMACS_H
#define DPLLSOLVER_DIMACS_H

#include "cnf/cnf.h"
#include <stdbool.h>

int
parseDimacs(char* dimacs, Cnf* result);

#endif

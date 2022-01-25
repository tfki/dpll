DPLL Solver
===========

Return Type Conventions
-----------------------

Functions returning void a guaranteed to not fail.
Functions returning int must be expected to fail.
Functions returning bool are used to support a better flow in conditions/loops. (E.g. iterators)
Results are always written using out variable pointers which are placed at the end of the parameter list.
Non out variable pointers are marked as const.

TODO
----

+ memory leaks in simplify tests
+ replace clause iterator with literal iterator
+ simplify for single variable instead of whole assignment
+ complete solver tests
+ complete benchmark code
+ discuss status type
+ Assignment_setAll has room for optimization
+ get return type of dpllsolve sorted out
+ all tests should be static void
+ solve pickAndRemove Callback does not follow the return conventions

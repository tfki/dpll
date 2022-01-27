DPLL Solver
===========

Return Type Conventions
-----------------------

Functions returning void a guaranteed to not fail. Functions returning int must be expected to fail. Functions returning
bool are used to support a better flow in conditions/loops. (E.g. iterators)
Results are always written using out variable pointers which are placed at the end of the parameter list. Non out
variable pointers are marked as const.

TODO
----

+ replace clause iterator with literal iterator
+ simplify for single variable instead of whole assignment
+ complete benchmark code
+ Assignment_setAll has room for optimization
+ get return type of dpllsolve sorted out
+ solve pickAndRemove Callback does not follow the return conventions
+ iterate assignmentstack get backwards if there is a use case

+ for all methods that allocate a parameter should use an allocator passed as parameter
  - our allocator could look similar the vulkan allocator 

#include "solver/assignmentstack.h"

int
main()
{
  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  AssignmentStack_set(&assignment, 3, 1);
  AssignmentStack_set(&assignment, 5, 1);
  AssignmentStack_set(&assignment, 2, 0);
  AssignmentStack_set(&assignment, 1, 1);

  bool val3;
  AssignmentStack_get(&assignment, 3, &val3);

  bool val5;
  AssignmentStack_get(&assignment, 5, &val5);

  bool val2;
  AssignmentStack_get(&assignment, 2, &val2);

  AssignmentStack_destroy(&assignment);
  return 0;
}

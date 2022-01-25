#include "solver/assignmentstack.h"

int
main()
{
  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  AssignmentStack_push(&assignment, 3, 1);
  AssignmentStack_push(&assignment, 5, 1);
  AssignmentStack_push(&assignment, 2, 0);
  AssignmentStack_push(&assignment, 1, 1);

  bool val3;
  AssignmentStack_get(&assignment, 3, &val3);

  bool val5;
  AssignmentStack_get(&assignment, 5, &val5);

  bool val2;
  AssignmentStack_get(&assignment, 2, &val2);

  AssignmentStack_destroy(&assignment);
  return 0;
}

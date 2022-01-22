
#include "solver/assignment.h"

int
main()
{
  assignment a;
  assignment_create(&a);

  assignment_set(&a, 3, 1);
  assignment_set(&a, 5, 1);
  assignment_set(&a, 2, 0);
  assignment_set(&a, 1, 1);

  int8_t val3;
  assignment_get(&a, 3, &val3);

  int8_t val5;
  assignment_get(&a, 5, &val5);

  int8_t val2;
  assignment_get(&a, 2, &val2);

  assignment_destroy(&a);
  return 0;
}

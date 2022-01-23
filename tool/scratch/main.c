#include "solver/assignment.h"

int
main()
{
  Assignment assignment;
  Assignment_create(&assignment);

  Assignment_set(&assignment, 3, 1);
  Assignment_set(&assignment, 5, 1);
  Assignment_set(&assignment, 2, 0);
  Assignment_set(&assignment, 1, 1);

  int8_t val3;
  Assignment_get(&assignment, 3, &val3);

  int8_t val5;
  Assignment_get(&assignment, 5, &val5);

  int8_t val2;
  Assignment_get(&assignment, 2, &val2);

  Assignment_destroy(&assignment);
  return 0;
}

#include <solver/assignment.h>
#include <assert.h>

static void
test_assignment_create()
{
  assignment testAssignment;
  assert(!assignment_create(&testAssignment));
  assert(testAssignment.count == 0u);
  assert(testAssignment.capacity == 1024u);
  assert(testAssignment.pKeys != NULL);
  assert(testAssignment.pValues != NULL);

  assignment_destroy(&testAssignment);
}

static void
test_assignment_get_set()
{
  assignment testAssignment;
  assert(!assignment_create(&testAssignment));

  assert(!assignment_set(&testAssignment, 1u, 1));
  assert(!assignment_set(&testAssignment, 2u, 1));
  assert(!assignment_set(&testAssignment, 3u, 0));
  assert(!assignment_set(&testAssignment, 4u, 0));

  assert(testAssignment.count == 4);
  assert(testAssignment.capacity == 1024);

  int8_t value;
  assignment_get(&testAssignment, 1u, &value);
  assert(value == 1);

  assignment_get(&testAssignment, 2u, &value);
  assert(value == 1);

  assignment_get(&testAssignment, 3u, &value);
  assert(value == 0);

  assignment_get(&testAssignment, 4u, &value);
  assert(value == 0);

  assignment_destroy(&testAssignment);
}

static void
test_assignment_copy()
{
  assignment testAssignment1;
  assert(!assignment_create(&testAssignment1));

  assert(!assignment_set(&testAssignment1, 1u, 1));
  assert(!assignment_set(&testAssignment1, 2u, 1));
  assert(!assignment_set(&testAssignment1, 3u, 0));
  assert(!assignment_set(&testAssignment1, 4u, 0));

  assert(testAssignment1.count == 4);
  assert(testAssignment1.capacity == 1024);

  assignment testAssignment2;
  assignment_copy(&testAssignment2, &testAssignment1);

  for (size_t i = 0u; i < testAssignment1.count; ++i) {
    int8_t value1;
    int8_t value2;
    assignment_get(&testAssignment1, i, &value1);
    assignment_get(&testAssignment2, i, &value2);
    assert(value1 == value2);
  }

  assignment_destroy(&testAssignment1);
  assignment_destroy(&testAssignment2);
}

static void
test_assignment_destroy(){
  assignment testAssignment1;
  assert(!assignment_create(&testAssignment1));

  assert(!assignment_set(&testAssignment1, 1u, 1));
  assert(!assignment_set(&testAssignment1, 2u, 1));
  assert(!assignment_set(&testAssignment1, 3u, 0));
  assert(!assignment_set(&testAssignment1, 4u, 0));

  assignment_destroy(&testAssignment1);

  assert(testAssignment1.count == 0u);
  assert(testAssignment1.capacity == 0u);
  assert(testAssignment1.pKeys == NULL);
  assert(testAssignment1.pValues == NULL);
}

int
main()
{
  test_assignment_create();
  test_assignment_get_set();
  test_assignment_copy();
  test_assignment_destroy();

  return 0;
}

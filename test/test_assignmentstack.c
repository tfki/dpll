#include "test_common.h"

#include <solver/assignmentstack.h>

static void
test_AssignmentStack_create()
{
  AssignmentStack assignment;
  TEST_ASSERT(!AssignmentStack_create(&assignment));
  TEST_ASSERT(assignment.count == 0u);
  TEST_ASSERT(assignment.capacity == 1024u);
  TEST_ASSERT(assignment.pKeys != NULL);
  TEST_ASSERT(assignment.pValues != NULL);

  AssignmentStack_destroy(&assignment);
}

static void
test_AssignmentStack_getAndSet()
{
  AssignmentStack assignment;
  TEST_ASSERT(!AssignmentStack_create(&assignment));

  TEST_ASSERT(!AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 2u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 3u, false));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 4u, false));

  TEST_ASSERT(assignment.count == 4);
  TEST_ASSERT(assignment.capacity == 1024);

  bool value;
  AssignmentStack_get(&assignment, 1u, &value);
  TEST_ASSERT(value);

  AssignmentStack_get(&assignment, 2u, &value);
  TEST_ASSERT(value);

  AssignmentStack_get(&assignment, 3u, &value);
  TEST_ASSERT(!value);

  AssignmentStack_get(&assignment, 4u, &value);
  TEST_ASSERT(!value);

  AssignmentStack_destroy(&assignment);
}

static void
test_AssignmentStack_copy()
{
  uint32_t keys[] = { 1u, 2u, 3u, 4u };
  bool values[] = { true, true, false, false };

  AssignmentStack assignment1;
  TEST_ASSERT(!AssignmentStack_create(&assignment1));

  TEST_ASSERT(!AssignmentStack_push(&assignment1, keys[0u], values[0u]));
  TEST_ASSERT(!AssignmentStack_push(&assignment1, keys[1u], values[1u]));
  TEST_ASSERT(!AssignmentStack_push(&assignment1, keys[2u], values[2u]));
  TEST_ASSERT(!AssignmentStack_push(&assignment1, keys[3u], values[3u]));

  TEST_ASSERT(assignment1.count == 4u);
  TEST_ASSERT(assignment1.capacity == 1024u);

  AssignmentStack assignment2;
  AssignmentStack_copy(&assignment2, &assignment1);

  for (size_t i = 0u; i < assignment1.count; ++i) {
    bool value1;
    bool value2;
    AssignmentStack_get(&assignment1, keys[i], &value1);
    AssignmentStack_get(&assignment2, keys[i], &value2);
    TEST_ASSERT(values[i] == value1);
    TEST_ASSERT(values[i] == value2);
  }

  AssignmentStack_destroy(&assignment1);
  AssignmentStack_destroy(&assignment2);
}

static void
test_AssignmentStack_destroy()
{
  AssignmentStack assignment;
  TEST_ASSERT(!AssignmentStack_create(&assignment));

  TEST_ASSERT(!AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 2u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 3u, false));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 4u, false));

  AssignmentStack_destroy(&assignment);

  TEST_ASSERT(assignment.count == 0u);
  TEST_ASSERT(assignment.capacity == 0u);
  TEST_ASSERT(assignment.pKeys == NULL);
  TEST_ASSERT(assignment.pValues == NULL);
}

void
test_AssignmentStack_swap()
{
  AssignmentStack a;
  TEST_ASSERT(!AssignmentStack_create(&a));

  AssignmentStack b;
  TEST_ASSERT(!AssignmentStack_create(&b));

  TEST_ASSERT(!AssignmentStack_push(&a, 1u, false));
  TEST_ASSERT(!AssignmentStack_push(&a, 3u, true));

  TEST_ASSERT(!AssignmentStack_push(&b, 2u, true));
  TEST_ASSERT(!AssignmentStack_push(&b, 4u, false));
  TEST_ASSERT(!AssignmentStack_push(&b, 6u, true));

  AssignmentStack_swap(&a, &b);

  TEST_ASSERT(a.count == 3);
  TEST_ASSERT(a.pKeys[0] == 2u);
  TEST_ASSERT(a.pKeys[1] == 4u);
  TEST_ASSERT(a.pKeys[2] == 6u);
  TEST_ASSERT(a.pValues[0] == true);
  TEST_ASSERT(a.pValues[1] == false);
  TEST_ASSERT(a.pValues[2] == true);

  TEST_ASSERT(b.count == 2);
  TEST_ASSERT(b.pKeys[0] == 1u);
  TEST_ASSERT(b.pKeys[1] == 3u);
  TEST_ASSERT(b.pValues[0] == false);
  TEST_ASSERT(b.pValues[1] == true);

  AssignmentStack_destroy(&b);
  AssignmentStack_destroy(&a);
}

int
main()
{
  test_AssignmentStack_create();
  test_AssignmentStack_getAndSet();
  test_AssignmentStack_copy();
  test_AssignmentStack_destroy();
  test_AssignmentStack_swap();

  return 0;
}

#include "test_common.h"

#include <solver/assignmentstack.h>

static void
test_AssignmentStack_create()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT_EQ(assignment.count, 0u);
  TEST_ASSERT_EQ(assignment.capacity, 1024u);
  TEST_ASSERT_NON_NULL(assignment.pKeys);
  TEST_ASSERT_NON_NULL(assignment.pValues);

  AssignmentStack_destroy(&assignment);
}

static void
test_AssignmentStack_getAndSet()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));

  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 2u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 3u, false));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 4u, false));

  TEST_ASSERT_EQ(assignment.count, 4u);
  TEST_ASSERT_EQ(assignment.capacity, 1024u);

  bool value;
  TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment, 1u, &value));
  TEST_ASSERT_TRUE(value);

  TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment, 2u, &value));
  TEST_ASSERT_TRUE(value);

  TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment, 3u, &value));
  TEST_ASSERT_FALSE(value);

  TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment, 4u, &value));
  TEST_ASSERT_FALSE(value);

  AssignmentStack_destroy(&assignment);
}

static void
test_AssignmentStack_copy()
{
  uint32_t keys[] = { 1u, 2u, 3u, 4u };
  bool values[] = { true, true, false, false };

  AssignmentStack assignment1;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment1));

  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment1, keys[0u], values[0u]));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment1, keys[1u], values[1u]));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment1, keys[2u], values[2u]));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment1, keys[3u], values[3u]));

  TEST_ASSERT_EQ(assignment1.count, 4u);
  TEST_ASSERT_EQ(assignment1.capacity, 1024u);

  AssignmentStack assignment2;
  TEST_ASSERT_SUCCESS(AssignmentStack_copy(&assignment2, &assignment1));

  for (size_t i = 0u; i < assignment1.count; ++i) {
    bool value1, value2;
    TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment1, keys[i], &value1));
    TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment2, keys[i], &value2));
    TEST_ASSERT_EQ(values[i], value1);
    TEST_ASSERT_EQ(values[i], value2);
  }

  AssignmentStack_destroy(&assignment1);
  AssignmentStack_destroy(&assignment2);
}

static void
test_AssignmentStack_destroy()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));

  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 2u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 3u, false));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 4u, false));

  AssignmentStack_destroy(&assignment);

  TEST_ASSERT_EQ(assignment.count, 0u);
  TEST_ASSERT_EQ(assignment.capacity, 0u);
  TEST_ASSERT_NULL(assignment.pKeys);
  TEST_ASSERT_NULL(assignment.pValues);
}

void
test_AssignmentStack_swap()
{
  AssignmentStack a;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&a));

  AssignmentStack b;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&b));

  TEST_ASSERT_SUCCESS(AssignmentStack_push(&a, 1u, false));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&a, 3u, true));

  TEST_ASSERT_SUCCESS(AssignmentStack_push(&b, 2u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&b, 4u, false));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&b, 6u, true));

  AssignmentStack_swap(&a, &b);

  TEST_ASSERT_EQ(a.count, 3);
  TEST_ASSERT_EQ(a.pKeys[0], 2u);
  TEST_ASSERT_EQ(a.pKeys[1], 4u);
  TEST_ASSERT_EQ(a.pKeys[2], 6u);
  TEST_ASSERT_TRUE(a.pValues[0]);
  TEST_ASSERT_FALSE(a.pValues[1]);
  TEST_ASSERT_TRUE(a.pValues[2]);

  TEST_ASSERT_EQ(b.count, 2);
  TEST_ASSERT_EQ(b.pKeys[0], 1u);
  TEST_ASSERT_EQ(b.pKeys[1], 3u);
  TEST_ASSERT_FALSE(b.pValues[0]);
  TEST_ASSERT_TRUE(b.pValues[1]);

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
}

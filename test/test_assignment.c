#include "test_common.h"

#include <solver/assignment.h>

static void
test_Assignment_create()
{
  Assignment assignment;
  TEST_ASSERT(!Assignment_create(&assignment));
  TEST_ASSERT(assignment.count == 0u);
  TEST_ASSERT(assignment.capacity == 1024u);
  TEST_ASSERT(assignment.pKeys != NULL);
  TEST_ASSERT(assignment.pValues != NULL);

  Assignment_destroy(&assignment);
}

static void
test_Assignment_getAndSet()
{
  Assignment assignment;
  TEST_ASSERT(!Assignment_create(&assignment));

  TEST_ASSERT(!Assignment_set(&assignment, 1u, 1));
  TEST_ASSERT(!Assignment_set(&assignment, 2u, 1));
  TEST_ASSERT(!Assignment_set(&assignment, 3u, 0));
  TEST_ASSERT(!Assignment_set(&assignment, 4u, 0));

  TEST_ASSERT(assignment.count == 4);
  TEST_ASSERT(assignment.capacity == 1024);

  int8_t value;
  Assignment_get(&assignment, 1u, &value);
  TEST_ASSERT(value == 1);

  Assignment_get(&assignment, 2u, &value);
  TEST_ASSERT(value == 1);

  Assignment_get(&assignment, 3u, &value);
  TEST_ASSERT(value == 0);

  Assignment_get(&assignment, 4u, &value);
  TEST_ASSERT(value == 0);

  Assignment_destroy(&assignment);
}

static void
test_Assignment_copy()
{
  uint32_t keys[] = { 1u, 2u, 3u, 4u };
  int8_t values[] = { 1, 1, 0, 0 };

  Assignment assignment1;
  TEST_ASSERT(!Assignment_create(&assignment1));

  TEST_ASSERT(!Assignment_set(&assignment1, keys[0u], values[0u]));
  TEST_ASSERT(!Assignment_set(&assignment1, keys[1u], values[1u]));
  TEST_ASSERT(!Assignment_set(&assignment1, keys[2u], values[2u]));
  TEST_ASSERT(!Assignment_set(&assignment1, keys[3u], values[3u]));

  TEST_ASSERT(assignment1.count == 4u);
  TEST_ASSERT(assignment1.capacity == 1024u);

  Assignment assignment2;
  Assignment_copy(&assignment2, &assignment1);

  for (size_t i = 0u; i < assignment1.count; ++i) {
    int8_t value1;
    int8_t value2;
    Assignment_get(&assignment1, keys[i], &value1);
    Assignment_get(&assignment2, keys[i], &value2);
    TEST_ASSERT(values[i] == value1);
    TEST_ASSERT(values[i] == value2);
  }

  Assignment_destroy(&assignment1);
  Assignment_destroy(&assignment2);
}

static void
test_Assignment_destroy()
{
  Assignment assignment;
  TEST_ASSERT(!Assignment_create(&assignment));

  TEST_ASSERT(!Assignment_set(&assignment, 1u, 1));
  TEST_ASSERT(!Assignment_set(&assignment, 2u, 1));
  TEST_ASSERT(!Assignment_set(&assignment, 3u, 0));
  TEST_ASSERT(!Assignment_set(&assignment, 4u, 0));

  Assignment_destroy(&assignment);

  TEST_ASSERT(assignment.count == 0u);
  TEST_ASSERT(assignment.capacity == 0u);
  TEST_ASSERT(assignment.pKeys == NULL);
  TEST_ASSERT(assignment.pValues == NULL);
}

int
main()
{
  test_Assignment_create();
  test_Assignment_getAndSet();
  test_Assignment_copy();
  test_Assignment_destroy();

  return 0;
}

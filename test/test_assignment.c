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

  TEST_ASSERT(!Assignment_set(&assignment, 1u, true));
  TEST_ASSERT(!Assignment_set(&assignment, 2u, true));
  TEST_ASSERT(!Assignment_set(&assignment, 3u, false));
  TEST_ASSERT(!Assignment_set(&assignment, 4u, false));

  TEST_ASSERT(assignment.count == 4);
  TEST_ASSERT(assignment.capacity == 1024);

  bool value;
  Assignment_get(&assignment, 1u, &value);
  TEST_ASSERT(value);

  Assignment_get(&assignment, 2u, &value);
  TEST_ASSERT(value);

  Assignment_get(&assignment, 3u, &value);
  TEST_ASSERT(!value);

  Assignment_get(&assignment, 4u, &value);
  TEST_ASSERT(!value);

  Assignment_destroy(&assignment);
}

static void
test_Assignment_copy()
{
  uint32_t keys[] = { 1u, 2u, 3u, 4u };
  bool values[] = { true, true, false, false };

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
    bool value1;
    bool value2;
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

  TEST_ASSERT(!Assignment_set(&assignment, 1u, true));
  TEST_ASSERT(!Assignment_set(&assignment, 2u, true));
  TEST_ASSERT(!Assignment_set(&assignment, 3u, false));
  TEST_ASSERT(!Assignment_set(&assignment, 4u, false));

  Assignment_destroy(&assignment);

  TEST_ASSERT(assignment.count == 0u);
  TEST_ASSERT(assignment.capacity == 0u);
  TEST_ASSERT(assignment.pKeys == NULL);
  TEST_ASSERT(assignment.pValues == NULL);
}

void
test_Assignment_swap()
{
  Assignment a;
  TEST_ASSERT(!Assignment_create(&a));

  Assignment b;
  TEST_ASSERT(!Assignment_create(&b));

  TEST_ASSERT(!Assignment_set(&a, 1u, false));
  TEST_ASSERT(!Assignment_set(&a, 3u, true));

  TEST_ASSERT(!Assignment_set(&b, 2u, true));
  TEST_ASSERT(!Assignment_set(&b, 4u, false));
  TEST_ASSERT(!Assignment_set(&b, 6u, true));

  Assignment_swap(&a, &b);

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

  Assignment_destroy(&b);
  Assignment_destroy(&a);
}

void
test_Assignment_setAll()
{
  Assignment a;
  TEST_ASSERT(!Assignment_create(&a));

  Assignment b;
  TEST_ASSERT(!Assignment_create(&b));

  TEST_ASSERT(!Assignment_set(&a, 1u, false));
  TEST_ASSERT(!Assignment_set(&a, 3u, true));

  TEST_ASSERT(!Assignment_set(&b, 2u, true));
  TEST_ASSERT(!Assignment_set(&b, 4u, false));
  TEST_ASSERT(!Assignment_set(&b, 6u, true));

  Assignment_setAll(&a, &b);

  TEST_ASSERT(a.count == 5);
  TEST_ASSERT(a.pKeys[0] == 1u);
  TEST_ASSERT(a.pKeys[1] == 3u);
  TEST_ASSERT(a.pKeys[2] == 2u);
  TEST_ASSERT(a.pKeys[3] == 4u);
  TEST_ASSERT(a.pKeys[4] == 6u);
  TEST_ASSERT(a.pValues[0] == false);
  TEST_ASSERT(a.pValues[1] == true);
  TEST_ASSERT(a.pValues[2] == true);
  TEST_ASSERT(a.pValues[3] == false);
  TEST_ASSERT(a.pValues[4] == true);

  TEST_ASSERT(b.count == 3);
  TEST_ASSERT(b.pKeys[0] == 2u);
  TEST_ASSERT(b.pKeys[1] == 4u);
  TEST_ASSERT(b.pKeys[2] == 6u);
  TEST_ASSERT(b.pValues[0] == true);
  TEST_ASSERT(b.pValues[1] == false);
  TEST_ASSERT(b.pValues[2] == true);

  Assignment_destroy(&b);
  Assignment_destroy(&a);
}

int
main()
{
  test_Assignment_create();
  test_Assignment_getAndSet();
  test_Assignment_copy();
  test_Assignment_destroy();
  test_Assignment_swap();
  test_Assignment_setAll();

  return 0;
}

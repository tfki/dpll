#include <assert.h>
#include <solver/assignment.h>

static void
test_Assignment_create()
{
  Assignment assignment;
  assert(!Assignment_create(&assignment));
  assert(assignment.count == 0u);
  assert(assignment.capacity == 1024u);
  assert(assignment.pKeys != NULL);
  assert(assignment.pValues != NULL);

  Assignment_destroy(&assignment);
}

static void
test_Assignment_getAndSet()
{
  Assignment assignment;
  assert(!Assignment_create(&assignment));

  assert(!Assignment_set(&assignment, 1u, 1));
  assert(!Assignment_set(&assignment, 2u, 1));
  assert(!Assignment_set(&assignment, 3u, 0));
  assert(!Assignment_set(&assignment, 4u, 0));

  assert(assignment.count == 4);
  assert(assignment.capacity == 1024);

  int8_t value;
  Assignment_get(&assignment, 1u, &value);
  assert(value == 1);

  Assignment_get(&assignment, 2u, &value);
  assert(value == 1);

  Assignment_get(&assignment, 3u, &value);
  assert(value == 0);

  Assignment_get(&assignment, 4u, &value);
  assert(value == 0);

  Assignment_destroy(&assignment);
}

static void
test_Assignment_copy()
{
  uint32_t keys[] = { 1u, 2u, 3u, 4u };
  int8_t values[] = { 1, 1, 0, 0 };

  Assignment assignment1;
  assert(!Assignment_create(&assignment1));

  assert(!Assignment_set(&assignment1, keys[0u], values[0u]));
  assert(!Assignment_set(&assignment1, keys[1u], values[1u]));
  assert(!Assignment_set(&assignment1, keys[2u], values[2u]));
  assert(!Assignment_set(&assignment1, keys[3u], values[3u]));

  assert(assignment1.count == 4u);
  assert(assignment1.capacity == 1024u);

  Assignment assignment2;
  Assignment_copy(&assignment2, &assignment1);

  for (size_t i = 0u; i < assignment1.count; ++i) {
    int8_t value1;
    int8_t value2;
    Assignment_get(&assignment1, keys[i], &value1);
    Assignment_get(&assignment2, keys[i], &value2);
    assert(values[i] == value1);
    assert(values[i] == value2);
  }

  Assignment_destroy(&assignment1);
  Assignment_destroy(&assignment2);
}

static void
test_Assignment_destroy()
{
  Assignment assignment;
  assert(!Assignment_create(&assignment));

  assert(!Assignment_set(&assignment, 1u, 1));
  assert(!Assignment_set(&assignment, 2u, 1));
  assert(!Assignment_set(&assignment, 3u, 0));
  assert(!Assignment_set(&assignment, 4u, 0));

  Assignment_destroy(&assignment);

  assert(assignment.count == 0u);
  assert(assignment.capacity == 0u);
  assert(assignment.pKeys == NULL);
  assert(assignment.pValues == NULL);
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

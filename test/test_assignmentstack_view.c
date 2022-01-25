#include "test_common.h"

#include <solver/assignmentstack.h>

static void
test_AssignmentStackView_get()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));

  // variable not visible to the view
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 2u, false));

  AssignmentStackView assignmentView;
  AssignmentStackView_beginView(&assignmentView, &assignment);

  // variable not visible to the view
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 3u, true));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 4u, false));

  AssignmentStackView_endView(&assignmentView, &assignment);
  TEST_ASSERT(assignmentView.count == 2u);

  bool value;
  TEST_ASSERT(AssignmentStackView_get(&assignmentView, 1u, &value));
  TEST_ASSERT(AssignmentStackView_get(&assignmentView, 2u, &value));

  TEST_ASSERT_SUCCESS(AssignmentStackView_get(&assignmentView, 3u, &value));
  TEST_ASSERT(value);
  TEST_ASSERT_SUCCESS(AssignmentStackView_get(&assignmentView, 4u, &value));
  TEST_ASSERT(!value);

  AssignmentStack_destroy(&assignment);
}

int
main()
{
  test_AssignmentStackView_get();
  return 0;
}

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

  // last parameter should not be touched! So NULL is ok here.
  TEST_ASSERT_FAILURE(AssignmentStackView_get(&assignmentView, 1u, NULL));
  TEST_ASSERT_FAILURE(AssignmentStackView_get(&assignmentView, 2u, NULL));

  bool v1, v2;
  TEST_ASSERT_SUCCESS(AssignmentStackView_get(&assignmentView, 3u, &v1));
  TEST_ASSERT_SUCCESS(AssignmentStackView_get(&assignmentView, 4u, &v2));
  TEST_ASSERT_TRUE(v1);
  TEST_ASSERT_FALSE(v2);

  AssignmentStack_destroy(&assignment);
}

int
main()
{
  test_AssignmentStackView_get();
}

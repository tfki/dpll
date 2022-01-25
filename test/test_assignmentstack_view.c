#include "test_common.h"

#include <solver/assignmentstack.h>

static void
test_AssignmentStackView_get()
{
  AssignmentStack assignment;
  TEST_ASSERT(!AssignmentStack_create(&assignment));

  // variable not visible to the view
  TEST_ASSERT(!AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 2u, false));

  AssignmentStackView assignmentView;
  AssignmentStackView_beginView(&assignmentView, &assignment);

  // variable not visible to the view
  TEST_ASSERT(!AssignmentStack_push(&assignment, 3u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 4u, false));

  AssignmentStackView_endView(&assignmentView, &assignment);
  TEST_ASSERT(assignmentView.count == 2u);

  bool value;
  TEST_ASSERT(AssignmentStackView_get(&assignmentView, 1u, &value));
  TEST_ASSERT(AssignmentStackView_get(&assignmentView, 2u, &value));

  TEST_ASSERT(!AssignmentStackView_get(&assignmentView, 3u, &value));
  TEST_ASSERT(value);
  TEST_ASSERT(!AssignmentStackView_get(&assignmentView, 4u, &value));
  TEST_ASSERT(!value);

  AssignmentStack_destroy(&assignment);
}

void
test_AssignmentStackView_with_max_size(){
  AssignmentStack assignment;
  TEST_ASSERT(!AssignmentStack_create(&assignment));

  AssignmentStackView assignmentView;
  AssignmentStackView_beginView(&assignmentView, &assignment);

  TEST_ASSERT(!AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 2u, false));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 3u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 4u, false));

  AssignmentStackView_endView(&assignmentView, &assignment);
  TEST_ASSERT(assignmentView.count == 4u);

  bool value;
  TEST_ASSERT(!AssignmentStackView_get(&assignmentView, 1u, &value));
  TEST_ASSERT(value);
  TEST_ASSERT(!AssignmentStackView_get(&assignmentView, 2u, &value));
  TEST_ASSERT(!value);
  TEST_ASSERT(!AssignmentStackView_get(&assignmentView, 3u, &value));
  TEST_ASSERT(value);
  TEST_ASSERT(!AssignmentStackView_get(&assignmentView, 4u, &value));
  TEST_ASSERT(!value);

  AssignmentStack_destroy(&assignment);
}

void
test_AssignmentStackView_empty(){
  AssignmentStack assignment;
  TEST_ASSERT(!AssignmentStack_create(&assignment));

  AssignmentStackView assignmentView;

  TEST_ASSERT(!AssignmentStack_push(&assignment, 1u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 2u, false));

  AssignmentStackView_beginView(&assignmentView, &assignment);
  AssignmentStackView_endView(&assignmentView, &assignment);

  TEST_ASSERT(!AssignmentStack_push(&assignment, 3u, true));
  TEST_ASSERT(!AssignmentStack_push(&assignment, 4u, false));

  TEST_ASSERT(assignmentView.count == 0u);

  AssignmentStack_destroy(&assignment);
}

int
main()
{
  test_AssignmentStackView_get();
  test_AssignmentStackView_with_max_size();
  test_AssignmentStackView_empty();
}

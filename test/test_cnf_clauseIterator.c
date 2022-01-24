#include "test_common.h"

#include <cnf/cnf.h>

void
test_cnf_clauseIterator_with_empty_cnf()
{
  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

void
test_cnf_clauseIterator_with_one_clause()
{
  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  int32_t clause1[] = { 1, 2, -3 };
  size_t clause1Size = 3u;

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Size));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT(!Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT(iter.count == 3u);
  TEST_ASSERT(iter.pData[0] == 1);
  TEST_ASSERT(iter.pData[1] == 2);
  TEST_ASSERT(iter.pData[2] == -3);

  TEST_ASSERT(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

void
test_cnf_clauseIterator_with_multiple_clauses()
{
  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  int32_t clause1[] = { 1, 2, 3 };
  size_t clause1Size = 3u;

  int32_t clause2[] = { 5, 2, -6, 1 };
  size_t clause2Size = 4u;

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause2, clause2Size));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT(!Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT(iter.count == 3u);
  TEST_ASSERT(iter.pData[0] == 1);
  TEST_ASSERT(iter.pData[1] == 2);
  TEST_ASSERT(iter.pData[2] == 3);

  TEST_ASSERT(!Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT(iter.count == 4u);
  TEST_ASSERT(iter.pData[0] == 5);
  TEST_ASSERT(iter.pData[1] == 2);
  TEST_ASSERT(iter.pData[2] == -6);
  TEST_ASSERT(iter.pData[3] == 1);

  TEST_ASSERT(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

void
test_cnf_clauseIterator_with_mixed()
{
  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  int32_t clause1[] = { 1, 2, 3 };
  size_t clause1Size = 3u;

  int32_t *clause2 = NULL;
  size_t clause2Size = 0u;

  int32_t clause3[] = { 5, 2, -6, 1 };
  size_t clause3Size = 4u;

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause2, clause2Size));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause3, clause3Size));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT(!Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT(iter.count == 3u);
  TEST_ASSERT(iter.pData[0] == 1);
  TEST_ASSERT(iter.pData[1] == 2);
  TEST_ASSERT(iter.pData[2] == 3);

  TEST_ASSERT(!Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT(iter.count == 0u);

  TEST_ASSERT(!Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT(iter.count == 4u);
  TEST_ASSERT(iter.pData[0] == 5);
  TEST_ASSERT(iter.pData[1] == 2);
  TEST_ASSERT(iter.pData[2] == -6);
  TEST_ASSERT(iter.pData[3] == 1);

  TEST_ASSERT(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

int
main()
{
  test_cnf_clauseIterator_with_empty_cnf();
  test_cnf_clauseIterator_with_one_clause();
  test_cnf_clauseIterator_with_multiple_clauses();
  test_cnf_clauseIterator_with_mixed();

  return 0;
}

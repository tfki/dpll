#include <cnf/cnf.h>

#include <common/test.h>

static void
test_cnf_clauseIterator_with_empty_cnf()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT_FALSE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_FALSE(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

static void
test_cnf_clauseIterator_with_one_clause()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause1[] = { 1, 2, -3 };
  size_t clause1Size = 3u;

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Size));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT_TRUE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_EQ(iter.count, 3u);
  TEST_ASSERT_EQ(iter.pData[0], 1);
  TEST_ASSERT_EQ(iter.pData[1], 2);
  TEST_ASSERT_EQ(iter.pData[2], -3);

  TEST_ASSERT_SUCCESS(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

static void
test_cnf_clauseIterator_with_multiple_clauses()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause1[] = { 1, 2, 3 };
  size_t clause1Size = 3u;

  int32_t clause2[] = { 5, 2, -6, 1 };
  size_t clause2Size = 4u;

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Size));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT_TRUE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_EQ(iter.count, 3u);
  TEST_ASSERT_EQ(iter.pData[0], 1);
  TEST_ASSERT_EQ(iter.pData[1], 2);
  TEST_ASSERT_EQ(iter.pData[2], 3);

  TEST_ASSERT_TRUE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_EQ(iter.count, 4u);
  TEST_ASSERT_EQ(iter.pData[0], 5);
  TEST_ASSERT_EQ(iter.pData[1], 2);
  TEST_ASSERT_EQ(iter.pData[2], -6);
  TEST_ASSERT_EQ(iter.pData[3], 1);

  TEST_ASSERT_SUCCESS(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

static void
test_cnf_clauseIterator_with_mixed()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause1[] = { 1, 2, 3 };
  size_t clause1Size = 3u;

  int32_t* clause2 = NULL;
  size_t clause2Size = 0u;

  int32_t clause3[] = { 5, 2, -6, 1 };
  size_t clause3Size = 4u;

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause3, clause3Size));

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  TEST_ASSERT_TRUE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_EQ(iter.count, 3u);
  TEST_ASSERT_EQ(iter.pData[0], 1);
  TEST_ASSERT_EQ(iter.pData[1], 2);
  TEST_ASSERT_EQ(iter.pData[2], 3);

  TEST_ASSERT_TRUE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_EQ(iter.count, 0u);

  TEST_ASSERT_TRUE(Cnf_ClauseIterator_next(&iter));
  TEST_ASSERT_EQ(iter.count, 4u);
  TEST_ASSERT_EQ(iter.pData[0], 5);
  TEST_ASSERT_EQ(iter.pData[1], 2);
  TEST_ASSERT_EQ(iter.pData[2], -6);
  TEST_ASSERT_EQ(iter.pData[3], 1);

  TEST_ASSERT_SUCCESS(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

int
main()
{
  test_cnf_clauseIterator_with_empty_cnf();
  test_cnf_clauseIterator_with_one_clause();
  test_cnf_clauseIterator_with_multiple_clauses();
  test_cnf_clauseIterator_with_mixed();
}

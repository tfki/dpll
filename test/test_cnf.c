#include <assert.h>
#include <cnf/cnf.h>

static int
test_cnf_create()
{
  cnf testCnf;

  assert(!cnf_create(&testCnf));
  assert(testCnf.count == 0u);
  assert(testCnf.capacity == 1024u);
  assert(testCnf.pData != NULL);

  cnf_destroy(&testCnf);
  return 0;
}

static int
test_cnf_pushClause()
{
  const int32_t rawCnf[] = { 10, -13, 11, -14, 0, -10, -11, 0, 16, -13, -11, 0 };
  const size_t rawCnfCount = 12u;

  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const int32_t clause2[] = { 16, -13, -11 };
  const size_t clause0_count = 4u;
  const size_t clause1_count = 2u;
  const size_t clause2_count = 3u;

  cnf testCnf;
  assert(!cnf_create(&testCnf));

  assert(!cnf_pushClause(&testCnf, clause0, clause0_count));
  assert(testCnf.count == 5);

  assert(!cnf_pushClause(&testCnf, clause1, clause1_count));
  assert(testCnf.count == 8);

  assert(!cnf_pushClause(&testCnf, clause2, clause2_count));
  assert(testCnf.count == 12);

  for (size_t i = 0u; i < rawCnfCount; ++i)
    assert(testCnf.pData[i] == rawCnf[i]);

  cnf_destroy(&testCnf);
  return 0;
}

static int
test_cnf_capacityOverflow()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  cnf testCnf;
  assert(!cnf_create(&testCnf));

  const size_t testCnfCapacity = testCnf.capacity;
  for (size_t i = testCnf.count; i < testCnfCapacity; i += 2)
    assert(!cnf_pushClause(&testCnf, clause, clauseCount));

  assert(testCnf.capacity == 1024u);
  assert(testCnf.count == 1024u);

  assert(!cnf_pushClause(&testCnf, clause, clauseCount));
  assert(testCnf.capacity == 2048u);
  assert(testCnf.count == 1026u);

  cnf_destroy(&testCnf);
  return 0;
}

static int
test_cnf_copy()
{
  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const size_t clause0_count = 4u;
  const size_t clause1_count = 2u;

  cnf testCnf;
  assert (!cnf_create(&testCnf));
  assert (!cnf_pushClause(&testCnf, clause0, clause0_count));
  assert (!cnf_pushClause(&testCnf, clause1, clause1_count));

  cnf copyCnf;
  assert (!cnf_copy(&copyCnf, &testCnf));

  for (size_t i = 0u; i < testCnf.count; ++i)
    assert (testCnf.pData[i] == copyCnf.pData[i]) ;

  cnf_destroy(&testCnf);
  cnf_destroy(&copyCnf);
  return 0;
}

static int
test_cnf_destroy()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  cnf testCnf;
  assert (!cnf_create(&testCnf));
  assert (!cnf_pushClause(&testCnf, clause, clauseCount));

  cnf_destroy(&testCnf);
  assert (testCnf.pData == NULL) ;
  assert (testCnf.capacity == 0u) ;
  assert (testCnf.count == 0u) ;

  return 0;
}

int
main()
{
  test_cnf_create();
  test_cnf_pushClause();
  test_cnf_capacityOverflow();
  test_cnf_copy();
  test_cnf_destroy();
}

#include <cnf/cnf.h>
#include <assert.h>

static int
test_Cnf_create()
{
  Cnf cnf;

  assert(!Cnf_create(&cnf));
  assert(cnf.count == 0u);
  assert(cnf.capacity == 1024u);
  assert(cnf.pData != NULL);

  Cnf_destroy(&cnf);
  return 0;
}

static int
test_Cnf_pushClause()
{
  const int32_t rawCnf[] = { 0, 10, -13, 11, -14, 0, -10, -11, 0, 16, -13, -11, 0 };
  const size_t rawCnfCount = 13u;

  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const int32_t clause2[] = { 16, -13, -11 };
  const size_t clause0Count = 4u;
  const size_t clause1Count = 2u;
  const size_t clause2Count = 3u;

  Cnf cnf;
  assert(!Cnf_create(&cnf));

  assert(!Cnf_pushClause(&cnf, clause0, clause0Count));
  assert(cnf.count == 6);

  assert(!Cnf_pushClause(&cnf, clause1, clause1Count));
  assert(cnf.count == 9);

  assert(!Cnf_pushClause(&cnf, clause2, clause2Count));
  assert(cnf.count == 13);

  for (size_t i = 0u; i < rawCnfCount; ++i)
    assert(cnf.pData[i] == rawCnf[i]);

  Cnf_destroy(&cnf);
  return 0;
}

static int
test_Cnf_capacityOverflow()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  Cnf cnf;
  assert(!Cnf_create(&cnf));

  // push first clause separately, as it will also insert leading zero
  // so count will be +=3
  assert(!Cnf_pushClause(&cnf, clause, clauseCount));

  const size_t testCnfCapacity = cnf.capacity;
  for (size_t i = cnf.count; i + 2 < testCnfCapacity; i += 2)
    assert(!Cnf_pushClause(&cnf, clause, clauseCount));

  assert(cnf.capacity == 1024u);
  assert(cnf.count == 1023u);

  assert(!Cnf_pushClause(&cnf, clause, clauseCount));
  assert(cnf.capacity == 2048u);
  assert(cnf.count == 1025u);

  Cnf_destroy(&cnf);
  return 0;
}

static int
test_Cnf_copy()
{
  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const size_t clause0Count = 4u;
  const size_t clause1Count = 2u;

  Cnf testCnf;
  assert(!Cnf_create(&testCnf));
  assert(!Cnf_pushClause(&testCnf, clause0, clause0Count));
  assert(!Cnf_pushClause(&testCnf, clause1, clause1Count));

  Cnf copyCnf;
  assert(!Cnf_copy(&copyCnf, &testCnf));

  for (size_t i = 0u; i < testCnf.count; ++i)
    assert(testCnf.pData[i] == copyCnf.pData[i]);

  Cnf_destroy(&testCnf);
  Cnf_destroy(&copyCnf);
  return 0;
}

static int
test_Cnf_destroy()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  Cnf testCnf;
  assert(!Cnf_create(&testCnf));
  assert(!Cnf_pushClause(&testCnf, clause, clauseCount));

  Cnf_destroy(&testCnf);
  assert(testCnf.pData == NULL);
  assert(testCnf.capacity == 0u);
  assert(testCnf.count == 0u);

  return 0;
}

int
main()
{
  test_Cnf_create();
  test_Cnf_pushClause();
  test_Cnf_capacityOverflow();
  test_Cnf_copy();
  test_Cnf_destroy();
}

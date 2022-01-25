
#include "test_common.h"
#include <cnf/dimacs.h>

void
test_parseDimacs_with_empty_cnf()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  char* dimacs = "";

  TEST_ASSERT_SUCCESS(parseDimacs(dimacs, &cnf));
  TEST_ASSERT_EQ(cnf.count, 0u);

  Cnf_destroy(&cnf);
}

void
test_parseDimacs_with_empty_clause()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  char* dimacs = "0";

  TEST_ASSERT_SUCCESS(parseDimacs(dimacs, &cnf));

  TEST_ASSERT_EQ(cnf.count, 2u);
  TEST_ASSERT_EQ(cnf.pData[0], 0);
  TEST_ASSERT_EQ(cnf.pData[1], 0);

  Cnf_destroy(&cnf);
}

void
test_parseDimacs_with_one_clause()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  char* dimacs = "1 -2 5 0";

  TEST_ASSERT_FAILURE(parseDimacs(dimacs, &cnf));

  TEST_ASSERT_EQ(cnf.count, 5u);
  TEST_ASSERT_EQ(cnf.pData[0], 0);
  TEST_ASSERT_EQ(cnf.pData[1], 1);
  TEST_ASSERT_EQ(cnf.pData[2], -2);
  TEST_ASSERT_EQ(cnf.pData[3], 5);
  TEST_ASSERT_EQ(cnf.pData[4], 0);

  Cnf_destroy(&cnf);
}

void
test_parseDimacs_with_multiple_clauses()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  char* dimacs = "1 -2 5 0 2 1 5 0\n -4 2 -8 0 9 -2 4 \n8 0";
  int32_t rawCnf[] = { 0, 1, -2, 5, 0, 2, 1, 5, 0, -4, 2, -8, 0, 9, -2, 4, 8, 0 };

  TEST_ASSERT_SUCCESS(parseDimacs(dimacs, &cnf));

  TEST_ASSERT_EQ(cnf.count, 18u);
  for (size_t i = 0u; i < cnf.count; ++i)
    TEST_ASSERT_EQ(cnf.pData[i], rawCnf[i]);

  Cnf_destroy(&cnf);
}

int
main()
{
  test_parseDimacs_with_empty_cnf();
  test_parseDimacs_with_empty_clause();
  test_parseDimacs_with_one_clause();
  test_parseDimacs_with_multiple_clauses();
}

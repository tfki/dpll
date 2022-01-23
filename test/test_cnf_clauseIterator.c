
#include <assert.h>
#include <cnf/cnf.h>

void
test_cnf_clauseIterator_with_empty_cnf()
{
  Cnf cnf;
  Cnf_create(&cnf);

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  assert(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

void
test_cnf_clauseIterator_with_one_clause()
{
  Cnf cnf;
  Cnf_create(&cnf);

  int32_t clause1[] = {1, 2, -3};
  size_t clause1size = 3;

  Cnf_pushClause(&cnf, clause1, clause1size);

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  assert(!Cnf_ClauseIterator_next(&iter));
  assert(iter.count == 3);
  assert(iter.pData[0] == 1);
  assert(iter.pData[1] == 2);
  assert(iter.pData[2] == -3);

  assert(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

void
test_cnf_clauseIterator_with_multiple_clauses(){
  Cnf cnf;
  Cnf_create(&cnf);

  int32_t clause1[] = {1, 2, 3};
  size_t clause1size = 3;

  int32_t clause2[] = {5, 2, -6, 1};
  size_t clause2size = 4;

  Cnf_pushClause(&cnf, clause1, clause1size);
  Cnf_pushClause(&cnf, clause2, clause2size);

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  assert(!Cnf_ClauseIterator_next(&iter));
  assert(iter.count == 3);
  assert(iter.pData[0] == 1);
  assert(iter.pData[1] == 2);
  assert(iter.pData[2] == 3);

  assert(!Cnf_ClauseIterator_next(&iter));
  assert(iter.count == 4);
  assert(iter.pData[0] == 5);
  assert(iter.pData[1] == 2);
  assert(iter.pData[2] == -6);
  assert(iter.pData[3] == 1);

  assert(Cnf_ClauseIterator_next(&iter));

  Cnf_destroy(&cnf);
}

void
test_cnf_clauseIterator_with_mixed(){
  Cnf cnf;
  Cnf_create(&cnf);

  int32_t clause1[] = {1, 2, 3};
  size_t clause1size = 3;

  int32_t clause2[] = {};
  size_t clause2size = 0;

  int32_t clause3[] = {5, 2, -6, 1};
  size_t clause3size = 4;


  Cnf_pushClause(&cnf, clause1, clause1size);
  Cnf_pushClause(&cnf, clause2, clause2size);
  Cnf_pushClause(&cnf, clause3, clause3size);

  Cnf_ClauseIterator iter;
  Cnf_ClauseIterator_create(&iter, &cnf);

  assert(!Cnf_ClauseIterator_next(&iter));
  assert(iter.count == 3);
  assert(iter.pData[0] == 1);
  assert(iter.pData[1] == 2);
  assert(iter.pData[2] == 3);

  assert(!Cnf_ClauseIterator_next(&iter));
  assert(iter.count == 0);

  assert(!Cnf_ClauseIterator_next(&iter));
  assert(iter.count == 4);
  assert(iter.pData[0] == 5);
  assert(iter.pData[1] == 2);
  assert(iter.pData[2] == -6);
  assert(iter.pData[3] == 1);

  assert(Cnf_ClauseIterator_next(&iter));

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

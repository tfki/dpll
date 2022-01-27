#include "test_common.h"

#include <cnf/dimacs.h>
#include <common/freadall.h>
#include <solver/solver.h>

typedef enum Complexity
{
  COMPLEXITY_TRIVIAL = 0b00000010,
  COMPLEXITY_EASY = 0b00000100,
  COMPLEXITY_MEDIUM = 0b00001000,
  COMPLEXITY_HARD = 0b00010000,
  COMPLEXITY_INSANE = 0b00100000
} Complexity;

typedef struct IndexEntry
{
  char pFileName[64u];
  bool satisfyable;
  Complexity complexity;
} IndexEntry;

typedef struct IndexEntryArray
{
  IndexEntry pData[512u];
  size_t count;
} IndexEntryArray;

static void
IndexEntryArray_create(IndexEntryArray* pIndexEntryArray)
{
  pIndexEntryArray->count = 0u;
}

static int
IndexEntryArray_push(IndexEntryArray* pIndexEntryArray, IndexEntry indexEntry)
{
  if (pIndexEntryArray->count + 1u > 512u)
    return 1;

  pIndexEntryArray->pData[pIndexEntryArray->count] = indexEntry;
  ++pIndexEntryArray->count;

  return 0;
}

static void
IndexEntryArray_destroy(IndexEntryArray* pIndexEntryArray)
{
  pIndexEntryArray->count = 0u;
}

static int
freadindex(char* fileName, IndexEntryArray* pIndexEntries)
{
  char* csvString;
  TEST_ASSERT_SUCCESS(freadall(fileName, &csvString));

  size_t csvPos = 0u;

  // skip header
  for (;; ++csvPos) {
    if (csvString[csvPos] == '\n') {
      ++csvPos;
      break;
    }
    if (csvString[csvPos] == '\0')
      return 0;
  }

  IndexEntry indexEntry;
  indexEntry.pFileName[0u] = 'r';
  indexEntry.pFileName[1u] = 'e';
  indexEntry.pFileName[2u] = 's';
  indexEntry.pFileName[3u] = '/';

  size_t indexEntryBegin;
  size_t indexEntryEnd;
  for (;; ++csvPos) {

    indexEntryBegin = csvPos;

    // eat till first field separator
    for (;; ++csvPos) {
      if (csvString[csvPos] == ';')
        break;
      if (csvString[csvPos] == '\0')
        return 0; // TODO
    }
    indexEntryEnd = csvPos;

    // calculate file name size
    size_t fileNameSize = indexEntryEnd - indexEntryBegin;
    TEST_ASSERT(fileNameSize < 59u);

    // copy file name into index entry field
    memcpy(&indexEntry.pFileName[4u], &csvString[indexEntryBegin], fileNameSize);
    indexEntry.pFileName[fileNameSize + 4u] = '\0';

    switch (csvString[++csvPos]) {
      case 't':
        TEST_ASSERT_EQ(csvString[++csvPos], 'r');
        TEST_ASSERT_EQ(csvString[++csvPos], 'u');
        TEST_ASSERT_EQ(csvString[++csvPos], 'e');
        indexEntry.satisfyable = true;
        break;
      case 'f':
        TEST_ASSERT_EQ(csvString[++csvPos], 'a');
        TEST_ASSERT_EQ(csvString[++csvPos], 'l');
        TEST_ASSERT_EQ(csvString[++csvPos], 's');
        TEST_ASSERT_EQ(csvString[++csvPos], 'e');
        indexEntry.satisfyable = false;
        break;
      default:
        return 1;
    }

    TEST_ASSERT_EQ(csvString[++csvPos], ';');

    switch (csvString[++csvPos]) {
      case 'T':
        TEST_ASSERT_EQ(csvString[++csvPos], 'R');
        TEST_ASSERT_EQ(csvString[++csvPos], 'I');
        TEST_ASSERT_EQ(csvString[++csvPos], 'V');
        TEST_ASSERT_EQ(csvString[++csvPos], 'I');
        TEST_ASSERT_EQ(csvString[++csvPos], 'A');
        TEST_ASSERT_EQ(csvString[++csvPos], 'L');
        indexEntry.complexity = COMPLEXITY_TRIVIAL;
        break;
      case 'E':
        TEST_ASSERT_EQ(csvString[++csvPos], 'A');
        TEST_ASSERT_EQ(csvString[++csvPos], 'S');
        TEST_ASSERT_EQ(csvString[++csvPos], 'Y');
        indexEntry.complexity = COMPLEXITY_EASY;
        break;
      case 'M':
        TEST_ASSERT_EQ(csvString[++csvPos], 'E');
        TEST_ASSERT_EQ(csvString[++csvPos], 'D');
        TEST_ASSERT_EQ(csvString[++csvPos], 'I');
        TEST_ASSERT_EQ(csvString[++csvPos], 'U');
        TEST_ASSERT_EQ(csvString[++csvPos], 'M');
        indexEntry.complexity = COMPLEXITY_MEDIUM;
        break;
      case 'H':
        TEST_ASSERT_EQ(csvString[++csvPos], 'A');
        TEST_ASSERT_EQ(csvString[++csvPos], 'R');
        TEST_ASSERT_EQ(csvString[++csvPos], 'D');
        indexEntry.complexity = COMPLEXITY_HARD;
        break;
      case 'I':
        TEST_ASSERT_EQ(csvString[++csvPos], 'N');
        TEST_ASSERT_EQ(csvString[++csvPos], 'S');
        TEST_ASSERT_EQ(csvString[++csvPos], 'A');
        TEST_ASSERT_EQ(csvString[++csvPos], 'N');
        TEST_ASSERT_EQ(csvString[++csvPos], 'E');
        indexEntry.complexity = COMPLEXITY_INSANE;
        break;
      default:
        return 1;
    }

    // insert entry
    TEST_ASSERT_SUCCESS(IndexEntryArray_push(pIndexEntries, indexEntry));

    // check for eof
    ++csvPos;
    if (csvString[csvPos] == '\0')
      return 0;

    // newline is required here
    if (csvString[csvPos] == '\r')
      ++csvPos;
    TEST_ASSERT_EQ(csvString[csvPos], '\n');
  }
}

static void
test_dpllsolver_res(const IndexEntry* entry)
{
  TEST_ASSERT_NON_NULL(entry);

  char* dimacs;
  TEST_ASSERT_SUCCESS(freadall(entry->pFileName, &dimacs));

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(parseDimacs(dimacs, &cnf));
  free(dimacs);

  AssignmentStack assignmentStack;
  TEST_ASSERT_EQ(0u == dpllSolve(&cnf, dpllTrivialPick, &assignmentStack), entry->satisfyable);

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignmentStack);
}

int
main()
{
  char* fileName = "res/_index.csv";

  IndexEntryArray iea;
  IndexEntryArray_create(&iea);

  TEST_ASSERT_SUCCESS(freadindex(fileName, &iea));

  for (size_t i = 0u; i < iea.count; ++i) {

    // skip hard and insane dimacs for testing
    if (iea.pData[i].complexity > COMPLEXITY_MEDIUM)
      continue;

    test_dpllsolver_res(&iea.pData[i]);
  }

  IndexEntryArray_destroy(&iea);
}

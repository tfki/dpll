#include <cnf/dimacs.h>
#include <solver/solver.h>

#include <common/bench.h>
#include <common/fread.h>
#include <common/test.h>

int
pathConcat(const char* pBse, const char* pRel, char* pBuf, int bufSize)
{
#ifdef _WIN32
  char separator = '\\';
#else
  char separator = '/';
#endif

  int bseSize = strlen(pBse);
  int relSize = strlen(pRel);

  if ((pRel == pBuf) && (relSize > bseSize))
    return 1;

  if (bufSize < (bseSize + relSize + 2))
    return 1;

  pBuf[bseSize] = separator;
  memcpy(&pBuf[bseSize + 1], pRel, relSize);
  pBuf[bseSize + 1 + relSize] = '\0';

  if (pBse != pBuf)
    memcpy(pBuf, pBse, bseSize);

  return 0;
}

int
pathCopy(const char* pBse, char* pBuf, int bufSize)
{
  int bseSize = strlen(pBse);

  if (bufSize < (bseSize + 1))
    return 1;

  memcpy(pBuf, pBse, bseSize + 1);
  return 0;
}

void
fileNameFromPath(const char* pBse, const char** pRel)
{
  int bseSize = strlen(pBse);

  for (int i = bseSize-1; i > 0; --i) {
    if (pBse[i] == '\\' || pBse[i] == '/')
    {
      *pRel = &pBse[i+1];
      return;
    }
  }

  *pRel = pBse;
}

typedef enum Complexity
{
  COMPLEXITY_TRIVIAL = 0b00000010,
  COMPLEXITY_EASY = 0b00000100,
  COMPLEXITY_MEDIUM = 0b00001000,
  COMPLEXITY_HARD = 0b00010000,
  COMPLEXITY_INSANE = 0b00100000
} Complexity;

typedef struct Entry
{
  char pFileName[256u];
  bool satisfiable;
  Complexity complexity;
} Entry;

typedef struct EntryArray
{
  Entry pData[512u];
  size_t count;
} EntryArray;

static void
EntryArray_create(EntryArray* pIndexEntryArray)
{
  pIndexEntryArray->count = 0u;
}

static int
EntryArray_push(EntryArray* pIndexEntryArray, Entry indexEntry)
{
  if (pIndexEntryArray->count + 1u > sizeof(pIndexEntryArray->pData))
    return 1;

  pIndexEntryArray->pData[pIndexEntryArray->count] = indexEntry;
  ++pIndexEntryArray->count;

  return 0;
}

static void
EntryArray_destroy(EntryArray* pIndexEntryArray)
{
  pIndexEntryArray->count = 0u;
}

static int
freadindex(char* fileName, EntryArray* pEntries)
{
  char* csvString;

  if (fReadAll(fileName, &csvString))
    return 1;

  size_t csvPos = 0u;

  // skip header
  for (;; ++csvPos) {
    if (csvString[csvPos] == '\n') {
      ++csvPos;
      break;
    }
    if (csvString[csvPos] == '\0') {
      free(csvString);
      return 0;
    }
  }

  Entry indexEntry;

  size_t indexEntryBegin;
  size_t indexEntryEnd;
  for (;; ++csvPos) {

    indexEntryBegin = csvPos;

    // eat till first field separator
    for (;; ++csvPos) {
      if (csvString[csvPos] == ';')
        break;
      if (csvString[csvPos] == '\0') {
        free(csvString);
        return 0; // TODO
      }
    }
    indexEntryEnd = csvPos;

    // calculate file name size
    size_t fileNameSize = indexEntryEnd - indexEntryBegin;
    TEST_ASSERT(fileNameSize < (sizeof(indexEntry.pFileName) - 1u));

    // copy file name into index entry field
    memcpy(indexEntry.pFileName, &csvString[indexEntryBegin], fileNameSize);
    indexEntry.pFileName[fileNameSize] = '\0';

    switch (csvString[++csvPos]) {
      case 't':
        TEST_ASSERT_EQ(csvString[++csvPos], 'r');
        TEST_ASSERT_EQ(csvString[++csvPos], 'u');
        TEST_ASSERT_EQ(csvString[++csvPos], 'e');
        indexEntry.satisfiable = true;
        break;
      case 'f':
        TEST_ASSERT_EQ(csvString[++csvPos], 'a');
        TEST_ASSERT_EQ(csvString[++csvPos], 'l');
        TEST_ASSERT_EQ(csvString[++csvPos], 's');
        TEST_ASSERT_EQ(csvString[++csvPos], 'e');
        indexEntry.satisfiable = false;
        break;
      default:
        free(csvString);
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
        free(csvString);
        return 1;
    }

    // insert entry
    TEST_ASSERT_SUCCESS(EntryArray_push(pEntries, indexEntry));

    // check for eof
    ++csvPos;
    if (csvString[csvPos] == '\0') {
      free(csvString);
      return 0;
    }

    // newline is required here
    if (csvString[csvPos] == '\r')
      ++csvPos;
    TEST_ASSERT_EQ(csvString[csvPos], '\n');
  }
}

static void
test_dpllsolver_res(const Entry* entry)
{
  TEST_ASSERT_NON_NULL(entry);

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(fReadCnf(entry->pFileName, &cnf));

  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));

  Bench bench;
  Bench_create(&bench);

  TEST_ASSERT_SUCCESS(Bench_begin(&bench));
  int status = dpllSolve(&cnf, dpllTrivialPick, &assignment);
  TEST_ASSERT_SUCCESS(Bench_end(&bench));

  TEST_ASSERT_EQ(0u == status, entry->satisfiable);

  double seconds;
  Bench_seconds(&bench, &seconds);

  const char *fileName;
  fileNameFromPath(entry->pFileName, &fileName);
  LOGI("Dimacs file (%s) was successfully tested. It took %f seconds to find a sat solution.", fileName, seconds);

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignment);
}

int
main(int argc, char** argv)
{
  // get the absolute path to index.csv

  TEST_ASSERT_EQ(argc, 1);
  char absPath[512u];
  const int absPathSize = sizeof(absPath);

  TEST_ASSERT_SUCCESS(pathConcat(argv[0], "..", absPath, absPathSize));
  TEST_ASSERT_SUCCESS(pathConcat(absPath, "res", absPath, absPathSize));
  TEST_ASSERT_SUCCESS(pathConcat(absPath, "_index.csv", absPath, absPathSize));

  // create index array from file

  EntryArray entries;
  EntryArray_create(&entries);

  TEST_ASSERT_SUCCESS(freadindex(absPath, &entries));

  // make index array entry file names absolute paths

  for (size_t i = 0u; i < entries.count; ++i) {
    char* entryFileName = entries.pData[i].pFileName;
    const int entryFileNameSize = sizeof(entries.pData->pFileName);

    TEST_ASSERT_SUCCESS(pathConcat(argv[0], "..", absPath, absPathSize));
    TEST_ASSERT_SUCCESS(pathConcat(absPath, "res", absPath, absPathSize));
    TEST_ASSERT_SUCCESS(pathConcat(absPath, entryFileName, absPath, absPathSize));
    TEST_ASSERT_SUCCESS(pathCopy(absPath, entryFileName, entryFileNameSize));
  }

  // run all indices as test

  for (size_t i = 0u; i < entries.count; ++i) {

    // skip hard and insane dimacs for testing
    if (entries.pData[i].complexity > COMPLEXITY_MEDIUM)
      continue;

    test_dpllsolver_res(&entries.pData[i]);
  }

  EntryArray_destroy(&entries);
}

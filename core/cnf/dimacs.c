#include "cnf/dimacs.h"
#include <common/common.h>
#include <stdio.h>
#include <string.h>

typedef enum dimacsParserState
{
  START_OF_LINE,
  PARSING_CLAUSE,
  PARSING_PROBLEM,
  PARSING_COMMENT,
  PARSING_LITERAL
} dimacsParserState;

bool
parseDimacs(char* dimacs, Cnf* cnf)
{
  SANITIZING_ASSERT(dimacs);     // dimacs must not be NULL
  SANITIZING_ASSERT(cnf);        // cnf must not be null
  SANITIZING_ASSERT(cnf->pData); // cnf must be created

  size_t dimacsOffset = 0;
  size_t dimacsLen = strlen(dimacs);

  dimacsParserState state = START_OF_LINE;

  ClauseBuffer clauseBuffer;
  ClauseBuffer_create(&clauseBuffer);

  char* literalStart;
  char* literalEnd;

  while (dimacsOffset < dimacsLen) {
    char inputChar = dimacs[dimacsOffset];

    switch (state) {
      case START_OF_LINE:
        switch (inputChar) {
          case '\0':
            // we are at the end of dimacs
            ClauseBuffer_destroy(&clauseBuffer);
            return true;
          case 'c':
            state = PARSING_COMMENT;
            break;
          case 'p':
            state = PARSING_PROBLEM;
            break;
          case '0':
            // empty clause
            Cnf_pushClause(cnf, NULL, 0);
            break;
          case '-':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            state = PARSING_LITERAL;
            literalStart = dimacs + dimacsOffset;
            ClauseBuffer_reset(&clauseBuffer);
            break;
          default:
            break;
        }
        break;

      case PARSING_CLAUSE:
        switch (inputChar) {
          case '0':
            // end of clause reached
            state = START_OF_LINE;
            Cnf_pushClause(cnf, clauseBuffer.pData, clauseBuffer.count);
            ClauseBuffer_reset(&clauseBuffer);
            break;
          case '-':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            state = PARSING_LITERAL;
            literalStart = dimacs + dimacsOffset;
            break;
          case '\n':
          case '\t':
          case ' ':
            // ignore whitespace
            break;
          default:
            ClauseBuffer_destroy(&clauseBuffer);
            printf("unexpected character '%c' at index %d. expected whitespace or number or sign", inputChar, dimacsOffset);
            return false;
        }
        break;

      case PARSING_LITERAL:
        switch (inputChar) {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            break;
          default:
            // literal over
            state = PARSING_CLAUSE;
            literalEnd = dimacs + dimacsOffset - 1;
            int32_t value = strtol(literalStart, &literalEnd, 10);

            if (ClauseBuffer_push(&clauseBuffer, value)) {
              return false;
            }
            break;
        }
        break;

      case PARSING_COMMENT:
      case PARSING_PROBLEM:
        switch (inputChar) {
          case '\n':
            state = START_OF_LINE;
            break;
          default:
            break;
        }
        break;
    }

    dimacsOffset++;
  }

  ClauseBuffer_destroy(&clauseBuffer);

  return true;
}

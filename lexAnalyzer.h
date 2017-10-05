/*--------------------------------------------------------------------*/
/* lexAnalyzer.h                                                      */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#ifndef LEXANALYZER_INCLUDED
#define LEXANALYZER_INCLUDED

#include <stdio.h>
#include "dynarray.h"

/* If no lines remain in psFile, then return NULL. Otherwise read a line
   of psFile and return it as a string. The string does not contain a
   terminating newline character. The caller owns the string. */

char* LexAnalyzer_readLine(FILE* psFile);

/*--------------------------------------------------------------------*/

/* Write all tokens in oTokens to stdout.  First write the number
   tokens; then write the word tokens. */

void LexAnalyzer_writeTokens(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Free all of the tokens in oTokens. */

void LexAnalyzer_freeTokens(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Lexically analyze string pcLine.  If pcLine contains a lexical
   error, then return NULL.  Otherwise return a DynArray object
   containing the tokens in pcLine.  The caller owns the DynArray
   object and the tokens that it contains. */

DynArray_T LexAnalyzer_lexLine(const char *pcLine);

#endif

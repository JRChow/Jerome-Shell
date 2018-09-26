/*--------------------------------------------------------------------*/
/* synAnalyzer.h                                                      */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#ifndef SYNANALYZER_INCLUDED
#define SYNANALYZER_INCLUDED

#include "dynarray.h"
#include "command.h"

/*--------------------------------------------------------------------*/

/* Free all elements in the dynamic array oTokens. */

void SynAnalyzer_freeElements(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Syntactically analyze dynamic array oTokens. If oTokens contains a 
   syntactic error, then return NULL.  Otherwise return a Command 
   object. */

Command_T SynAnalyzer_synTokens(DynArray_T oTokens);

#endif

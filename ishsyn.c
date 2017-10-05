/*--------------------------------------------------------------------*/
/* ishsyn.c                                                           */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#include "lexAnalyzer.h"
#include "synAnalyzer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

/* The name of the executable binary file. */
static const char *pcPgmName;

/*--------------------------------------------------------------------*/

/* Return the value of the global variable pcPgmName, which means the 
   name of the program. */

const char* getPgmName()
{
   assert(pcPgmName != NULL);

   return pcPgmName;
}

/*--------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
   char* pcLine;
   int iRet;
   DynArray_T oTokens = NULL;
   Command_T oCommand = NULL;
   
   pcPgmName = argv[0];
   
   /* Write to stdout a prompt. */
   printf("%% ");

   /* Read a line from stdin. */
   while ((pcLine = LexAnalyzer_readLine(stdin)) != NULL)
   {
      /* Write the line to stdout. */
      printf("%s\n", pcLine);
      
      /* Flush the stdout buffer. */
      iRet = fflush(stdout);
      if (iRet == EOF)
      {perror(pcPgmName); exit(EXIT_FAILURE);}
      
      /* Pass the line to lexical analyzer. */
      oTokens = LexAnalyzer_lexLine(pcLine);
      
      /* Pass the DynArray object to syntactic analyzer. */
      if (oTokens != NULL)
         oCommand = SynAnalyzer_synTokens(oTokens);
      else oCommand = NULL;
      
      /* Print command and free command. */
      if (oCommand != NULL)
      {
         Command_print(oCommand);
         Command_free(oCommand);
      }
      
      /* Free up memory. */
      if (oTokens != NULL)
      {
         LexAnalyzer_freeTokens(oTokens);
         DynArray_free(oTokens);
      }
      free(pcLine);
      
      printf("%% ");
   }
   printf("\n");
   return 0;
}

/*--------------------------------------------------------------------*/
/* synAnalyzer.c                                                      */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#include "synAnalyzer.h"
#include "token.h"
#include "ish.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/

/* Syntactically analyze dynamic array oTokens. If oTokens contains a 
   syntactic error, then return NULL.  Otherwise return a Command 
   object. */

Command_T SynAnalyzer_synTokens(DynArray_T oTokens)
{
   size_t ulInd; /* Index iterator. */
   size_t ulLen; /* Length of oTokens. */
   size_t ulStdInCount = 0; /* Count the number of stdin. */
   size_t ulStdOutCount = 0; /* Count the number of stdout. */
   Token_T oToken = NULL;
   Command_T oCommand = NULL;
   char* pcName = NULL;
   char* pcStdIn = NULL;
   char* pcStdOut = NULL;
   DynArray_T oArgs = NULL;
   
   assert(oTokens != NULL);

   /* Handle empty input. */
   if (DynArray_getLength(oTokens) == 0) return NULL;


   /* Ensures oTokens must begin with an ordinary token. */
   ulLen = DynArray_getLength(oTokens);
   oToken = DynArray_get(oTokens, 0);
   if (Token_getType(oToken) != TOKEN_ORDINARY)
   {
      fprintf(stderr, "%s: missing command name\n", getPgmName());
      return NULL;
   }
   pcName = Token_getString(oToken);
   
   for (ulInd = 1; ulInd < ulLen; ulInd++)
   {
      oToken = DynArray_get(oTokens, ulInd);

      /* Handle special tokens. */
      if (Token_getType(oToken) == TOKEN_SPECIAL)
      {
         /* If the token is '<', increment stdin counter. */
         if (Token_isStdIn(oToken))
         {
            ulStdInCount++;
            /* Check for multiple stdin-redirection. */
            if (ulStdInCount > 1)
            {
               fprintf(stderr,
                       "%s: multiple redirection of standard input\n",
                       getPgmName());
               if (oArgs != NULL) DynArray_free(oArgs);
               return NULL;
            }
            /* Check if there is still a subsequent file name. */
            if ( ulInd == (ulLen - 1) )
            {
               fprintf(stderr,
                       "%s: standard input redirection without file name\n",
                       getPgmName());
               if (oArgs != NULL) DynArray_free(oArgs);
               return NULL;
            }
            /* Get stdin file name. */
            ulInd++;
            oToken = DynArray_get(oTokens, ulInd);
            if (Token_getType(oToken) == TOKEN_ORDINARY)
            pcStdIn = Token_getString(oToken);
         }
         /* If the token is '>', increment counter. */
         else if (Token_isStdOut(oToken))
         {  
            ulStdOutCount++;
            /* Check for multiple stdout-redirection. */
            if (ulStdOutCount > 1)
            {
               fprintf(stderr,
                       "%s: multiple redirection of standard output\n",
                       getPgmName());
               if (oArgs != NULL) DynArray_free(oArgs);
               return NULL;
            }
            /* Check if there is still a subsequent file name. */
            if ( ulInd == (ulLen - 1) )
            {
               fprintf(stderr,
                 "%s: standard output redirection without file name\n",
                       getPgmName());
               if (oArgs != NULL) DynArray_free(oArgs);
               return NULL;           
            }
            /* Get stdout file name. */
            ulInd++;
            oToken = DynArray_get(oTokens, ulInd);
            if (Token_getType(oToken) == TOKEN_ORDINARY)
               pcStdOut = Token_getString(oToken);
         }
         
      }
      /* Handle ordinary tokens. */
      else if (Token_getType(oToken) == TOKEN_ORDINARY)
      {
         /* Initialise dynamic array for arguments. */
         if (oArgs == NULL)
         {
            oArgs = DynArray_new(0);
            if (oArgs == NULL)
            {perror(getPgmName()); exit(EXIT_FAILURE);}
         }
         
         /* Add to oArgs. */
         DynArray_add(oArgs, Token_getString(oToken));
      }
   }

   /* Create a new command object. */
   oCommand = Command_new(pcName, oArgs, pcStdIn, pcStdOut);
   if (oCommand == NULL)
   {perror(getPgmName()); exit(EXIT_FAILURE);}

   if (oArgs != NULL) DynArray_free(oArgs);
      
   return oCommand;
}

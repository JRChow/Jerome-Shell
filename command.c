/*--------------------------------------------------------------------*/
/* command.c                                                          */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#include "command.h"
#include "ish.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*--------------------------------------------------------------------*/

/* A command consists of a name, possibly-multiple arguments, a 
   standard input file, and a standard output file. */
struct Command
{
   /* Command name. */
   char* pcName;

   /* A dynamic array of arguments. */
   DynArray_T oArgs;

   /* Standard input file. */
   char* pcStdIn;

   /* Standard output file. */
   char* pcStdOut;
};

/*--------------------------------------------------------------------*/

#ifndef NDEBUG

/* Check if the command object is ready to be returned. Returns 
   1 (TRUE) iff it is valid, and 0 (FALSE) otherwise. */

static int Command_isValid(Command_T oCommand)
{
   if (oCommand == NULL) return 0;
   if (oCommand->pcName == NULL && oCommand->oArgs == NULL &&
       oCommand->pcStdIn == NULL && oCommand->pcStdOut == NULL)
      return 0;
   return 1;
}

#endif

/*--------------------------------------------------------------------*/

/* Create and return a command object whose name is pcName, whose 
   arguments are oArgs, whose standard input file is pcStdIn and whose 
   standard output file is pcStdOut.  */

Command_T Command_new(char* pcName, DynArray_T oArgs,
                      char* pcStdIn, char* pcStdOut)
{
   Command_T oCommand;

   assert(pcName != NULL);

   /* Allocate memory for oCommand. */
   oCommand = (struct Command*)malloc(sizeof(struct Command));
   if (oCommand == NULL)
   {perror(getPgmName()); exit(EXIT_FAILURE);}


   /* Allocate memory for pcName */
   oCommand->pcName =
      (char*)malloc(sizeof(char) * (strlen(pcName)+1));
   if (oCommand->pcName == NULL)
   {perror(getPgmName()); exit(EXIT_FAILURE);}
   
   /* Copy pcName. */
   strcpy(oCommand->pcName, pcName);
   

   if (oArgs != NULL)
   {
      size_t ulInd;
      size_t ulLen = DynArray_getLength(oArgs);
      oCommand->oArgs = DynArray_new(ulLen);
      for (ulInd = 0; ulInd < ulLen; ulInd++)
      {
         char* inputStr = DynArray_get(oArgs, ulInd);
         char* copyStr =
            (char*)malloc(sizeof(char) * (strlen(inputStr)+1));
         strcpy(copyStr, inputStr);
         DynArray_set(oCommand->oArgs, ulInd, copyStr);
      }
   }
   else oCommand->oArgs = NULL;

   if (pcStdIn != NULL)
   {
      /* Allocate memory for pcStdIn */
      oCommand->pcStdIn =
         (char*)malloc(sizeof(char) * (strlen(pcStdIn)+1));
      if (oCommand->pcStdIn == NULL)
      {perror(getPgmName()); exit(EXIT_FAILURE);}
      
      /* Copy pcStdIn. */
      strcpy(oCommand->pcStdIn, pcStdIn);
   }
   else oCommand->pcStdIn = NULL;

   if (pcStdOut != NULL)
   {
      /* Allocate memory for pcStdOut */
      oCommand->pcStdOut =
         (char*)malloc(sizeof(char) *(strlen(pcStdOut) + 1));
      if (oCommand->pcStdOut == NULL)
      {perror(getPgmName()); exit(EXIT_FAILURE);}
      
      /* Copy pcStdOut. */
      strcpy(oCommand->pcStdOut, pcStdOut);
   }
   else oCommand->pcStdOut = NULL;

   return oCommand;
}

/*--------------------------------------------------------------------*/

/* Helper function for freeing one argument. */

static void freeArg(void* arg, void* pvExtra)
{
   assert(arg != NULL);
   free(arg);
}

/*--------------------------------------------------------------------*/

/* Free oCommand. */

void Command_free(Command_T oCommand)
{
   if (oCommand != NULL)
   {
      free(oCommand->pcName);
      if (oCommand->pcStdIn != NULL) free(oCommand->pcStdIn);
      if (oCommand->pcStdOut != NULL) free(oCommand->pcStdOut);
      if (oCommand->oArgs != NULL)
      {
         DynArray_map(oCommand->oArgs, freeArg, NULL);
         DynArray_free(oCommand->oArgs);
      }
      free(oCommand);
   }
}

/*--------------------------------------------------------------------*/

/* Helper function for printing command objects. 
   Excerpt from precept 10.  */

static void printString(void *pvItem, void *pvFormat)
{
   assert(pvItem != NULL);
   assert(pvFormat != NULL);
   printf((char*)pvFormat, (char*)pvItem);
}

/*--------------------------------------------------------------------*/

/* Print the content of oCommand. */

void Command_print(Command_T oCommand)
{
   assert(oCommand != NULL);
   assert(Command_isValid(oCommand));

   if (oCommand->pcName != NULL)
      printf("Command name: %s\n", oCommand->pcName);
   if (oCommand->oArgs != NULL)
      DynArray_map(oCommand->oArgs, printString, "Command arg: %s\n");
   if (oCommand->pcStdIn != NULL)
      printf("Command stdin: %s\n", oCommand->pcStdIn);
   if (oCommand->pcStdOut != NULL)
      printf("Command stdout: %s\n", oCommand->pcStdOut);
}

/*--------------------------------------------------------------------*/

/* Returns the command name of oCommand. */

char* Command_getName(Command_T oCommand)
{
   assert(oCommand != NULL);
   assert(Command_isValid(oCommand));

   return oCommand->pcName;
}

/*--------------------------------------------------------------------*/

/* Returns the argument list of oCommand as an array. */

char** Command_getArgv(Command_T oCommand)
{
   char** ppcArgv;
   size_t i; /* Array iterator. */
   size_t uArgsLen;
   
   assert(oCommand != NULL);
   assert(Command_isValid(oCommand));

   if (oCommand->oArgs == NULL) uArgsLen = 0;
   else uArgsLen = DynArray_getLength(oCommand->oArgs);
   
   ppcArgv = (char**)calloc(uArgsLen + 2, sizeof(char*));
   ppcArgv[0] = oCommand->pcName;
   for (i = 1; i < uArgsLen+1; i++)
      ppcArgv[i] = DynArray_get(oCommand->oArgs, i-1);
   ppcArgv[i] = NULL;
   return ppcArgv;
}

/*--------------------------------------------------------------------*/

/* Returns the number of arguments of oCommand. */

size_t Command_getArgsNum(Command_T oCommand)
{
   assert(oCommand != NULL);
   assert(Command_isValid(oCommand));

   if (oCommand->oArgs == NULL) return 0;
   return DynArray_getLength(oCommand->oArgs);
}

/*--------------------------------------------------------------------*/

/* Returns a pointer to the standard input of oCommand. */

char* Command_getStdIn(Command_T oCommand)
{
   assert(oCommand != NULL);
   assert(Command_isValid(oCommand));

   return oCommand->pcStdIn;
}

/*--------------------------------------------------------------------*/

/* Returns a pointer to the standard output of oCommand. */

char* Command_getStdOut(Command_T oCommand)
{
   assert(oCommand != NULL);
   assert(Command_isValid(oCommand));

   return oCommand->pcStdOut;
}


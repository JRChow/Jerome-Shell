/*--------------------------------------------------------------------*/
/* command.h                                                          */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#ifndef COMMAND_INCLUDED
#define COMMAND_INCLUDED

#include "dynarray.h"

typedef struct Command* Command_T;

/*--------------------------------------------------------------------*/

/* Create and return a command object whose name is pcName, whose 
   arguments are oArgs, whose standard input file is pcStdIn and whose 
   standard output file is pcStdOut.  */

Command_T Command_new(char* pcName, DynArray_T oArgs,
                      char* pcStdIn, char* StdOut);

/*--------------------------------------------------------------------*/

/* Free oCommand. */

void Command_free(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Print the content of oCommand. */

void Command_print(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Get the command name. */

char* Command_getName(Command_T oCommand);

/*--------------------------------------------------------------------*/
   
/* Store the arguments of oCommand into ppcArgv. */
   
char** Command_getArgv(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Returns the number of arguments of oCommand. */

size_t Command_getArgsNum(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Returns a pointer to the standard input of oCommand. */

char* Command_getStdIn(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Returns a pointer to the standard output of oCommand. */

char* Command_getStdOut(Command_T oCommand);

#endif

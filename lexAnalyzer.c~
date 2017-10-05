/*--------------------------------------------------------------------*/
/* lexAnalyzer.c                                                      */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "token.h"
#include "ish.h"
#include "lexAnalyzer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

/* If no lines remain in psFile, then return NULL. Otherwise read a line
   of psFile and return it as a string. The string does not contain a
   terminating newline character. The caller owns the string. */

char *LexAnalyzer_readLine(FILE *psFile)
{
   enum {INITIAL_LINE_LENGTH = 2};
   enum {GROWTH_FACTOR = 2};

   size_t uLineLength = 0;
   size_t uPhysLineLength = INITIAL_LINE_LENGTH;
   char *pcLine;
   int iChar;

   assert(psFile != NULL);

   /* If no lines remain, return NULL. */
   if (feof(psFile))
      return NULL;
   iChar = fgetc(psFile);
   if (iChar == EOF)
      return NULL;

   /* Allocate memory for the string. */
   pcLine = (char*)malloc(uPhysLineLength);
   if (pcLine == NULL)
   {perror(getPgmName()); exit(EXIT_FAILURE);}

   /* Read characters into the string. */
   while ((iChar != '\n') && (iChar != EOF))
   {
      if (uLineLength == uPhysLineLength)
      {
         uPhysLineLength *= GROWTH_FACTOR;
         pcLine = (char*)realloc(pcLine, uPhysLineLength);
         if (pcLine == NULL)
            {perror(getPgmName()); exit(EXIT_FAILURE);}
      }
      pcLine[uLineLength] = (char)iChar;
      uLineLength++;
      iChar = fgetc(psFile);
   }

   /* Append a null character to the string. */
   if (uLineLength == uPhysLineLength)
   {
      uPhysLineLength++;
      pcLine = (char*)realloc(pcLine, uPhysLineLength);
      if (pcLine == NULL)
         {perror(getPgmName()); exit(EXIT_FAILURE);}
   }
   pcLine[uLineLength] = '\0';

   return pcLine;
}

/*--------------------------------------------------------------------*/

/* Write all tokens in oTokens to stdout. */

void LexAnalyzer_writeTokens(DynArray_T oTokens)
{  
   size_t u;
   size_t uLength;
   Token_T oToken;

   assert(oTokens != NULL);

   uLength = DynArray_getLength(oTokens);

   for (u = 0; u < uLength; u++)
   {
      oToken = DynArray_get(oTokens, u);
      Token_print(oToken);
   }
}

/*--------------------------------------------------------------------*/

/* Free all of the tokens in oTokens. */

void LexAnalyzer_freeTokens(DynArray_T oTokens)
{
   size_t u;
   size_t uLength;
   Token_T oToken;

   assert(oTokens != NULL);

   uLength = DynArray_getLength(oTokens);

   for (u = 0; u < uLength; u++)
   {
      oToken = DynArray_get(oTokens, u);
      Token_free(oToken);
   }
}


/*--------------------------------------------------------------------*/

/* Lexically analyze string pcLine.  If pcLine contains a lexical
   error, then return NULL.  Otherwise return a DynArray object
   containing the tokens in pcLine.  The caller owns the DynArray
   object and the tokens that it contains. */

DynArray_T LexAnalyzer_lexLine(const char *pcLine)
{
   /* lexLine() uses a DFA approach.  It "reads" its characters from
      pcLine. The DFA has these four states: */
   enum LexState {STATE_START, STATE_IN_QUOTE, STATE_OUT_QUOTE,
                  STATE_SPECIAL, STATE_ORDINARY};

   /* The current state of the DFA. */
   enum LexState eState = STATE_START;

   /* An index into pcLine. */
   size_t uLineIndex = 0;

   /* Pointer to a buffer in which the characters comprising each
      token are accumulated. */
   char *pcBuffer;

   /* An index into the buffer. */
   int uBufferIndex = 0;

   char c;
   Token_T oToken;
   DynArray_T oTokens;
   int iSuccessful;

   assert(pcLine != NULL);

   /* Create an empty token DynArray object. */
   oTokens = DynArray_new(0);
   if (oTokens == NULL)
   {perror(getPgmName()); exit(EXIT_FAILURE);}

   /* Allocate memory for a buffer that is large enough to store the
      largest token that might appear within pcLine. */
   pcBuffer = (char*)malloc(strlen(pcLine) + 1);
   if (pcBuffer == NULL)
      {perror(getPgmName()); exit(EXIT_FAILURE);}

   for (;;)
   {
      /* "Read" the next character from pcLine. */
      c = pcLine[uLineIndex++];

      switch (eState)
      {
         /* Handle the START state. */
         case STATE_START:
            if (c == '\0')
            {
               free(pcBuffer);
               return oTokens;
            }
            else if (c == '\"')
               eState = STATE_IN_QUOTE;
            else if (isspace(c))
               eState = STATE_START;
            else if (c == '<' || c == '>')
            {
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_SPECIAL;
            }
            else
            {
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_ORDINARY;
            }
            break;

         /* Handle the ORDINARY state. */
         case STATE_ORDINARY:
            if (c == '\0')
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               free(pcBuffer);
               return oTokens;
            }
            else if (c == '<' || c == '>')
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_SPECIAL;
            }
            else if (isspace(c))
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               eState = STATE_START;
            }
            else if (c == '\"')
               eState = STATE_IN_QUOTE;            
            else
            {
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_ORDINARY;
            }
            break;

         /* Handle the SPECIAL state. */
         case STATE_SPECIAL:
            if (c == '\0')
            {
               /* Create a SPECIAL token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_SPECIAL, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               free(pcBuffer);
               return oTokens;
            }
            else if (c == '<' || c == '>')
            {
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_SPECIAL;
            }
            else if (isspace(c))
            {
               /* Create a SPECIAL token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_SPECIAL, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               eState = STATE_START;
            }
            else if (c == '\"')
            {
               /* Create a SPECIAL token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_SPECIAL, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               eState = STATE_IN_QUOTE;            
            }
            else
            {
               /* Create a SPECIAL token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_SPECIAL, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
               {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_ORDINARY;            
            }
            break;

            /* Handle the IN_QUOTE state. */
         case STATE_IN_QUOTE:
            if (c == '\0')
            {
               fprintf(stderr, "%s: unmatched quote\n", getPgmName());
               free(pcBuffer);
               LexAnalyzer_freeTokens(oTokens);
               DynArray_free(oTokens); 
               return NULL;
            }
            else if (c == '\"')
               eState = STATE_OUT_QUOTE;
            else
            {
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_IN_QUOTE;
            }
            break;

         /* Handle the OUT_QUOTE state. */
         case STATE_OUT_QUOTE:
            if (c == '\0')
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               free(pcBuffer);
               return oTokens;
            }
            else if (c == '<' || c == '>')
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               eState = STATE_SPECIAL;
            }
            else if (c == '\"')
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               eState = STATE_IN_QUOTE;
            }
            else if (isspace(c))
            {
               /* Create an ORDINARY token. */
               pcBuffer[uBufferIndex] = '\0';
               oToken = Token_new(TOKEN_ORDINARY, pcBuffer);
               iSuccessful = DynArray_add(oTokens, oToken);
               if (! iSuccessful)
                  {perror(getPgmName()); exit(EXIT_FAILURE);}
               uBufferIndex = 0;
               eState = STATE_START;
            }
            else
            {
               pcBuffer[uBufferIndex++] = c;
               eState = STATE_ORDINARY;
            }
            break;

         default:
            assert(0);
      }
   }
}


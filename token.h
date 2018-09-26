/*--------------------------------------------------------------------*/
/* token.h                                                            */
/* Author: Jingran Zhou                                               */
/*--------------------------------------------------------------------*/

#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

typedef struct Token* Token_T;

/*--------------------------------------------------------------------*/

/* Define token types. */

enum TokenType {TOKEN_ORDINARY, TOKEN_SPECIAL};

/*--------------------------------------------------------------------*/

/* Return the token type of oToken. */

enum TokenType Token_getType(Token_T oToken);

/*--------------------------------------------------------------------*/

/* Return the value of oToken, which is a string. */

char* Token_getString(Token_T oToken);

/*--------------------------------------------------------------------*/

/* Set the type of oToken to eNewType. */

void Token_setType(Token_T oToken, enum TokenType eNewType);

/*--------------------------------------------------------------------*/

/* Print oToken properly. */

void Token_print(Token_T oToken);

/*--------------------------------------------------------------------*/

/* Create and return a token whose type is eTokenType and whose
   value consists of string pcValue.  The caller owns the token. */

Token_T Token_new(enum TokenType eTokenType,
                  char *pcValue);

/*--------------------------------------------------------------------*/

/* Free oToken. */

void Token_free(Token_T oToken);

/*--------------------------------------------------------------------*/

/* Returns 1 (TRUE) if oToken is a stdin-redirect token. */

int Token_isStdIn(Token_T oToken);

/*--------------------------------------------------------------------*/

/* Returns 1 (TRUE) if oToken is a stdout-redirect token. */

int Token_isStdOut(Token_T oToken);

#endif

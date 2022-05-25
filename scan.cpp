/****************************************************/
/* File: scan.c                                     */
/* The scanner implementation for the CMINUS compiler*/
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"

/* states in scanner DFA */
typedef enum
{
	START, INRCOM, INLCOM, INCOMMENT, INNUM, INID, DONE, INNEQ, WAITEQ, WAITBACK
}
StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN + 1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void) {
	if (!(linepos < bufsize)) {
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source)) {
			if (EchoSource) fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else {
			EOF_flag = TRUE;
			return EOF;
		}
	}
	else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void) {
	if (!EOF_flag) linepos--;
}

/* lookup table of reserved words */
static struct {
	char* str;
	TokenType tok;
} reservedWords[MAXRESERVED] = {
	{(char*)"if",IF},{(char*)"int",INT},{(char*)"else",ELSE},{(char*)"return",RETURN},
	{(char*)"void",VOID},{(char*)"while",WHILE}
};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup(char* s) {
	int i;
	for (i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */

TokenType getToken() {
	int tokenStringpos = 0;
	TokenType currentToken;
	StateType state = START;
	bool save;
	int waiteqbuffer = 0;

	while (state != DONE) {
		int c = getNextChar();
		save = true;
		switch (state) {
		case START:
			if (isdigit(c)) {
				state = INNUM;
			}
			else if (isalpha(c)) {
				state = INID;
			}
			else if (c == '~') {
				state = INNEQ;
			}
			else if ((c == ' ') || (c == '\t') || (c == '\n')) {
				save = false;
			}
			else if ((c == '>') || (c == '<') || (c == '=')) {
				state = WAITEQ;
				waiteqbuffer = c;
			}
			else if (c == '/') {
				state = INLCOM;
			}
			else {
				state = DONE;
				switch (c) {
				case EOF:
					save = false;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case',':
					currentToken = COMMA;
					break;
				case'[':
					currentToken = LBRK;
					break;
				case']':
					currentToken = RBRK;
					break;
				case'{':
					currentToken = LBRACE;
					break;
				case'}':
					currentToken = RBRACE;
					break;
				case'-':
					currentToken = MINUS;
					break;
				case'*':
					currentToken = TIMES;
					break;
				case'(':
					currentToken = LPAREN;
					break;
				case')':
					currentToken = RPAREN;
					break;
				case';':
					currentToken = SEMI;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INLCOM:
			save = false;
			if (c == '*') {
				tokenStringpos--;
				tokenString[tokenStringpos] = 0;
				state = INCOMMENT;
			}
			else {
				state = DONE;
				ungetNextChar();
				currentToken = OVER;
			}
			break;
		case INCOMMENT:
			save = false;
			if (c == EOF) {
				currentToken = ENDFILE;
			}
			else if (c == '*') {
				state = WAITBACK;
			}
			break;
		case WAITBACK:
			save = false;
			if (c == '*')
				state = WAITBACK;
			else if (c == '/') {
				state = START;
			}
			else {
				state = INCOMMENT;
			}
			break;
		case WAITEQ:
			state = DONE;
			if (c == '=') {
				if (waiteqbuffer == '<') {
					currentToken = LTE;
				}
				else if (waiteqbuffer == '>') {
					currentToken = GTE;
				}
				else if (waiteqbuffer == '=') {
					currentToken = EQ;
				}
				else {
					currentToken = ERROR;
				}
			}
			else {
				save = false;
				ungetNextChar();
				if (waiteqbuffer == '<') {
					currentToken = LT;
				}
				else if (waiteqbuffer == '>') {
					currentToken = GT;
				}
				else if (waiteqbuffer == '=') {
					currentToken = ASSIGN;
				}
				else {
					currentToken = ERROR;
				}
			}
			break;
		case INNEQ:
			state = DONE;
			if (c == '=') {
				state = DONE;
				currentToken = NEQ;
			}
			else {
				ungetNextChar();
				save = false;
				currentToken = ERROR;
			}
			break;
		case INNUM:
			if (!isdigit(c)) {
				state = DONE;
				ungetNextChar();
				save = false;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c)) {
				state = DONE;
				ungetNextChar();
				save = false;
				currentToken = ID;
			}
			break;
		case DONE:
		default:
			fprintf(listing, "ÀèÇ¬Â¡ 2019141460301 Scanner Bug:state=%d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}
		if (save && tokenStringpos < MAXTOKENLEN)
			tokenString[tokenStringpos++] = (char)c;
		if (state == DONE) {
			tokenString[tokenStringpos] = '\0';
			if (currentToken == ID) {
				currentToken = reservedLookup(tokenString);
			}
		}
	}
	if (TraceScan) {
		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
}

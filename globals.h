/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for CMINUS compiler        */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stack>
#include <iostream>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 6 //关键字最大程度 
#define MAXTOKENLEN 40 //标识符最大长度 

typedef enum /* book-keeping tokens */
{
	ENDFILE, ERROR,
	/* reserved words */
	IF, ELSE, INT, RETURN, VOID, WHILE,
	/* multicharacter tokens */
	ID, NUM,
	/* special symbols */
	/*[]{} >= <= ~= == = < > + - * / () ; , */
	LBRK, RBRK, LBRACE, RBRACE, GTE, LTE, NEQ, EQ, ASSIGN, LT, GT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI, COMMA
} TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum { StmtK, ExpK } NodeKind;//结点类型

typedef enum
{
	IfK, WhileK, AssignK, ReturnK, IntK, VoidK, ParamsK, ParamK, CompK, Var_DeclK, Arry_DeclK, FuncK, ArgsK
} StmtKind;//Stmt类型细分(关键字操作)

typedef enum
{
	OpK, ConstK, IdK, CallK, Arry_ElemK
} ExpKind;//Exp类型细分(运算符与变量，数字)

#define MAXCHILDREN 4

typedef struct treeNode {
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;
	int lineno;
	NodeKind nodekind;
	union {
		StmtKind stmt;
		ExpKind exp;
	} kind;
	union {
		TokenType op;
		int val;
		char* name;
	} attr;
	//	ExpType type; /* for type checking of exps */
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif
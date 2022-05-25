/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the CMINUS compiler                          */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
 /* Procedure printToken prints a token
  * and its lexeme to the listing file
  */
void printToken(TokenType token, const char* tokenString) {
	switch (token) {
	case IF:
	case RETURN:
	case ELSE:
	case VOID:
	case WHILE:
	case INT:
		fprintf(listing,
			"reserved word: %s\n", tokenString);
		break;
	case EQ:
		fprintf(listing, "==\n");
		break;
	case COMMA:
		fprintf(listing, ",\n");
		break;
	case LBRK:
		fprintf(listing, "[\n");
		break;
	case RBRK:
		fprintf(listing, "]\n");
		break;
	case LBRACE:
		fprintf(listing, "{\n");
		break;
	case RBRACE:
		fprintf(listing, "}\n");
		break;
	case LTE:
		fprintf(listing, "<=\n");
		break;
	case GTE:
		fprintf(listing, ">=\n");
		break;
	case LT:
		fprintf(listing, "<\n");
		break;
	case GT:
		fprintf(listing, ">\n");
		break;
	case NEQ:
		fprintf(listing, "~=\n");
		break;
	case ASSIGN:
		fprintf(listing, "=\n");
		break;
	case LPAREN:
		fprintf(listing, "(\n");
		break;
	case RPAREN:
		fprintf(listing, ")\n");
		break;
	case SEMI:
		fprintf(listing, ";\n");
		break;
	case PLUS:
		fprintf(listing, "+\n");
		break;
	case MINUS:
		fprintf(listing, "-\n");
		break;
	case TIMES:
		fprintf(listing, "*\n");
		break;
	case OVER:
		fprintf(listing, "/\n");
		break;
	case ENDFILE:
		fprintf(listing, "EOF\n");
		break;
	case NUM:
		fprintf(listing,
			"NUM, val= %s\n", tokenString);
		break;

	case ID:
		fprintf(listing,
			"ID, name= %s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing,
			"ERROR: %s\n", tokenString);
		break;

	default: /* should never happen */
		fprintf(listing, "Unknown token: %d\n", token);
	}
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode* newStmtNode(StmtKind kind) {
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode* newExpNode(ExpKind kind) {
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL) {
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	}
	else {
		for (i = 0; i < MAXCHILDREN; i++) {
			t->child[i] = NULL;
		}
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
	}
	return t;
}


/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char* copyString(char* s) {
	int n;
	char* t;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = (char*)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void) {
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode* tree) {
	int i;
	INDENT;
	while (tree != NULL) {
		printSpaces();
		if (tree->nodekind == StmtK) {
			switch (tree->kind.stmt) {
			case IfK:
				fprintf(listing, "If\n");
				break;
			case WhileK:
				fprintf(listing, "While\n");
				break;
			case AssignK:
				fprintf(listing, "Assign\n");
				break;
			case ReturnK:
				fprintf(listing, "Return\n");
				break;
			case IntK:
				fprintf(listing, "IntK\n");
				break;
			case VoidK:
				fprintf(listing, "VoidK\n");
				break;
			case ParamsK:
				fprintf(listing, "ParamsK\n");
				break;
			case ParamK:
				fprintf(listing, "ParamK\n");
				break;
			case CompK:
				fprintf(listing, "CompK\n");
				break;
			case Var_DeclK:
				fprintf(listing, "Var_DeclK\n");
				break;
			case Arry_DeclK:
				fprintf(listing, "Arry_DeclK\n");
				break;
			case FuncK:
				fprintf(listing, "FuncK\n");
				break;
			case ArgsK:
				fprintf(listing, "ArgsK\n");
				break;
			default:
				fprintf(listing, "Unknown StmtNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK) {
			switch (tree->kind.exp) {
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstK:
				fprintf(listing, "ConstK: %d\n", tree->attr.val);
				break;
			case IdK:
				if (tree->attr.name != NULL) {
					fprintf(listing, "IdK: %s\n", tree->attr.name);
				}
				else {
					fprintf(listing, "IdK:\n");
				}

				break;
			case CallK:
				fprintf(listing, "CallK\n");
				break;
			case Arry_ElemK:
				fprintf(listing, "Arry_ElemK\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else fprintf(listing, "Unknown node kind\n");
		for (i = 0; i < MAXCHILDREN; i++) {
			ExpKind x;
			if (tree->nodekind == ExpK) {
				x = tree->kind.exp;
			}
			StmtKind y;
			if (tree->nodekind == StmtK) {
				y = tree->kind.stmt;
			}
			printTree(tree->child[i]);

		}
		tree = tree->sibling;
	}
	UNINDENT;
}

void changeExpNode(TreeNode* ExpNode, ExpKind kind) {
	ExpNode->kind.exp = kind;
}

void changeStmtNode(TreeNode* StmtNode, StmtKind kind) {
	StmtNode->kind.stmt = kind;
}



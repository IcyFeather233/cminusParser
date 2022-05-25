/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the CMINUS compiler*/
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* program();
static TreeNode* declaration();
static TreeNode* declaration2();
static TreeNode* type_specifier();
static TreeNode* params();
static TreeNode* param_list1();
static TreeNode* param_list2();
static TreeNode* param();
static TreeNode* compound_stmt();
static TreeNode* statement();
static TreeNode* expression_stmt();
static TreeNode* selection_stmt();
static TreeNode* iteration_stmt();
static TreeNode* return_stmt();
static TreeNode* expression();
static TreeNode* var();
static TreeNode* simple_expression();
static TreeNode* relop();
static TreeNode* additive_expression();
static TreeNode* addop();
static TreeNode* term();
static TreeNode* mulop();
static TreeNode* factor();
static TreeNode* factor2();
static TreeNode* args();
int Style = -1;

bool isvar = true;
bool plan2 = false;
bool plan3 = false;
std::stack<TreeNode*> ArrBuffer;
std::stack<TreeNode*> VarBuffer;

static void syntaxError(char* message) {
	fprintf(listing, "\n>>> ");
	fprintf(listing, "黎乾隆 2019141460301 Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

static void match(TokenType expected) {
	if (token == expected) {
		token = getToken();
	}
	else {
		fprintf(listing, "\n>>> ");
		fprintf(listing, "黎乾隆 2019141460301 Syntax error at line %d: unexpected token ->", lineno);
		printToken(token, tokenString);
		fprintf(listing, "      ");
	}
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */

TreeNode* program() {
	TreeNode* t = declaration();
	TreeNode* p = t;
	while (token == INT || token == VOID) {
		TreeNode* q;
		q = declaration();
		if (q != NULL) {
			if (t == NULL) {
				t = p = q;
			}
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

TreeNode* declaration() {
	TreeNode* t = newStmtNode(Var_DeclK);
	if (t != NULL) {
		t->child[0] = type_specifier();

		TreeNode* p = newExpNode(IdK);
		p->attr.name = copyString(tokenString);
		match(ID);

		t->child[1] = declaration2();
		if (Style == 0) {
			t->child[1]->child[0] = p;
			Style = -1;
		}
		else if (Style == 1) {
			changeStmtNode(t, FuncK);
			t->child[2] = t->child[1];
			t->child[1] = p;
			Style = -1;
		}
		else {
			t->child[1] = p;
		}
	}
	return t;
}

TreeNode* declaration2() {
	TreeNode* t = NULL;
	TreeNode* p = NULL;
	switch (token) {
	case SEMI:
		match(SEMI);
		break;

	case LBRK:
		Style = 0;
		match(LBRK);
		t = newStmtNode(Arry_DeclK);
		t->child[1] = newExpNode(ConstK);
		if (t->child[1] != NULL) {
			t->child[1]->attr.val = atoi(tokenString);
		}
		match(NUM);
		match(RBRK);
		match(SEMI);
		break;

	case LPAREN:
		Style = 1;
		match(LPAREN);
		t = params();
		match(RPAREN);
		p = compound_stmt();
		if (t != NULL) {
			t->sibling = p;
		}
		break;

	default:
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* type_specifier() {
	TreeNode* t = NULL;
	if (token == INT) {
		t = newStmtNode(IntK);
		match(INT);
	}
	else if (token == VOID) {
		t = newStmtNode(VoidK);
		match(VOID);
	}
	else {
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

TreeNode* params() {
	TreeNode* t = newStmtNode(ParamsK);
	if (token == INT) {
		t->child[0] = param_list2();
	}
	else if (token == VOID) {
		t->child[0] = param_list1();
	}
	else {
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

TreeNode* param_list1() {
	TreeNode* t = newStmtNode(ParamK);
	TreeNode* p = t;
	if (t != NULL) {
		t->child[0] = newStmtNode(VoidK);
		match(VOID);
		if (token == ID) {
			t->child[1] = newExpNode(IdK);
			t->child[1]->attr.name = copyString(tokenString);
			match(ID);
			if (token == LBRK) {
				match(LBRK);
				t->child[2] = newExpNode(IdK);
				match(RBRK);
			}
			while (token == COMMA) {
				match(COMMA);
				TreeNode* q = param();
				p->sibling = q;
				p = q;
			}
		}
		else {
			p = t->child[0];
			t->child[0] = NULL;
			t = p;
		}
	}
	return t;
}

TreeNode* param_list2() {
	TreeNode* t = newStmtNode(ParamK);
	TreeNode* p = t;
	if (t != NULL) {
		t->child[0] = newStmtNode(IntK);
		match(INT);
		t->child[1] = newExpNode(IdK);
		t->child[1]->attr.name = copyString(tokenString);
		match(ID);
		if (token == LBRK) {
			match(LBRK);
			t->child[2] = newExpNode(IdK);
			t->child[2]->attr.name = NULL;
			match(RBRK);
		}
		while (token == COMMA) {
			match(COMMA);
			TreeNode* q = param();
			p->sibling = q;
			p = q;
		}
	}
	return t;
}

TreeNode* param() {
	TreeNode* t = newStmtNode(ParamK);
	if (t != NULL) {
		t->child[0] = type_specifier();
		t->child[1] = newExpNode(IdK);
		t->child[1]->attr.name = copyString(tokenString);
		match(ID);
		if (token == LBRK) {
			match(LBRK);
			t->child[2] = newExpNode(IdK);
			match(RBRK);
		}
	}
	return t;
}

TreeNode* compound_stmt() {
	TreeNode* t = newStmtNode(CompK);
	TreeNode* p = NULL;
	if (t != NULL) {
		match(LBRACE);
		while (token == INT || token == VOID) {
			TreeNode* q = newStmtNode(Var_DeclK);
			q->child[0] = type_specifier();
			q->child[1] = newExpNode(IdK);
			q->child[1]->attr.name = copyString(tokenString);
			match(ID);
			if (token == LBRK) {
				match(LBRK);
				TreeNode* n = newStmtNode(Arry_DeclK);
				if (n != NULL) {
					n->child[0] = q->child[1];
					n->child[1] = newExpNode(ConstK);
					n->child[1]->attr.val = atoi(tokenString);
				}
				match(NUM);
				match(RBRK);
			}
			match(SEMI);

			if (p == NULL) {
				p = q;
				t->child[0] = p;
			}
			else {
				p->sibling = q;
				p = q;
			}
		}
		p = NULL;
		while (token == ID || token == LBRACE || token == SEMI || token == LPAREN ||
			token == IF || token == WHILE || token == RETURN || token == NUM) {
			TreeNode* q = statement();
			if (p == NULL) {
				p = q;
				t->child[1] = p;
			}
			else if (q != NULL) {
				p->sibling = q;
				p = q;
			}
		}
		match(RBRACE);
	}
	return t;
}

TreeNode* statement() {
	TreeNode* t = NULL;
	switch (token) {
	case ID:
	case SEMI:
	case LPAREN:
	case NUM:
		t = expression_stmt();
		break;
	case LBRACE:
		t = compound_stmt();
		break;
	case IF:
		t = selection_stmt();
		break;
	case WHILE:
		t = iteration_stmt();
		break;
	case RETURN:
		t = return_stmt();
		break;
	default:
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* expression_stmt() {
	TreeNode* t = NULL;
	if (token == SEMI) {
		match(SEMI);
	}
	else {
		t = expression();
		match(SEMI);
	}

	return t;
}

TreeNode* selection_stmt() {
	TreeNode* t = newStmtNode(IfK);
	match(IF);
	match(LPAREN);
	t->child[0] = expression();

	match(RPAREN);
	t->child[1] = statement();
	if (token == ELSE) {
		match(ELSE);
		t->child[2] = statement();
	}
	return t;
}

TreeNode* iteration_stmt() {
	TreeNode* t = newStmtNode(WhileK);
	match(WHILE);
	match(LPAREN);
	t->child[0] = expression();
	match(RPAREN);
	t->child[1] = statement();
	return t;
}

TreeNode* return_stmt() {
	TreeNode* t = newStmtNode(ReturnK);
	match(RETURN);
	if (token == LPAREN || token == ID || token == NUM) {
		t->child[0] = expression();
	}
	match(SEMI);
	return t;
}

TreeNode* expression() {
	TreeNode* t = NULL;
	TreeNode* p = NULL;
	if (token == ID) {
		t = var();//--------------
		if (!isvar) {
			t = simple_expression();
		}
		else {
			match(ASSIGN);
			p = newStmtNode(AssignK);
			p->child[0] = t;
			t = p;
			t->child[1] = expression();
		}
	}
	else if (token == LPAREN || token == NUM) {
		t = simple_expression();
	}
	return t;
}

TreeNode* var() {
	isvar = true;
	TreeNode* t = newExpNode(IdK);
	TreeNode* p = t;
	t->attr.name = copyString(tokenString);
	match(ID);
	if (token == ASSIGN) { //是VAR
		isvar = true;
		return t;
	}
	else if (token == LBRK) {
		t = newExpNode(Arry_ElemK);
		t->child[0] = p;
		match(LBRK);
		t->child[1] = expression();
		match(RBRK);
		if (token == ASSIGN) { //是VAR
			isvar = true;
			return t;
		}
		else { //不是VAR
			plan2 = true;
			isvar = false;
			ArrBuffer.push(t);//----------
			return NULL;
		}
	}
	else { //不是VAR
		plan3 = true;
		VarBuffer.push(t);
		isvar = false;
		return NULL;
	}
}

TreeNode* simple_expression() {
	TreeNode* t = additive_expression();
	TreeNode* p = t;
	if (token == LTE || token == GTE || token == EQ || token == LT ||
		token == GT || token == NEQ) {
		t = relop();
		t->child[0] = p;
		t->child[1] = additive_expression();
	}
	return t;
}

TreeNode* relop() {
	TreeNode* t = newExpNode(OpK);
	switch (token) {
	case LTE:
		t->attr.op = LTE;
		match(LTE);
		break;
	case GTE:
		t->attr.op = GTE;
		match(GTE);
		break;
	case EQ:
		t->attr.op = EQ;
		match(EQ);
		break;
	case LT:
		t->attr.op = LT;
		match(LT);
		break;
	case GT:
		t->attr.op = GT;
		match(GT);
		break;
	case NEQ:
		t->attr.op = NEQ;
		match(NEQ);
		break;
	default:
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* additive_expression() {
	TreeNode* t = term();
	while (token == PLUS || token == MINUS) {
		TreeNode* q = addop();
		q->child[0] = t;
		q->child[1] = term();
		t = q;
	}
	return t;
}

TreeNode* addop() {
	TreeNode* t = newExpNode(OpK);
	switch (token) {
	case PLUS:
		t->attr.op = PLUS;
		match(PLUS);
		break;
	case MINUS:
		t->attr.op = MINUS;
		match(MINUS);
		break;
	default:
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* term() {
	if (plan2 == false) {
		TreeNode* t = factor();
		while (token == TIMES || token == OVER) {
			TreeNode* q = mulop();
			q->child[0] = t;
			q->child[1] = factor();
			t = q;
		}
		return t;
	}
	else {
		TreeNode* t = ArrBuffer.top();
		ArrBuffer.pop();
		plan2 = false;
		while (token == TIMES || token == OVER) {
			TreeNode* q = mulop();
			q->child[0] = t;
			q->child[1] = factor();
			t = q;
		}
		return t;
	}

}

TreeNode* mulop() {
	TreeNode* t = newExpNode(OpK);
	switch (token) {
	case TIMES:
		t->attr.op = TIMES;
		match(TIMES);
		break;
	case OVER:
		t->attr.op = OVER;
		match(OVER);
		break;
	default:
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* factor() {
	TreeNode* t = NULL;
	TreeNode* p = NULL;
	if (plan3 == true) {
		t = VarBuffer.top();
		VarBuffer.pop();
		p = factor2();
		if (p != NULL) {
			p->child[0] = t;
			t = p;
		}
		plan3 = false;
		return t;
	}
	switch (token) {
	case ID:
		t = newExpNode(IdK);
		t->attr.name = copyString(tokenString);
		match(ID);
		p = factor2();
		if (p != NULL) {
			p->child[0] = t;
			t = p;
		}
		break;
	case LPAREN:
		match(LPAREN);
		t = expression();
		match(RPAREN);
		break;
	case NUM:
		t = newExpNode(ConstK);
		t->attr.val = atoi(tokenString);
		match(NUM);
		break;
	default:
		syntaxError((char*)"unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode* factor2() {
	TreeNode* t = NULL;
	switch (token) {
	case LBRK:
		match(LBRK);
		t = newExpNode(Arry_ElemK);
		t->child[1] = expression();
		match(RBRK);
		break;
	case LPAREN:
		match(LPAREN);
		t = newExpNode(CallK);
		t->child[1] = args();
		match(RPAREN);
		break;
	default:
		return NULL;
		break;
	}
	return t;
}

TreeNode* args() {
	TreeNode* t = newStmtNode(ArgsK);
	TreeNode* p = t;
	if (token == ID || token == LPAREN || token == NUM) {
		t->child[0] = expression();
		if (t->child[0] != NULL)
			p = t->child[0];
		while (token == COMMA) {
			match(COMMA);
			TreeNode* q = expression();
			p->sibling = q;
			p = q;
		}
	}
	else {
		return NULL;
	}

	return t;
}

TreeNode* parse() {
	TreeNode* t;
	token = getToken();
	t = program();
	if (token != ENDFILE)
		syntaxError((char*)"Code ends before file\n");
	return t;
}
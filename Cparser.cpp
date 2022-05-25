/****************************************************/
/* File: main.c                                     */
/* Main program for CMINUS compiler                 */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"

/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE TRUE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE TRUE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif
using namespace std;
/* allocate global variables */
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

int main(int argc, char* argv[]) {
	TreeNode* syntaxTree;
	char pgm[120]; /* source code file name */
	char* pgm_output = NULL;
	/*memset(pgm_output, 0, sizeof(pgm_output));
	memset(pgm, 0, sizeof(pgm));*/
    if (argc != 2) {
        fprintf(stderr,"usage: %s <filename>\n",argv[0]);
        exit(1);
    }

	strcpy(pgm,argv[1]);
	if (strchr (pgm, '.') == NULL)
		strcat(pgm,".c-");
	int len = strlen(pgm);
	if (strchr(pgm, '.') == NULL) {
	    pgm_output = (char*)malloc(len+5);
	    strcpy(pgm_output, pgm);
	    strcat(pgm, ".c-");
	    strcat(pgm_output, ".txt");
	}
	else {//将.c-替换成.txt
	    pgm_output = (char*)malloc(len+2);
	    strcpy(pgm_output, pgm);
	    pgm_output[len - 2] = 't';
	    pgm_output[len-1] = 'x';
	    strcat(pgm_output, "t");
	}
	source = fopen(pgm, "r");
	if (source == NULL) {
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}
	//strncpy(pgm_output, pgm, strlen(pgm) - 3);
	//strcat(outfile, ".txt");
	printf("\noutputfile:%s\n", pgm_output);
	listing = fopen(pgm_output, "w");
#if NO_PARSE
	while (getToken() != ENDFILE);
#else
	syntaxTree = parse();
	if (TraceParse) {
        fprintf(listing, "CMINUS PARSING:\n\n");
		fprintf(listing, "Syntax tree:\n");
		printTree(syntaxTree);
	}
#endif
	free(pgm_output);
	fclose(source);
	return 0;
}
#ifndef GRAMATICA_H_INCLUDED
#define GRAMATICA_H_INCLUDED

#ifndef STDARG_H
#define STDARG_H
#include<stdarg.h>
#endif

#include "no_terminales.h"
#include "lex.h"

typedef enum{
	GENERATE, POP, SCAN, BLANK
}t_stack_command;

typedef enum{
	TERMINAL, NO_TERMINAL
}t_production_type;

struct grammatica {
	t_stack_command op;
	struct production *production;
};

struct production{
	t_production_type type;
	int index;
	struct production *next;
	struct production *last;
};

/* La pila con la que operaremos la gramatica */
struct t_stack{
	struct production *data;
	struct t_stack *next;
};

unsigned short int accept;
unsigned short int reported;
unsigned short int gLinea;
unsigned short int gDebug;
struct t_token *input;
struct t_stack *stack;
void stackPush(struct production *data);
void stackPop();
void stackStart();
void stackPrint();
void match();
void inputPop();
void errorMatch();
void errorGenerate();
void generate();
void printInput();
void debugPrint();
struct production *getReversedProduction();

char *terminalString(int);
char *noTerminalString(int);

void setBlankProduction(int, int terminals, ...);
void setPopProduction(int, int terminals, ...);
void clone_production(size_t M,struct production prod[M],int nt,int terminals,...);
struct production *copy_production(struct production prod,struct production *last);
void printTableRow(int non_terminal);
void destroyParsingTable();

/* Funcion que realiza el chequeo sintactico */

void syntax(struct t_token *tokens);
#define CNT 34
#define CT 22

#endif

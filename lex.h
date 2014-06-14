#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

#include "terminales.h"

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include<stdio.h>
#endif

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include<stdlib.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include<string.h>
#endif

#ifndef ERRNO_H_INCLUDED
#define ERRNO_H_INCLUDED
#include<errno.h>
#endif

#ifndef UNISTD_H_INCLNDED
#define UNISTD_H_INCLNDED
#include<unistd.h>
#endif

void help(char *);
struct t_token* lexer(FILE *);				/* Debe recibir FILE para INPUT */
FILE* check_fopen(char *,const char *);		/* Es como fopen pero imprimiendo el mensaje de error, si hubiese  */
char *get_line(FILE *,off_t **);		/* Obtiene el contenido de un FILE leyendo desde offset chars */
						/* hasta el primer \n o EOF, actualiza el offset del ultimo retorno. */
						/* Debe chequearse el buffer recibido por un EOF para romper el bucle de lectura */
/* Definimos la lista de tokens */

typedef enum{
	TERMINADOR_PUNTOCOMA, TERMINADOR_ENTER, L_PARENTESIS, R_PARENTESIS, OP_CONDICION, OP_SUMA, OP_MUL, OP_RELACIONAL,
	OP_ASIGNACION, L_CORCHETE, R_CORCHETE, COMA, DELIMITADOR_CODIGO,PR_IF,PR_TRUE,PR_FALSE,PR_ALERT, LITERAL_NUM,
	LITERAL_CADENA, ID, COMMENT, _EOF, __ERROR
}t_tokentype;	// __ERROR es un identificador especial para cuando ocurre un error en el scanner

char *tokentype(t_tokentype); // toString() para t_tokentype


/* Durante el analisis vamos a guardar los token en una cola cuyos nodos tendran la siguiente estructura */

struct t_token  {
	char *lexeme;
	t_tokentype tokentype;
	struct t_token *next_token;
	unsigned short int malloced_lexeme;
	unsigned int tableIndex;			/* Este campo nuevo nos va a indicar el indice de del token en la tabal de parsing. */
};							/* Para determinarlo, vamos a usar la nueva funcion getTokenTableIndex. */
							/* Los índices de los tokens se encuentran harcodedados en token_indexes.h */
void tokenize(struct t_token **,struct t_token **,char *);	/* Realiza la traduccion de la linea a los tokens correspondientes */
struct t_token *create_token(char *, t_tokentype);			/* Crea un token */
void add_token(struct t_token **,struct t_token **,char *, t_tokentype);	/* Agrega un token a la cola */
void token_clean(struct t_token *);							/* Limpia la cola */

// AUTOMATAS
unsigned short int automata_minus(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_lesser(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_greater(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_exclamation(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_equal(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_if(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_if_mayus(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_true(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_true_mayus(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_false(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_false_mayus(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_alert(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_alert_mayus(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_id(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_comment(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_literal_string(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_literal_number(struct t_token **,struct t_token **,char *,unsigned short int);
unsigned short int automata_comment(struct t_token **,struct t_token **,char *,unsigned short int);

char *extractor(char *,char *); /* Devuelve una cadena compuesta por los caracteres que se encuentran entre dos punteros a char */

/* Arreglos para el analisis sintactico */
int getTokenTableIndex(t_tokentype);

#endif

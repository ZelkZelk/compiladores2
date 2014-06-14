#include "gramatica.h"
#include "parsing_table.h"
#include "no_terminales.h"
#include "terminales.h"

void syntax(struct t_token *tokens){
	input = tokens;			// Tiramos los tokens a una variable global
	gDebug = 1;			// Setear a 1 para full debug
	gLinea = 1;
	reported = 0;
	accept = 1;
	/* Obtenemos la Tabla de Parsing */
	getParsingTable();

	/* Iniciamos la Pila con un COFFEESCRIPT */
	stackStart();

	if(gDebug == 1){
		printf("INICIO\n");
		debugPrint();
	}

	while(stack != NULL){
		switch(stack->data->type){
			case TERMINAL:
				if(stack->data->index == TERMINADOR_ENTER_INDEX) gLinea++;

				match();
				break;
			case NO_TERMINAL:
				generate();
				break;
		}
	}

	if(accept){
		printf("Se acepta el INPUT\n");
	}
	else{
		printf("Se rechaza el INPUT\n");

	}

	/* Frees */
	destroyParsingTable();
}

char *noTerminalString(int i){
	switch(i){
		case EXPRESION: return "exp";
		case EXPRESION2: return "exp2";
		case EXPRESION3: return "exp3";
		case EXPRESION4: return "exp4";
		case EXPRESION10: return "exp10";
		case VALOR: return "valor";
		case VALOR1: return "valor1";
		case OTRO_VALOR: return "otrovalor";
		case COMENTARIO: return "comment";
		case PARAMETROS: return "param";
		case PARAMETROS1: return "param1";
		case EXPRESION5: return "exp5";
		case TERM3: return "term3";
		case LINEA1: return "linea1";
		case TERM1: return "term1";
		case BODY1: return "body1";
		case BODY2: return "body2";
		case BODY: return "body";
		case EXPRESION6: return "exp6";
		case EXPRESION9: return "exp9";
		case TERM: return "term";
		case VALORES: return "valores";
		case VALORES1: return "valores1";
		case TERM2: return "term2";
		case CONDICIONAL: return "cond";
		case CONDICIONAL1: return "cond1";
		case COFFEESCRIPT: return "coffee";
		case TERMINADORES: return "terminadores";
		case TERMINADORES1: return "terminadores1";
		case TERMINADOR: return "terminador";
		case LINEA: return "linea";
		case OPERADOR: return "op";
		case F_ALERT: return "f_alert";
	}	
}

char *terminalString(int i){
	switch(i){
		case TERMINADOR_PUNTOCOMA_INDEX: return ";";
		case TERMINADOR_ENTER_INDEX: return "ENTER";
		case L_PARENTESIS_INDEX: return "(";
		case R_PARENTESIS_INDEX: return ")";
		case OP_CONDICION_INDEX: return "?";
		case OP_SUMA_INDEX: return "SUMA";
		case OP_MUL_INDEX: return "MUL";
		case OP_RELACIONAL_INDEX: return "REL";
		case OP_ASIGNACION_INDEX: return "=";
		case L_CORCHETE_INDEX: return "[";
		case R_CORCHETE_INDEX : return "]";
		case COMA_INDEX: return ",";
		case DELIMITADOR_CODIGO_INDEX : return "->";
		case PR_IF_INDEX :  return "IF";
		case PR_TRUE_INDEX: return "TRUE";
		case PR_FALSE_INDEX: return "FALSE";
		case PR_ALERT_INDEX : return "ALERT";
		case LITERAL_NUM_INDEX : return "NUM";
		case LITERAL_CADENA_INDEX:return "CADENA";
		case ID_INDEX : return "ID";
		case COMMENT_INDEX:return "COMMENT";
		case _EOF_INDEX: return "$";
		default: return "ERROR";
	}
}

void match(){
	if(stack->data->index == input->tableIndex){

		if(gDebug == 1) {
			printf("MATCH\t\t%s\n",terminalString(stack->data->index));
		}

		reported = 0;
		stackPop();
		inputPop();
		debugPrint();
	}
	else{
		errorMatch();
	}
}

void generate(){
	struct production *reversed;
	struct production *iterator;

	switch(table[stack->data->index][input->tableIndex].op){
		case GENERATE:
			reported = 0;
			reversed = getReversedProduction();

			if(gDebug == 1) {
				printf("GENERAR \t%s,%s = ",noTerminalString(stack->data->index),terminalString(input->tableIndex));

				for(iterator = reversed;iterator != NULL;iterator = iterator->last){
					if(iterator->type == TERMINAL){
						printf("%s ",terminalString(iterator->index));
					}
					else{
						printf("%s ",noTerminalString(iterator->index));
					}
				}

				printf("\n");
			}

			stackPop();

			for(iterator = reversed;iterator != NULL;iterator = iterator->last){
				struct production *toStack;
				toStack = copy_production(*iterator,NULL);
				stackPush(toStack);
			}
			break;
		case POP:
			if(gDebug == 1) printf("POP\n");
			if(reported == 0)printf("No se esperaba token %s en la línea %d\n",terminalString(input->tableIndex),gLinea);
			stackPop();
			reported = 1;
			accept = 0;
			break;
		case SCAN:
			if(gDebug == 1) printf("SCAN\n");
			if(reported == 0)printf("No se esperaba token %s en la línea %d\n",terminalString(input->tableIndex),gLinea);
			inputPop();
			reported = 1;
			accept = 0;
			break;
		case BLANK:
			stackPop();
			if(gDebug == 1) printf("BLANK\n");
			reported = 0;
			break;
	}

	if(gDebug == 1) debugPrint();
}

struct production *getReversedProduction(){
	struct production *head;

	for(head = table[stack->data->index][input->tableIndex].production;head != NULL;head = head->next){
		if(head->next == NULL) return head;
	}

	return NULL;
}

void printInput(){
	printf("INPUT : \t");

	struct t_token *iterator;

	for(iterator = input;iterator != NULL;iterator = iterator->next_token){
		printf("%s ",terminalString(iterator->tableIndex));
	}

	printf("\n");
}

void errorMatch(){
	if(reported == 0)printf("Se esperaba %s se encontró %s en la línea %d\n",terminalString(stack->data->index),terminalString(input->tableIndex),gLinea);
	inputPop();
	accept = 0;
	reported = 1;
}

void debugPrint(){
	if(gDebug == 1){
		stackPrint();
		printInput();
		printf("--------------------------------------------------\n");
	}
}

void stackPrint(){
	printf("STACK : \t");

	if(stack != NULL){
		struct t_stack *iterator;

		for(iterator = stack;iterator != NULL;iterator = iterator->next){

			if(iterator->data->type == TERMINAL){
				printf("%s ",terminalString(iterator->data->index));
			}
			else if(iterator->data->type == NO_TERMINAL){
				printf("%s ",noTerminalString(iterator->data->index));	
			}
		}
	}

	printf("\n");
}

void stackStart(){
	struct production *pr;
	pr = (struct production *)malloc(sizeof(struct production));
	pr->type = NO_TERMINAL;
	pr->index = COFFEESCRIPT;
	stackPush(pr);
}

void stackPush(struct production *data){
	if(stack == NULL){
		stack = (struct t_stack *)malloc(sizeof(struct t_stack));
		stack->data = data;
		stack->next = NULL;
	}
	else{
		struct t_stack *aux;
		aux = (struct t_stack *)malloc(sizeof(struct t_stack));
		aux->data = data;
		aux->next = stack;
		stack = aux;
	}
}

void inputPop(){
	if(input != NULL){
		struct t_token *aux;
		aux = input;
		input = input->next_token;		
		
		if(aux->malloced_lexeme)
			free(aux->lexeme);

		free(aux);
	}
}

void stackPop(){
	if(stack != NULL){
		struct t_stack *aux;
		aux = stack;
		stack = stack->next;

		if(aux->data != NULL) free(aux->data);
		aux->next = NULL;
		free(aux);
	}
}

void printTableRow(int non_terminal){
	int i;

	for(i = 0; i < CT; i++){
		printf("TERMINAL %d : ",i);

		struct production *iterator;

		switch(table[non_terminal][i].op){
			case GENERATE:
				if(table[non_terminal][i].production != NULL ){
					for(iterator = table[non_terminal][i].production;iterator != NULL;iterator = iterator->next){
						char *type;

						if(iterator->type == NO_TERMINAL) type = "NT";
						else if(iterator->type == TERMINAL) type = "T";

						printf("%s%d ", type, iterator->index);
					}
				}
				break;
			case BLANK:						
				printf("BLANK");
				break;
			case SCAN:						
				printf("SCAN");
				break;
			case POP:						
				printf("POP");
				break;
		}

		printf("\n");
	}
}

void clone_production(size_t M,struct production prod[M],int non_terminal,int terminal,...){
	va_list args;
	va_start(args,terminal);

	for( ; ; ){ 
		if(terminal == -1) break; 
		struct production *pr = NULL;
		struct production *ptr = NULL;
		unsigned int i = 0;

		for(i = 0;i < M;i++){
			if(pr == NULL){
				pr = copy_production(prod[i],ptr);
				ptr = pr;
			}
			else{
				ptr->next = copy_production(prod[i],ptr);
				ptr = ptr->next;
			}
		}

		table[non_terminal][terminal].production = pr;
		table[non_terminal][terminal].op = GENERATE;
    		terminal = va_arg(args,int);
	}

	va_end(args);
}

struct production *copy_production(struct production prod,struct production *last){
	struct production *pr;
	pr = (struct production *) malloc(sizeof(struct production));
	pr->type = prod.type;
	pr->index = prod.index;
	pr->last = last;
	pr->next = NULL;
	return pr;
}	

void setPopProduction(int NT, int terminal, ...){
	va_list args;
	va_start(args,terminal);

	for( ; ; ){ 
		if(terminal == -1) break; 

		table[NT][terminal].op = POP;
		table[NT][terminal].production = NULL;
    		terminal = va_arg(args,int);
	}

	va_end(args);
}

void setBlankProduction(int NT, int terminal, ...){
	va_list args;
	va_start(args,terminal);

	for( ; ; ){ 
		if(terminal == -1) break; 

		table[NT][terminal].op = BLANK;
		table[NT][terminal].production = NULL;
    		terminal = va_arg(args,int);
	}

	va_end(args);
}

void destroyParsingTable(){	/* En realidad solo destruye las producciones */
	int i, j;

	for(i = 0; i < CNT;i++){
		for(j = 0;j < CT;j++){
			if(table[i][j].production != NULL){				
				struct production *iterator;			
				struct production *ptr = NULL;
			
				for(iterator = table[i][j].production;iterator != NULL;iterator = iterator->next){
					if(ptr != NULL){
						free(ptr);
					}

					ptr = iterator;
				}

				if(ptr != NULL){
					free(ptr);
				}
			}
		}
	}
}

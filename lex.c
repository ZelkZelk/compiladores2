#include "lex.h"

char *tokentype(t_tokentype type){
	switch(type){
		case TERMINADOR_PUNTOCOMA: return "TERMINADOR_PUNTOCOMA";
		case TERMINADOR_ENTER: return "TERMINADOR_ENTER";
		case L_PARENTESIS: return "L_PARENTESIS";
		case R_PARENTESIS: return "R_PARENTESIS";
		case OP_CONDICION: return "OP_CONDICION";
		case OP_SUMA: return "OP_SUMA";
		case OP_MUL: return "OP_MUL";
		case OP_RELACIONAL: return "OP_RELACIONAL";
		case OP_ASIGNACION: return "OP_ASIGNACION";
		case L_CORCHETE: return "L_CORCHETE";
		case R_CORCHETE: return "R_CORCHETE";
		case COMA: return "COMA";
		case DELIMITADOR_CODIGO: return "DELIMITADOR_CODIGO";
		case PR_IF: return "PR_IF";
		case PR_FALSE: return "PR_FALSE";
		case PR_TRUE: return "PR_TRUE";
		case PR_ALERT: return "PR_ALERT";
		case LITERAL_NUM: return "LITERAL_NUM";
		case LITERAL_CADENA: return "LITERAL_CADENA";
		case ID: return "ID";
		case COMMENT: return "COMMENT";
		case _EOF: return "EOF";
		case __ERROR: return "ERROR";
	}

	return "";
}

/* Esta vez el ejercicio no pide imprimir el flujo de tokens.
 * Asi que voy a devolver la cabeza de la cola de tokens.
 */

struct t_token* lexer(FILE* input){
	rewind(input);
	off_t offset = 0;
	off_t *offset_ptr = &offset;
	unsigned short int break_loop = 0;
	struct t_token *head = NULL;				/* Definimos un puntero cabeza de la cola de tokens */
	struct t_token *tail = NULL;				/* Tambien necesito un puntero que apunte al final de la cola de tokens */

	do{
		char *line = get_line(input,&offset_ptr); 	/* Leemos el FILE linea a linea para realizar el analisis */	

		if(line != NULL){
			if(strchr(line,EOF)){			/* Si la cadena tiene un EOF, rompemos el ciclo */
				break_loop = 1;
			}

			tokenize(&head,&tail,line);			
			free(line);
		}
		else{
			printf("Error al leer archivo\n");
			break_loop = 1;
		}
	}
	while(break_loop == 0);

	/* 
	 * Voy a chequear por errorer léxicos, si se encuentra se devuelve NULL y no se procede al analisis sintactico.
	 * Lamentablemente por razones de tiempo no voy a poner mucho detalle en el mensaje de error.	
	 */

	if(head != NULL){
		struct t_token *iterator;
		int line = 1;
		unsigned short int error = 0;

		for(iterator = head;iterator != NULL;iterator = iterator->next_token){
			switch(iterator->tokentype){
				case TERMINADOR_ENTER:
					line++;
					break;
				case __ERROR:
					printf("Lexema no válido en la Línea %d\n",line);
					error = 1;
					break;
			}
		}

		// Vamos a limpiar la memoria
		if(error){
			token_clean(head);
			return NULL;
		}
	}

	return head;
}

void token_clean(struct t_token *head){
	if(head != NULL){
		struct t_token *pivot;
		pivot = head->next_token;

		if(head->malloced_lexeme)
			free(head->lexeme);

		if(pivot != NULL){
			token_clean(pivot);
		}
	}
}

/*
	Durante el analisis voy a ir guardando los tokens en una estructura tipo cola, la implementacion la
	realizo con dos punteros, uno de ellos apuntara siempre al primer nodo, mientras que el segundo ira
	apuntando al final de la cola, de modo que al agregar un nuevo item, el puntero debe refrescarse cada vez.

	Los tokens simples de 1 solo caracter y que no presenten colisiones en dicho caracter con otros tokens
	se agregan directamente a la cola de tokens.

	Mientras que los tokens complejos requeriran el uso de un automata para el proceso de decision.
	Los automatas que tengan alguna colision van a ser evaluados en cortocircuito.

	Tambien para no hacer el proceso muy lento, voy a usar el primer caracter como centinela para decidir que automata 
	utilizar. Los automatas consumiran los caracteres desde el puntero, en caso de encontrar un lexema correcto, en el 
	loop principal aumento el puntero segun los caracteres consumidos por cada automata.
*/

void tokenize(struct t_token **head,struct t_token **tail,char *line){
	char *ch = line;
	unsigned short int error;
						
	while(*ch != '\0'){
		error = 0;

		switch(*ch){
			case EOF:
				add_token(head,tail,"EOF",_EOF);	/* El EOF rompe la funcion */
				return;
			case ';':
				add_token(head,tail,";",TERMINADOR_PUNTOCOMA);
				break;
			case '\n':
				add_token(head,tail,"\n",TERMINADOR_ENTER);
				break;
			case '(':
				add_token(head,tail,"(",L_PARENTESIS);
				break;
			case ')':
				add_token(head,tail,")",R_PARENTESIS);
				break;
			case '?':
				add_token(head,tail,"?",OP_CONDICION);
				break;
			case '+':
				add_token(head,tail,"+",OP_SUMA);
				break;
			case '/':
				add_token(head,tail,"/",OP_MUL);
				break;
			case '*':
				add_token(head,tail,"*",OP_MUL);
				break;	
			case ']':
				add_token(head,tail,"]",R_CORCHETE);
				break;
			case '[':
				add_token(head,tail,"[",L_CORCHETE);
				break;
			case ',':
				add_token(head,tail,",",COMA);
				break;
			/* Hasta aqui llegan los tokens simples */
			case '-':	/* Posibles lexemas: - -> */
				error = !automata_minus(head,tail,ch,1);
				break;
			case '<':	/* Posibles lexemas: < <= */
				error = !automata_lesser(head,tail,ch,1);
				break;
			case '>':	/* Posibles lexemas: > >= */
				error = !automata_greater(head,tail,ch,1);
				break;
			case '!':	/* Posibles lexemas: != */
				error = !automata_exclamation(head,tail,ch,1);
				break;
			case '=':	/* Posibles lexemas: = == */
				error = !automata_equal(head,tail,ch,1);
				break;
			case 'i':	/* Posibles tokens: PR_IF, ID */
				error = !(automata_if(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 'I':	/* Posibles tokens: PR_IF, ID */	
				error = !(automata_if_mayus(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 't':	/* Posibles tokens: PR_TRUE, ID */
				error = !(automata_true(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 'T':	/* Posibles tokens: PR_TRUE, ID */	
				error = !(automata_true_mayus(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 'f':	/* Posibles tokens: PR_FALSE, ID */
				error = !(automata_false(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 'F':	/* Posibles tokens: PR_FALSE, ID */	
				error = !(automata_false_mayus(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 'a':	/* Posibles tokens: PR_ALERT, ID */
				error = !(automata_alert(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case 'A':	/* Posibles tokens: PR_ALERT, ID */	
				error = !(automata_alert_mayus(head,tail,ch,1) || automata_id(head,tail,ch,1));
				break;
			case '#':	/* Posibles tokens: COMMENT */
				error = !(automata_comment(head,tail,ch,1));
				break;
			case '"':	/* Posibles tokens: LITERAL_CADENA */
				error = !(automata_literal_string(head,tail,ch,1));
				break;
			case ' ':	/* Ignoramos el caracter */
				ch += 1;
				continue;
			default:	/* Posibles tokens: ID, LITERAL_NUMBER */
				error = !(automata_id(head,tail,ch,1) || automata_literal_number(head,tail,ch,1));
		}

		if(error == 1 || tail == NULL){			// Si se detecta un error, se agrega el token especial __ERROR
			add_token(head,tail,"ERROR",__ERROR);	// Se continua desde la proxima linea.
			return;
		}
		else{
			ch += strlen((*tail)->lexeme);		// Aumentamos el puntero segun la cantidad de caracteres consumidos
		}
	}
}

/* Devuelve la cadena entre i,j */

char *extractor(char *i,char *j){
	if(j > i){
		unsigned int x = 0;
		char *iterator;
		char *string = (char *)malloc(j - i + 1);
		memset(string,'\0',j - i + 1);

		for(iterator = i;iterator < j;iterator++){
			string[x] = *iterator;
			x++;			
		}

		return string;
	}
	else{
		return NULL;
	}
}

/*
	Todos estos automatas requieren los dos punteros de la cola, el puntero a la cadena alimentadora y el 
	entero que define su estado.

	Devuelve 0 en caso de que no se llegue a un estado de aceptacion.

	Caso contrario devuelve el estado de aceptacion.

	Por comodidad, el estado inicial de todos es 1.
*/

unsigned short int automata_literal_string(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	char *start = input;

	while(*input != '\0'){
		switch(status){
			case 1:
				if(*input == '"'){
					status = 2;
					++input;
				}
				else{
					return 0;
				}
				
				break;
			case 2:
				if(*input == '"'){
					status = 3;
					++input;
				}
				else{
					++input;
				}
				
				break;
			case 3:
				add_token(head,tail,extractor(start,input),LITERAL_CADENA);		
				(*tail)->malloced_lexeme = 1;
				return status;	
		}
	}

	/* A diferencia de otras regex, esta requiere que la comilla se cierre,
	 * en consecuencia, si llegamos a esta parte de la ejecucion asumimos error.
	 */

	return 0;
}

unsigned short int automata_literal_number(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	const char numbers[11] = "0123456789\0";
	char *start = input;

	while(*input != '\0'){
		switch(status){
			case 1:	/* Aseguramos al menos un numero al principio */
				if(strchr(numbers,*input)){
					status = 2;
					++input;
				}
				else{
					return 0;
				}

				break;
			case 2:
				if(strchr(numbers,*input)){	// Seguimos comiendo numeros
					status = 2;
					++input;
				}
				else if(*input == '.'){		// Decimal
					status = 3;
					++input;
				}
				else if(*input == 'e' || *input == 'E'){	// Tiene parte cientifica
					status = 5;
					++input;
				}
				else{
					add_token(head,tail,extractor(start,input),LITERAL_NUM);
					(*tail)->malloced_lexeme = 1;
					return status;
				}

				break;
			case 3:	
				if(strchr(numbers,*input)){ 	// Debemos asegurar al menos una posicion decimal
					status = 4;
					++input;			
				}
				else{
					return 0;
				}

				break;
			case 4:
				if(strchr(numbers,*input)){	// Seguimos comiendo numeros
					status = 4;
					++input;
				}
				else if(*input == 'e' || *input == 'E'){	// Tiene parte cientifica
					status = 5;
					++input;
				}
				else{
					add_token(head,tail,extractor(start,input),LITERAL_NUM);
					(*tail)->malloced_lexeme = 1;
					return status;
				}
				break;
			case 5:		// En este estado puede haber signo o numero
				if(*input == '+' || *input == '-' || strchr(numbers,*input)){
					status = 6;
					++input;	
				}
				else{
					return 0;
				}

				break;
			case 6:
				if(strchr(numbers,*input)){	// Seguimos comiendo numeros
					status = 6;
					++input;
				}
				else{
					add_token(head,tail,extractor(start,input),LITERAL_NUM);
					(*tail)->malloced_lexeme = 1;
					return status;
				}
				break;
		}
	}

	// Si se acaba la cadena en un estado de aceptacion se encola el token
	if(status == 6 || status == 4 || status == 2){
		add_token(head,tail,extractor(start,input),LITERAL_NUM);
		(*tail)->malloced_lexeme = 1;
		return status;
	}

	return 0;
}

unsigned short int automata_comment(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	char *start = input;

	while(*input != '\0'){
		switch(status){
			case 1:
				if(*input == '#'){
					status = 2;
					++input;
				}
				else{
					return 0;
				}

				break;
			case 2:
				if(*input != '\n'){
					status = 2;
					++input;
				}
				else{
					add_token(head,tail,extractor(start,input),COMMENT);
					(*tail)->malloced_lexeme = 1;
					return status;
				}

				break;
		}
	}

	if(status == 2){
		add_token(head,tail,extractor(start,input),COMMENT);
		(*tail)->malloced_lexeme = 1;
		return status;
	}

	return 0;
}

unsigned short int automata_id(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	const char alpha[27] = "abcdefghijklmnopqrstuvwxyz\0";	
	const char alpha_mayus[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0";	
	const char numbers[11] = "0123456789\0";
	const char underscore[2] = "_\0";
	char *start = input;

	while(*input != '\0'){
		switch(status){
			case 1:
				if(strchr(alpha,*input) || strchr(alpha_mayus,*input) || strchr(underscore,*input)){
					status = 2;
					++input;
				}
				else{
					return 0;
				}

				break;
			case 2:
				if(strchr(alpha,*input) || strchr(alpha_mayus,*input) || strchr(underscore,*input) || strchr(numbers,*input)){
					status = 2;	
					++input;
				}
				else{
					status = 3;
				}

				break;
			case 3:
				add_token(head,tail,extractor(start,input),ID);
				(*tail)->malloced_lexeme = 1;
				return status;
		}
	}

	if(status == 2){	// Puede ser que se llegue al final de la cadena
		add_token(head,tail,extractor(start,input),ID);
		(*tail)->malloced_lexeme = 1;
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_false(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_false(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'a'){
		return automata_false(head,tail,++input,3);
	}
	else if(status == 3 && *input == 'l'){
		return automata_false(head,tail,++input,4);
	}
	else if(status == 4 && *input == 's'){
		return automata_false(head,tail,++input,5);
	}
	else if(status == 5 && *input == 'e'){
		add_token(head,tail,"false",PR_FALSE);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_false_mayus(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_false_mayus(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'A'){
		return automata_false_mayus(head,tail,++input,3);
	}
	else if(status == 3 && *input == 'L'){
		return automata_false_mayus(head,tail,++input,4);
	}
	else if(status == 4 && *input == 'S'){
		return automata_false_mayus(head,tail,++input,5);
	}
	else if(status == 5 && *input == 'E'){
		add_token(head,tail,"FALSE",PR_FALSE);
		return status;
	}
	else{
		return 0;
	}
}
unsigned short int automata_alert(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_alert(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'l'){
		return automata_alert(head,tail,++input,3);
	}
	else if(status == 3 && *input == 'e'){
		return automata_alert(head,tail,++input,4);
	}
	else if(status == 4 && *input == 'r'){
		return automata_alert(head,tail,++input,5);
	}
	else if(status == 5 && *input == 't'){
		add_token(head,tail,"alert",PR_ALERT);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_alert_mayus(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_alert_mayus(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'L'){
		return automata_alert_mayus(head,tail,++input,3);
	}
	else if(status == 3 && *input == 'E'){
		return automata_alert_mayus(head,tail,++input,4);
	}
	else if(status == 4 && *input == 'R'){
		return automata_alert_mayus(head,tail,++input,5);
	}
	else if(status == 5 && *input == 'T'){
		add_token(head,tail,"ALERT",PR_ALERT);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_true(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_true(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'r'){
		return automata_true(head,tail,++input,3);
	}
	else if(status == 3 && *input == 'u'){
		return automata_true(head,tail,++input,4);
	}
	else if(status == 4 && *input == 'e'){
		add_token(head,tail,"true",PR_TRUE);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_true_mayus(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_true_mayus(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'R'){
		return automata_true_mayus(head,tail,++input,3);
	}
	else if(status == 3 && *input == 'U'){
		return automata_true_mayus(head,tail,++input,4);
	}
	else if(status == 4 && *input == 'E'){
		add_token(head,tail,"TRUE",PR_TRUE);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_if(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_if(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'f'){
		add_token(head,tail,"if",PR_IF);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_if_mayus(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){
		return automata_true_mayus(head,tail,++input,2);
	}
	else if(status == 2 && *input == 'F'){
		add_token(head,tail,"IF",PR_IF);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_equal(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){						// Siempre va a tener el signo igual
		return automata_equal(head,tail,++input,2);
	}
	else if(status == 2){
		if(*input == '='){
			add_token(head,tail,"==",OP_RELACIONAL);
			return status;
		}
		else{
			return automata_equal(head,tail,input,3);
		}	
	}
	else if(status == 3){
		add_token(head,tail,"=",OP_ASIGNACION);
		return status;
	}
	else{
		return 0;
	}
}
unsigned short int automata_exclamation(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){						// Siempre va a tener el signo de exclamacion
		return automata_exclamation(head,tail,++input,2);
	}
	else if(status == 2){
		if(*input == '='){
			add_token(head,tail,"!=",OP_RELACIONAL);
			return status;
		}
		else{
			return automata_exclamation(head,tail,input,0);
		}	
	}
	else{
		return 0;
	}
}

unsigned short int automata_greater(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){						// Siempre en el estado 1 va a contener mayor que
		return automata_greater(head,tail,++input,2);
	}
	else if(status == 2){
		if(*input == '='){
			add_token(head,tail,">=",OP_RELACIONAL);
			return status;
		}
		else{
			return automata_greater(head,tail,input,3);
		}
	}
	else if(status == 3){
		add_token(head,tail,">",OP_RELACIONAL);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_lesser(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){						// Siempre en el estado 1 va a contener menor que
		return automata_lesser(head,tail,++input,2);
	}
	else if(status == 2){
		if(*input == '='){
			add_token(head,tail,"<=",OP_RELACIONAL);
			return status;
		}
		else{
			return automata_lesser(head,tail,input,3);
		}
	}
	else if(status == 3){
		add_token(head,tail,"<",OP_RELACIONAL);
		return status;
	}
	else{
		return 0;
	}
}

unsigned short int automata_minus(struct t_token **head,struct t_token **tail,char *input,unsigned short int status){
	if(status == 1){						// Siempre en el estado 1 input va a contener el signo menos
		return automata_minus(head,tail,++input,2);
	}
	else if(status == 2){
		if(*input == '>'){
			add_token(head,tail,"->",DELIMITADOR_CODIGO);
			return status;
		}
		else{
			return automata_minus(head,tail,input,3);
		}
	}
	else if(status == 3){
		add_token(head,tail,"-",OP_SUMA);
		return status;
	}
	else{
		return 0;
	}
}

/* Esta funcion resuelve el indice en la tabla de parsing del token */

int getTokenTableIndex(t_tokentype tokentype){
	switch(tokentype){		
		case TERMINADOR_PUNTOCOMA: TERMINADOR_PUNTOCOMA_INDEX;
		case TERMINADOR_ENTER: TERMINADOR_ENTER_INDEX;
		case L_PARENTESIS: L_PARENTESIS_INDEX;
		case R_PARENTESIS: R_PARENTESIS_INDEX;
		case OP_CONDICION: OP_CONDICION_INDEX;
		case OP_SUMA: OP_SUMA_INDEX;
		case OP_MUL: OP_MUL_INDEX;
		case OP_RELACIONAL: OP_RELACIONAL_INDEX;
		case OP_ASIGNACION: OP_ASIGNACION_INDEX;
		case L_CORCHETE: L_CORCHETE_INDEX;
		case R_CORCHETE: R_CORCHETE_INDEX;
		case COMA: COMA_INDEX;
		case DELIMITADOR_CODIGO: DELIMITADOR_CODIGO;
		case PR_IF: PR_IF_INDEX;
		case PR_TRUE: PR_TRUE_INDEX;
		case PR_FALSE: PR_FALSE_INDEX;
		case PR_ALERT: PR_ALERT_INDEX;
		case LITERAL_NUM: LITERAL_NUM_INDEX;
		case LITERAL_CADENA: LITERAL_CADENA_INDEX;
		case ID: ID_INDEX;
		case COMMENT: COMMENT_INDEX;
		case _EOF: _EOF_INDEX;
		case __ERROR: __ERROR_INDEX;
	}
}

/* Esta funcion agrega un nuevo token a la cola, acomodando los punteros necesarios para la implementacion */

void add_token(struct t_token **head,struct t_token **tail,char *lexeme, t_tokentype t_tokentype){
	struct t_token *new_token = create_token(lexeme,t_tokentype);

	if(*tail != NULL){
		(*tail)->next_token = new_token;
	}

	*tail = new_token;
	
	if(*head == NULL){
		*head = new_token;
	}	
}


/* 
	En esta funcion pedimos la memoria necesaria para alojar un token, cargamos sus datos y devolvemos un puntero a dicha 
	memoria.
*/

struct t_token *create_token(char *lexeme,t_tokentype t_tokentype){
	struct t_token *token = (struct t_token *)malloc(sizeof(struct t_token));
	token->lexeme = lexeme;
	token->tokentype = t_tokentype;
	token->next_token = NULL;
	token->malloced_lexeme = 0;
	token->tableIndex = getTokenTableIndex(t_tokentype);
	return token;
}

/* Esta funcion devuelve las lineas de un fichero */

char *get_line(FILE *file,off_t **offset){
	size_t buffer_size = 30;
	char *buffer = (char *)malloc(buffer_size);
	char *ptr = buffer;
	unsigned short int break_loop = 0;
	memset(buffer,'\0',buffer_size);	/* Limpiamos el buffer */

	/* El programa que use esta funcion debe chequear que el buffer recibido contenga un EOF para cortar su loop */

	do{
		/* Movemos el FILE al offset */
		lseek(fileno(file),**offset,SEEK_SET);

		/* Lectura y carga en el buffer */
		int bytes = read(fileno(file),ptr,buffer_size - 1); /* Notese que dejamos un byte para el \0 */	

		if(bytes == 0){			/* EOF */
			*buffer = EOF;
			break_loop = 1;
		}
		else if(bytes == -1){		/* Ocurrio algun error */
			free(buffer);
			return NULL;
		}
		else{
			/* Limpiamos el buffer, esto asegura que la cadena termina con \0 */
			memset(ptr + bytes,'\0',buffer_size - bytes);

			/* Buscamos el \n */
			char *new_line = strchr(ptr,'\n');

			if(new_line == NULL){							/* No se encontro el \n */
				char *block = (char *)realloc(buffer,bytes + buffer_size);	/* Pedimos mas memoria */
				buffer = block;
				ptr = buffer + bytes;						/* Apuntamos ptr a la nueva memoria */
				**offset += bytes;						/* Incrementamos el offset */
				memset(ptr,'\0',buffer_size);					/* Limpiamos la memoria nueva */
			}
			else{
				/* Para nosotros la cadena termina en el \n */
				int len = new_line - ptr + 1;

				/* Limpiamos el buffer */
				if(len < buffer_size){
					memset(new_line + 1,'\0',buffer_size - len);	
				}

				/* Incrementamos el offset */
				**offset += len;

				/* 
				 * Voy a realizar una lectura adicional del archivo para saber si ya estoy en EOF
				 * en tal caso, el ultimo end of line se va a transformar en un end of file.
			         * Asi cumplimos el estandar ANSI C, todo archivo debe terminar con un end of line.
				 * http://stackoverflow.com/questions/729692/why-should-files-end-with-a-newline	
				 */

				char last;
				lseek(fileno(file),**offset,SEEK_SET);
				int last_byte = read(fileno(file),&last,1); 

				if(last_byte == 0){	/* EOF */
					*new_line = EOF;	
				}

				/* Flagueamos la ruptura */
				break_loop = 1;	
			}
		}
	}while(break_loop == 0);

	/* Descomentar para ver el valor de los caracteres */
	//char *i; for(i = buffer;*i != '\0';i++) printf("|%d|",*i);

	return buffer;
}

FILE* check_fopen(char *filepath, const char *flag){
	FILE* file = fopen(filepath,flag);
	
	if(file == NULL){
		switch(errno){			/* Imprimimos los errores mas comunes */
			case EACCES:
				printf("Acceso denegado a %s\n",filepath);
				break;
			case ENOENT:
				printf("Archivo no existe %s\n",filepath);
				break;
			default:
				printf("Error #%d archivo %s\n",errno,filepath);
		}
	}	

	return file;
}


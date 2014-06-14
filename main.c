#include "gramatica.h"
#include "lex.h"

/* MAIN */

int main(int argc,char *argv[]){		
	if(argc == 2){
		FILE* input = check_fopen(argv[1],"r");

		if(input != NULL){
			struct t_token *tokens = lexer(input);
			fclose(input);	

			if(tokens != NULL){
				/* Procedemos al Analisis Sintactico */
				syntax(tokens);
				//token_clean(tokens);
			}			
		}		
	}
	else{					/* Se especifico una cantidad no soportada de parametros, imprimimos la ayuda */
		help(argv[0]);
	}
	
	
	return EXIT_SUCCESS;
}

void help(char *bin){
	printf("\n\n");
	printf("\tDebe especificar el archivo de entrada\n");
	printf("\tNo especifique otros parametros\n\n");
	printf("\t\t%s input.txt\n\n\n",bin);
}

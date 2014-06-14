#include "parsing_table.h"

void getParsingTable(){
	/* Por defecto todos son scans, excepto los eof */
	int i,j;

	for(i = 0;i < CNT;i++){
		for(j = 0;j < 21;j++){
			table[i][j].op = SCAN;
		}

		table[i][_EOF_INDEX].op = POP;
	}

	expresion();	
	expresion2();	
	expresion3();
	expresion4();
	valor();
	valor1();
	otro_valor();
	parametros();
	parametros1();
	expresion5();
	term3();
	linea1();
	term1();
	body1();
	body2();
	body();
	expresion6();
	expresion8();
	term();
	valores();
	valores1();
	term2();
	condicional();
	condicional1();
	comentario();
	expresion9();
	expresion10();
	coffeescript();
	terminadores();
	terminadores1();
	terminador();
	linea();
	operador();
	f_alert();
}

void expresion3(){
	/* operador exp */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = OPERADOR;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = EXPRESION;

	clone_production(2,pr1,EXPRESION3,OP_SUMA_INDEX,OP_MUL_INDEX,OP_RELACIONAL_INDEX,-1);

	/* Vacio */
	setBlankProduction(EXPRESION3,PR_ALERT_INDEX,LITERAL_NUM_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,_EOF_INDEX,L_PARENTESIS_INDEX,L_CORCHETE_INDEX,-1);
}

void expresion4(){
	/* parametros */
	struct production pr1[1];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = PARAMETROS;
	clone_production(1,pr1,EXPRESION4,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);

	/* exp3 */
	struct production pr2[1];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = EXPRESION3;
	clone_production(1,pr2,EXPRESION4,OP_SUMA_INDEX,OP_MUL_INDEX,OP_RELACIONAL_INDEX,-1);

	/* Pop */	
	setPopProduction(EXPRESION4,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void valor(){
	/* id valor1 */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = ID_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = VALOR1;
	clone_production(2,pr1,VALOR,ID_INDEX,-1);

	/* otro_valor valor1 */
	struct production pr2[2];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = OTRO_VALOR;
	pr2[1].type = NO_TERMINAL;
	pr2[1].index = VALOR1;
	clone_production(2,pr2,VALOR,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);

	/* Pop */	
	setPopProduction(VALOR,L_PARENTESIS_INDEX,PR_ALERT_INDEX,COMA_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,R_CORCHETE_INDEX,OP_CONDICION_INDEX,-1);
}

void valor1(){
	/* [ valor ] valor1 */
	struct production pr1[4];
	pr1[0].type = TERMINAL;
	pr1[0].index = L_CORCHETE_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = VALOR;
	pr1[2].type = TERMINAL;
	pr1[2].index = R_CORCHETE_INDEX;
	pr1[3].type = NO_TERMINAL;
	pr1[3].index = VALOR1;
	clone_production(4,pr1,VALOR1,L_CORCHETE_INDEX,-1);

	/* Vacio */
	setBlankProduction(VALOR1,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,OP_ASIGNACION_INDEX,COMA_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,R_CORCHETE_INDEX,OP_CONDICION_INDEX,-1);
}

void otro_valor(){
	/* num */
	struct production pr1[1];
	pr1[0].type = TERMINAL;
	pr1[0].index = LITERAL_NUM_INDEX;
	clone_production(1,pr1,OTRO_VALOR,LITERAL_NUM_INDEX,-1);

	/* literal */
	struct production pr2[1];
	pr2[0].type = TERMINAL;
	pr2[0].index = LITERAL_CADENA_INDEX;
	clone_production(1,pr2,OTRO_VALOR,LITERAL_CADENA_INDEX,-1);

	/* true */
	struct production pr3[1];
	pr3[0].type = TERMINAL;
	pr3[0].index = PR_FALSE_INDEX;
	clone_production(1,pr3,OTRO_VALOR,PR_TRUE_INDEX,-1);

	/* false */
	struct production pr4[1];
	pr4[0].type = TERMINAL;
	pr4[0].index = PR_FALSE_INDEX;
	clone_production(1,pr4,OTRO_VALOR,PR_FALSE_INDEX,-1);

	/* [ valores ] */
	struct production pr5[3];
	pr5[0].type = TERMINAL;
	pr5[0].index = L_CORCHETE_INDEX;
	pr5[1].type = NO_TERMINAL;
	pr5[1].index = VALORES;
	pr5[2].type = TERMINAL;
	pr5[2].index = R_CORCHETE_INDEX;
	clone_production(3,pr5,OTRO_VALOR,L_CORCHETE_INDEX,-1);

	/* Pop */	
	setPopProduction(OTRO_VALOR,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,OP_ASIGNACION_INDEX,COMA_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,R_CORCHETE_INDEX,OP_CONDICION_INDEX,-1);
}

void parametros(){
	/* valor parametros1 */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = VALOR;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = PARAMETROS1;
	clone_production(2,pr1,PARAMETROS,ID_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);
	
	/* Vacio */
	setBlankProduction(PARAMETROS,L_PARENTESIS_INDEX,PR_ALERT_INDEX,COMMENT_INDEX,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);

	/* Pop */	
	setPopProduction(PARAMETROS,L_PARENTESIS_INDEX,PR_ALERT_INDEX,OP_ASIGNACION_INDEX,COMMENT_INDEX,PR_IF_INDEX,L_CORCHETE_INDEX,R_CORCHETE_INDEX,-1);

}

void parametros1(){
	/* , parametros */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = COMA_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = PARAMETROS;
	clone_production(2,pr1,PARAMETROS1,COMA_INDEX,-1);
	
	/* Vacio */
	setBlankProduction(PARAMETROS1,ID_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,COMMENT_INDEX,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void expresion5(){
	/* id exp6  */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = ID_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = EXPRESION6;
	clone_production(2,pr1,EXPRESION5,ID_INDEX,-1);

	/* term3  */
	struct production pr2[1];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = TERM3;
	clone_production(1,pr2,EXPRESION5,L_PARENTESIS_INDEX,PR_ALERT_INDEX,COMMENT_INDEX,-1);
	
	/* otro_valor exp6 */
	struct production pr3[2];
	pr3[0].type = NO_TERMINAL;
	pr3[0].index = OTRO_VALOR;
	pr3[1].type = NO_TERMINAL;
	pr3[1].index = EXPRESION6;
	clone_production(2,pr3,EXPRESION5,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);
	
	/* Vacio */
	setBlankProduction(EXPRESION5,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void term3(){
	/* linea1 body1 */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = LINEA1;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = BODY1;
	clone_production(2,pr1,TERM3,L_PARENTESIS_INDEX,PR_ALERT_INDEX,COMMENT_INDEX,-1);

	/* Pop */	
	setPopProduction(TERM3,ID_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,PR_IF_INDEX,OP_CONDICION_INDEX,-1);
}

void linea1(){
	/* ( exp5 exp9 term1 */
	struct production pr1[4];
	pr1[0].type = TERMINAL;
	pr1[0].index = L_PARENTESIS_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = EXPRESION5;
	pr1[2].type = NO_TERMINAL;
	pr1[2].index = EXPRESION9;
	pr1[3].type = NO_TERMINAL;
	pr1[3].index = TERM1;
	clone_production(4,pr1,LINEA1,L_PARENTESIS_INDEX,-1);

	/* f_alert exp9 term1 */
	struct production pr2[3];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = F_ALERT;
	pr2[1].type = NO_TERMINAL;
	pr2[1].index = EXPRESION9;
	pr2[2].type = NO_TERMINAL;
	pr2[2].index = TERM1;
	clone_production(3,pr2,LINEA1,PR_ALERT_INDEX,-1);

	/* comentario */
	struct production pr3[1];
	pr3[0].type = NO_TERMINAL;
	pr3[0].index = COMENTARIO;
	clone_production(1,pr3,LINEA1,COMMENT_INDEX,-1);

	/* Vacio */
	setBlankProduction(LINEA1,ID_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void term1(){
	/* condicional comentario */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = CONDICIONAL;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = COMENTARIO;
	clone_production(2,pr1,TERM1,PR_IF_INDEX,-1);

	/* Vacio */
	setBlankProduction(TERM1,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);

}

void body1(){
	/* terminadores body2 */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = TERMINADORES;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = BODY2;
	clone_production(2,pr1,BODY1,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,-1);

	/* Vacio */
	setBlankProduction(BODY1,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);
}

void body2(){
	/* body */
	struct production pr1[1];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = BODY;
	clone_production(1,pr1,BODY2,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,-1);
	
	/* Vacio */
	setBlankProduction(BODY2,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);
}

void body(){
	/* linea body1 */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = LINEA;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = BODY1;
	clone_production(2,pr1,BODY,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,-1);
	
	/* Vacio */
	setBlankProduction(BODY,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);
}

void expresion6(){
	/* ( parametros ) term1 body1 */
	struct production pr1[5];
	pr1[0].type = TERMINAL;
	pr1[0].index = L_PARENTESIS_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = PARAMETROS;
	pr1[2].type = TERMINAL;
	pr1[2].index = R_PARENTESIS_INDEX;
	pr1[3].type = NO_TERMINAL;
	pr1[3].index = TERM1;
	pr1[4].type = NO_TERMINAL;
	pr1[4].index = BODY1;
	clone_production(5,pr1,EXPRESION6,L_PARENTESIS_INDEX,-1);

	/* valor1 exp8 */
	struct production pr2[2];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = VALOR1;
	pr2[1].type = NO_TERMINAL;
	pr2[1].index = EXPRESION8;
	clone_production(2,pr2,EXPRESION6,L_CORCHETE_INDEX,-1);

	/* = exp term1 body1 */
	struct production pr3[4];
	pr3[0].type = TERMINAL;
	pr3[0].index = OP_ASIGNACION_INDEX;
	pr3[1].type = NO_TERMINAL;
	pr3[1].index = EXPRESION;
	pr3[2].type = NO_TERMINAL;
	pr3[2].index = TERM1;
	pr3[3].type = NO_TERMINAL;
	pr3[3].index = BODY1;
	clone_production(4,pr3,EXPRESION6,OP_ASIGNACION_INDEX,-1);

	/* Vacio */
	setBlankProduction(EXPRESION6,ID_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void expresion8(){
	/* exp4 */
	struct production pr1[1];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = EXPRESION4;
	clone_production(1,pr1,EXPRESION8,ID_INDEX,L_PARENTESIS_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,OP_ASIGNACION_INDEX,-1);

	/* valores1 */
	struct production pr2[1];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = VALORES1;
	clone_production(1,pr2,EXPRESION8,COMA_INDEX,-1);

	/* Vacio */
	setBlankProduction(EXPRESION8,PR_ALERT_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void term(){
	/* -> body  */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = DELIMITADOR_CODIGO_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = BODY;
	clone_production(2,pr1,TERM,DELIMITADOR_CODIGO_INDEX,-1);

	/* Pop */
	setPopProduction(TERM,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,PR_IF_INDEX,OP_CONDICION_INDEX,-1);
}

void valores(){
	/* valor valores1 */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = VALOR;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = VALORES1;
	clone_production(2,pr1,VALORES,ID_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);

	/* Pop */
	setPopProduction(VALORES,R_CORCHETE_INDEX,-1);
}

void valores1(){
	/* , valores */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = COMA_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = VALORES;
	clone_production(2,pr1,VALORES1,COMA_INDEX,-1);

	/* Vacio */
	setBlankProduction(VALORES1,R_CORCHETE_INDEX,-1);
}

void term2(){
	/* term1 body1 ) */
	struct production pr1[3];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = TERM1;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = BODY1;
	pr1[2].type = TERMINAL;
	pr1[2].index = R_PARENTESIS_INDEX;
	clone_production(3,pr1,TERM2,PR_IF_INDEX,-1);
	
	/* Vacio */	
	setBlankProduction(TERM2,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void condicional(){
	/* if expresion condicional1 */
	struct production pr1[3];
	pr1[0].type = TERMINAL;
	pr1[0].index = PR_IF_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = EXPRESION;
	pr1[2].type = NO_TERMINAL;
	pr1[2].index = CONDICIONAL1;
	clone_production(3,pr1,CONDICIONAL,PR_IF_INDEX,-1);

	/* Vacio */
	setBlankProduction(CONDICIONAL,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);
}

void condicional1(){
	/* ? */
	struct production pr1[1];
	pr1[0].type = TERMINAL;
	pr1[0].index = OP_CONDICION_INDEX;
	clone_production(1,pr1,CONDICIONAL1,OP_CONDICION_INDEX,-1);

	/* Vacio */
	setBlankProduction(CONDICIONAL1,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,_EOF_INDEX,-1);
}

void comentario(){
	/* COMMENT */
	struct production pr1[1];
	pr1[0].type = TERMINAL;
	pr1[0].index = COMMENT_INDEX;
	clone_production(1,pr1,COMENTARIO,COMMENT_INDEX,-1);

	/* Vacio */
	setBlankProduction(COMENTARIO,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);
}

void expresion9(){
	/*  ? exp9 */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = OP_CONDICION_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = EXPRESION9;
	clone_production(2,pr1,EXPRESION9,OP_CONDICION_INDEX,-1);

	/* exp3 */
	struct production pr2[1];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = EXPRESION3;
	clone_production(1,pr2,EXPRESION9,OP_SUMA_INDEX,OP_MUL_INDEX,OP_RELACIONAL_INDEX,-1);

	/* Vacio */
	setBlankProduction(EXPRESION9,ID_INDEX,_EOF_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,-1);
}

void coffeescript(){
	/* body eof */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = BODY;
	pr1[1].type = TERMINAL;
	pr1[1].index = _EOF_INDEX;
	clone_production(2,pr1,COFFEESCRIPT,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,-1);
	
	/* Vacio */
	setBlankProduction(COFFEESCRIPT,_EOF_INDEX,-1);
}

void terminadores(){
	/* terminador terminadores1 */
	struct production pr1[2];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = TERMINADOR;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = TERMINADORES1;
	clone_production(2,pr1,TERMINADORES,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,-1);

	/* POP */
	setPopProduction(TERMINADORES,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,OP_CONDICION_INDEX,-1);
}

void terminadores1(){
	/* terminadores */
	struct production pr1[1];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = TERMINADORES;
	clone_production(1,pr1,TERMINADORES1,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,-1);

	/* Vacio */
	setBlankProduction(TERMINADORES1,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);
}

void terminador(){
	/* ; */
	struct production pr1[1];
	pr1[0].type = TERMINAL;
	pr1[0].index = TERMINADOR_PUNTOCOMA_INDEX;
	clone_production(1,pr1,TERMINADOR,TERMINADOR_PUNTOCOMA_INDEX,-1);

	/* \n */
	struct production pr2[1];
	pr2[0].type = TERMINAL;
	pr2[0].index = TERMINADOR_ENTER_INDEX;
	clone_production(1,pr2,TERMINADOR,TERMINADOR_ENTER_INDEX,-1);

	/* POP */
	setPopProduction(TERMINADOR,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,OP_CONDICION_INDEX,-1);
}

void linea(){
	/* exp condicional comentario */
	struct production pr1[3];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = EXPRESION;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = CONDICIONAL;
	pr1[2].type = NO_TERMINAL;
	pr1[2].index = COMENTARIO;
	clone_production(3,pr1,LINEA,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);

	/* comentario */
	struct production pr2[1];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = COMENTARIO;
	clone_production(1,pr2,LINEA,COMMENT_INDEX,-1);

	/* Vacio */
	setBlankProduction(LINEA,PR_IF_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,_EOF_INDEX,-1);

}

void operador(){
	/* SUM */
	struct production pr1[1];
	pr1[0].type = TERMINAL;
	pr1[0].index = OP_SUMA_INDEX;
	clone_production(1,pr1,OPERADOR,OP_SUMA_INDEX,-1);

	/* MUL */
	struct production pr2[1];
	pr2[0].type = TERMINAL;
	pr2[0].index = OP_MUL_INDEX;
	clone_production(1,pr2,OPERADOR,OP_MUL_INDEX,-1);

	/* REL */
	struct production pr3[1];
	pr3[0].type = TERMINAL;
	pr3[0].index = OP_RELACIONAL_INDEX;
	clone_production(1,pr3,OPERADOR,OP_RELACIONAL_INDEX,-1);

	/* POP */
	setPopProduction(OPERADOR,ID_INDEX,L_PARENTESIS_INDEX,PR_ALERT_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);
}

void f_alert(){
	/* ALERT parametros */
	struct production pr1[2];
	pr1[0].type = TERMINAL;
	pr1[0].index = PR_ALERT_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = PARAMETROS;
	clone_production(2,pr1,F_ALERT,PR_ALERT_INDEX,-1);
	
	/* POP */
	setPopProduction(F_ALERT,ID_INDEX,L_PARENTESIS_INDEX,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}

void expresion2(){
	/* ( parametros ) */
	struct production pr1[3];
	pr1[0].type = TERMINAL;
	pr1[0].index = L_PARENTESIS;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = PARAMETROS;
	pr1[2].type = TERMINAL;
	pr1[2].index = R_PARENTESIS;
	clone_production(3,pr1,EXPRESION2,L_PARENTESIS_INDEX,-1);

	/* valor1 exp4 */
	struct production pr2[2];
	pr2[0].type = NO_TERMINAL;
	pr2[0].index = VALOR1;
	pr2[1].type = NO_TERMINAL;
	pr2[1].index = EXPRESION4;
	clone_production(2,pr2,EXPRESION2,L_CORCHETE_INDEX,-1);

	/* = exp */
	struct production pr3[2];
	pr3[0].type = TERMINAL;
	pr3[0].index = OP_ASIGNACION_INDEX;
	pr3[1].type = NO_TERMINAL;
	pr3[1].index = EXPRESION;
	clone_production(2,pr3,EXPRESION2,OP_ASIGNACION_INDEX,-1);

	/* operador exp */
	struct production pr4[2];
	pr4[0].type = NO_TERMINAL;
	pr4[0].index = OPERADOR;
	pr4[1].type = NO_TERMINAL;
	pr4[1].index = EXPRESION;
	clone_production(2,pr4,EXPRESION2,OP_SUMA_INDEX,OP_MUL_INDEX,OP_RELACIONAL_INDEX,-1);

	/* Vacio */
	setBlankProduction(EXPRESION2,PR_ALERT_INDEX,LITERAL_NUM_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,-1);
}

void expresion10(){
	/* term */
	struct production pr1[1];
	pr1[0].type = NO_TERMINAL;
	pr1[0].index = TERM;
	clone_production(1,pr1,EXPRESION10,DELIMITADOR_CODIGO,-1);

	/* Vacio */
	setBlankProduction(EXPRESION10,PR_ALERT_INDEX,LITERAL_NUM_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,TERMINADOR_PUNTOCOMA_INDEX,TERMINADOR_ENTER_INDEX,_EOF_INDEX,L_PARENTESIS_INDEX,L_CORCHETE_INDEX,-1);
}

void expresion(){
	/* id exp3 exp9 */
	struct production pr1[3];
	pr1[0].type = TERMINAL;
	pr1[0].index = ID_INDEX;
	pr1[1].type = NO_TERMINAL;
	pr1[1].index = EXPRESION2;
	pr1[2].type = NO_TERMINAL;
	pr1[2].index = EXPRESION9;
	clone_production(3,pr1,EXPRESION,ID_INDEX,-1);

	/* ( exp5 ) exp10 exp9 */
	struct production pr2[5];
	pr2[0].type = TERMINAL;
	pr2[0].index = L_PARENTESIS_INDEX;
	pr2[1].type = NO_TERMINAL;
	pr2[1].index = EXPRESION5;
	pr2[2].type = TERMINAL;
	pr2[2].index = R_PARENTESIS;
	pr2[3].type = NO_TERMINAL;
	pr2[3].index = EXPRESION10;
	pr2[4].type = NO_TERMINAL;
	pr2[4].index = EXPRESION9;
	clone_production(5,pr2,EXPRESION,L_PARENTESIS_INDEX,-1);

	/* otrovalor exp2 exp9 */
	struct production pr3[3];
	pr3[0].type = NO_TERMINAL;
	pr3[0].index = OTRO_VALOR;
	pr3[1].type = NO_TERMINAL;
	pr3[1].index = EXPRESION2;
	pr3[2].type = NO_TERMINAL;
	pr3[2].index = EXPRESION9;
	clone_production(3,pr3,EXPRESION,LITERAL_NUM_INDEX,LITERAL_CADENA_INDEX,PR_TRUE_INDEX,PR_FALSE_INDEX,L_CORCHETE_INDEX,-1);

	/* f_alert exp9 */
	struct production pr4[2];
	pr4[0].type = NO_TERMINAL;
	pr4[0].index = F_ALERT;
	pr4[1].type = NO_TERMINAL;
	pr4[1].index = EXPRESION9;
	clone_production(2,pr4,EXPRESION,PR_ALERT_INDEX,-1);

	/* Pop */	
	setPopProduction(EXPRESION,COMMENT_INDEX,PR_IF_INDEX,R_PARENTESIS_INDEX,OP_CONDICION_INDEX,-1);
}


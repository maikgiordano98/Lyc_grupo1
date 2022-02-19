%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "symbol_table.h"
#include "Cola.h"
#include "Pila.h"
#include "Lista.h"
#include "funciones.h"

#define VAL_SZ 255

void yyerror(char *s, ...);
int yylex(void);

extern int yylineno;
FILE *lexout;

/* Tabla de simbolos */
List sym_table;
void AddInteger(const char *const val);
void AddReal(const char *const val);
void AddString(const char *const val);
void AddIds(const char *const ids, const char *const types);
Symbol *Lookup(const List *const lst, const char *const name);
void CheckId(const char *const id);

/* Polaca */
t_lista polacaLista;
t_cola cola;
t_pila pila;
t_pila condicionesOR;
t_pila simbolosASS;
t_pila rellenar;
t_pila maximoPila;

char simboloComparacion[4];
int posicionPolaca = 0;

int condicionCompuesta = 0;
int posicionComparacion = 0;
int banderaOR = 0;
int siguientePolaca = 0;
int pilaTam = 0;
int banderaMaxAnidado = 0;
int finmax = -1;

void verificaCondicion();
void invertirSimbolo(char* );

%}

%union {
	char strval[255 + 1];
}

%token <strval> ID

%token LL_ABR
%token LL_CRR
%token CR_ABR
%token CR_CRR
%token PR_ABR
%token PR_CRR

%token <strval> CONST_INT
%token <strval> CONST_R
%token <strval> STR

%token SUM
%token MIN
%token DIV
%token MULT

%token EQ
%token NEQ
%token LT
%token LEQ
%token GT
%token GEQ
%token NOT
%token AND
%token OR

%token IF
%token ELSE
%token WHILE

%token ASIGN

%token VAR
%token AS
%token COMA

%token INT
%token REAL
%token STRING_T

%token END_STMT
%token NL

%token GET
%token DISPLAY
%token LEN

%token CONST_NOMBRE

%right ASIGN /* assignment */
%left  SUM MIN /* left associative, same precedence. a-b-c will be (a-b)-c */
%left  MULT DIV /* left associative, higher precedence. */

/* Para poner ids y tipos en la tabla de simbolos */
%type <strval> ids lista_ids tipo tipos

%%

prg_: prg {printf("Compilacion exitosa!\n");};

prg: bloq
	 | prg bloq /* un programa y una lista de sentencias */
	 ;

bloq: flowcontr
		| sent
		| bloq flowcontr
		| bloq sent
		;

flowcontr: IF conds LL_ABR bloq LL_CRR ELSE { verificaCondicion(); }LL_ABR bloq LL_CRR {printf("IF CON ELSE ");}
				 | IF conds LL_ABR bloq LL_CRR {verificaCondicion(); printf("IF SIMPLE ");}
				 | WHILE conds LL_ABR bloq LL_CRR {verificaCondicion(); printf("WHILE ");}
				 ;

conds: cond {printf("Condicion ");}
		 | conds AND {condicionCompuesta++;} cond {printf("Condicion multiple ");}
		 | conds OR {condicionCompuesta++;} cond {banderaOR++; siguientePolaca = posicionPolaca+1; printf("Condicion multiple ");}
		 ;

cond: NOT cond {printf("Negacion de condicion ");}
		| PR_ABR cond PR_CRR
		| operando oplog operando { enlistar(&polacaLista, "CMP", posicionPolaca); posicionPolaca++;
									enlistar(&polacaLista, simboloComparacion, posicionPolaca); apilarEntero(&condicionesOR, posicionPolaca);
									apilar(&simbolosASS, simboloComparacion); posicionPolaca++;
									printf("VOY A ENLISTAR DPS DE ENLISTAR CMP   %s\n", simboloComparacion); 
								  }
		;

oplog: EQ {strcpy(simboloComparacion, "BNE"); printf("EQ ");}
		 | NEQ {strcpy(simboloComparacion, "BEQ"); printf("NEQ ");}
		 | LT {strcpy(simboloComparacion, "BGT"); printf("LT ");}
		 | LEQ {strcpy(simboloComparacion, "BGE"); printf("LEQ ");}
		 | GT {strcpy(simboloComparacion, "BLE"); printf("GT ");}
		 | GEQ {strcpy(simboloComparacion, "BLT"); printf("GEQ ");}
		 ;

sent: decl endstmt
		| assg endstmt
		| iostmt endstmt
		| endstmt
		;

endstmt: END_STMT | NL {printf("\n");};

decl: VAR lista_ids AS CR_ABR tipos CR_CRR {printf("Declaracion de variables "); AddIds($2, $5);}
	 	| CONST_NOMBRE ID ASIGN CONST_INT {enlistar(&polacaLista, $2, posicionPolaca); posicionPolaca++;
			 							   enlistar(&polacaLista, $4, posicionPolaca); posicionPolaca++;
            							   enlistar(&polacaLista, "=", posicionPolaca); posicionPolaca++;
										   printf("CONST_NOMBRE ID ASIGN CONST_INT"); AddConstantNombre($2, $4, TABLE_INT);}
		| CONST_NOMBRE ID ASIGN CONST_R {enlistar(&polacaLista, $2, posicionPolaca); posicionPolaca++;
										 enlistar(&polacaLista, $4, posicionPolaca); posicionPolaca++;
            							 enlistar(&polacaLista, "=", posicionPolaca); posicionPolaca++;
										 printf("CONST_NOMBRE ID ASIGN CONST_R"); AddConstantNombre($2, $4, TABLE_REAL);}
		| CONST_NOMBRE ID ASIGN STR {enlistar(&polacaLista, $2, posicionPolaca); posicionPolaca++;
									 enlistar(&polacaLista, $4, posicionPolaca); posicionPolaca++;
            						 enlistar(&polacaLista, "=", posicionPolaca); posicionPolaca++;
									 printf("CONST_NOMBRE ID ASIGN STR"); AddConstantString($2, $4);}
		;

lista_ids: CR_ABR ids CR_CRR {strcpy($$, $2);}
		 ;


ids: ID {strcpy($$, $1);}
	 | ids COMA ID {strcpy($$, $1); strcat($$, ","); strcat($$, $3);}
	 ;

tipos: tipo {strcpy($$, $1);}
		 | tipos COMA tipo {strcpy($$, $1); strcat($$, ","); strcat($$, $3);}
		 ;

tipo: REAL {strcpy($$, "real");} | STRING_T {strcpy($$, "string");} | INT {strcpy($$, "int");};

/* =========================
 * ASIGNACION
 * ========================= */

assg: ID ASIGN assg {CheckId($1); enlistar(&polacaLista, $1, posicionPolaca); posicionPolaca++;
					   enlistar(&polacaLista, "=", posicionPolaca); posicionPolaca++;
					   printf("Asignacion multiple ");}
		| ID ASIGN right {CheckId($1); enlistar(&polacaLista, $1, posicionPolaca); posicionPolaca++;
					   		enlistar(&polacaLista, "=", posicionPolaca); posicionPolaca++;
							printf("Asignacion ");}
		;

right: expr
		 | str_const
		 ;

str_const: STR {printf("CONST_STR "); AddString($1); enlistar(&polacaLista, $1, posicionPolaca); posicionPolaca++;}
	 ;

/* =========================
 * ARITMETICA
 * ========================= */

expr: expr SUM termino {enlistar(&polacaLista, "+", posicionPolaca); posicionPolaca++; printf("Operacion aritmetica SUM ");}
		| expr MIN termino {enlistar(&polacaLista, "-", posicionPolaca); posicionPolaca++;  printf("Operacion aritmetica MIN ");}
		| termino
		;

termino: termino MULT factor {enlistar(&polacaLista, "*", posicionPolaca); posicionPolaca++; printf("Multiplicacion: ");}
			 | termino DIV factor { enlistar(&polacaLista, "/", posicionPolaca); posicionPolaca++; printf("Division: ");}
			 | factor
			 ;

factor: PR_ABR expr PR_CRR {printf("Expresion en parentesis ");}
		| llong     {printf("LONGITUD ");}
		| ID        {CheckId($1); enlistar(&polacaLista, $1, posicionPolaca); posicionPolaca++; printf("ID ");}
		| const_num
		;

const_num: CONST_R   {enlistar(&polacaLista, $1, posicionPolaca); posicionPolaca++; printf("CONST_R "); AddReal($1);}
		| CONST_INT {enlistar(&polacaLista, $1, posicionPolaca); posicionPolaca++; printf("CONST_INT "); AddInteger($1);}
		;

llong: LEN PR_ABR lista_ids PR_CRR
		 | LEN PR_ABR lista_const PR_CRR
		 ;

lista_const: CR_ABR constantes CR_CRR
					 ;

constantes: constante
					| constantes COMA constante
					;

constante: const_num 
			| str_const
			;

/* =========================
 * IO
 * ========================= */

iostmt: DISPLAY operando {enlistar(&polacaLista, "DISPLAY", posicionPolaca); posicionPolaca++;
						printf("DISPLAY ");}
			| GET ID {enlistar(&polacaLista, $2, posicionPolaca); posicionPolaca++; 
					  enlistar(&polacaLista, "GET", posicionPolaca); posicionPolaca++;
					  CheckId($2); printf("GET ");}
			;

operando: expr | str_const;

%%
/* end of grammar */

int main(int argc, char *argv[]) {
	sym_table = NewList();

	lexout = fopen("lex.out", "wt");

    crear_lista(&polacaLista);
    crear_pila(&pila);
    crear_pila(&simbolosASS);
    crear_pila(&rellenar);
    crear_pila(&maximoPila);
    crear_cola(&cola);

	yyparse();


	/* Guardo la tabla */
	FILE *tstxt = fopen("ts.txt", "wt");
	fprintf(tstxt, "%40s%10s%40s%10s\n", "Nombre", "Tipo", "Valor", "Longitud");
	ListIterator it = Iterator(&sym_table);
	while ( HasNext(&it) ) {
		Symbol sym = Next(&it);
		char *type;
		switch ( sym.type ) {
		case TABLE_INT:
			type = sym.name[0]  == '_' ? "cte_int" : "Integer"; 
			break;
		case TABLE_REAL:
			type = sym.name[0]  == '_' ? "cte_real" : "Real";
			break;
		case TABLE_STRING:
			type = sym.name[0]  == '_' ? "cte_str" : "String";
			break;
		default:
			fprintf(stderr, "Tipo de dato invalido");
			exit(1);
		}
		fprintf(tstxt, "%40s%10s%40s%10d\n", sym.name, type, sym.value, sym.len);
	}

	vaciar_lista_INTERMEDIO(&polacaLista, posicionPolaca);

}

/**
 * Crea un mensaje del tipo "Cerca de la linea x: ... \n". "..." con lo que se
 * mande en fmt. Funciona como printf
 */
void yyerror(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	/* Creo el comienzo del mensaje */
	char buf[4096];
	snprintf(buf, 4096 - 1, "Cerca de la linea %d: ", yylineno);

	/* le agrego el mensaje formateado */
	const size_t len = strlen(buf);
	vsnprintf(buf + len, 4096 - len - 1, fmt, ap);

	/* un \n para que quede bien */
	strcat(buf, "\n");

	fflush(stdout); /* por si stdout es igual que stderr */

	/* imprimo el mensaje de error */
	fputs(buf, stderr);
	fflush(NULL); /* hago flush de todos */

	va_end(ap);
}

void doAddConstant(const char *const val, DataType type)
{
	Symbol s;
	strcpy(s.name, "_");
	strcat(s.name, val);
	s.type = type;
	strcpy(s.value, val);
	if(type == TABLE_STRING)
		s.len = strlen(val);
	else
		s.len = 0;
	AddSymbol(&sym_table, &s);
}

void AddConstantNombre(const char *const name, const char *const val, DataType type)
{
	Symbol s;
	strcpy(s.name, name);
	strcpy(s.value, val);
	s.type = type;
	if(type == TABLE_STRING)
		s.len = strlen(val);
	else
		s.len = 0;
	AddSymbol(&sym_table, &s);
}

void AddConstantString(const char *const name, const char *const val)
{
	char buf[SYM_NAME_SZ];
	/* saco la primer comilla */
	strcpy(buf, val+1);
	/* saco la ultima comilla */
	buf[strlen(buf)-1] = 0;

	AddConstantNombre(name, buf, TABLE_STRING);
}

void AddInteger(const char *const val)
{
	doAddConstant(val, TABLE_INT);
}


void AddReal(const char *const val)
{
	doAddConstant(val, TABLE_REAL);
}


void AddString(const char *const val)
{
	char buf[SYM_NAME_SZ];
	/* saco la primer comilla */
	strcpy(buf, val+1);
	/* saco la ultima comilla */
	buf[strlen(buf)-1] = 0;

	doAddConstant(buf, TABLE_STRING);
}

void addId(const char *id, DataType type)
{
	Symbol s;
	strcpy(s.name, id);
	s.type = type;
	strcpy(s.value, "");
	s.len = 0;
	AddSymbol(&sym_table, &s);
}


void AddIds(const char *const ids, const char *const types)
{
	const char *id_start = ids, *id_comma = ids;
	const char *type_start = types, *type_comma = types;

	while ( *id_start && *type_start ) {
		while ( *id_comma && *id_comma != ',' )
				id_comma++;
		while ( *type_comma && *type_comma != ',' )
				type_comma++;

		char id[255];
		strncpy(id, id_start, id_comma - id_start);
		id[id_comma - id_start] = 0;
		DataType type;
		if (strncmp("real", type_start, type_comma - type_start) == 0) {
			type = TABLE_REAL;
		} else if ( strncmp("int", type_start, type_comma - type_start) == 0 ) {
			type = TABLE_INT;
		} else if ( strncmp("string", type_start, type_comma - type_start) == 0 ) {
			type = TABLE_STRING;
		}

		addId(id, type);

		/*
		 * lo pongo en el principio del proximo id en caso de que no se haya
		 * encontrado el \0
		 */
		id_start = id_comma = *id_comma != 0 ? id_comma + 1 : id_comma;
		type_start = type_comma = *type_comma != 0 ? type_comma + 1 : type_comma;
	}

	if ( *id_start && !*type_start ) {
		yyerror("Error de declaracion: la cantidad de tipos de datos debe ser "
			"igual a la cantidad de identificadores.");
		exit(1);
	}

	if ( !*id_start && *type_start ) {
		yyerror("Error de declaracion: la cantidad de identificadores debe ser "
			"igual a la cantidad de tipos de datos.");
		exit(1);
	}
}

/**
 * Verifica que un id se encuentre en la tabla de simbolos.
 * Si no se encuentra, DISPLAY de error y exit(1);
 */
void CheckId(const char *const id)
{
	if ( Lookup(&sym_table, id) == NULL ) { /* no se ha declarado */
		yyerror("No se ha declarado la variable \"%s\"", id);
		exit(1);
	}
}

/**
 * Realiza una busqueda en la lista por name del Symbol
 * En caso de no encontrarlo, retorna NULL.
 * El puntero retornado es una variable static, por lo tanto, no deberia ser
 * utilizado luego de otra llamada a Lookup, dado que su valor puede cambiar.
 */
Symbol *Lookup(const List *const lst, const char *const name)
{
	ListIterator it = Iterator(lst);
	while ( HasNext(&it) ) {
		static Symbol s;
		s = Next(&it);
		if ( strcmp(s.name, name) == 0 )
			return &s;
	}

	return NULL;
}

void invertirSimbolo(char* aux){
	
	if(strcmp(aux, "BEQ") == 0){
					strcpy(aux, "BNE");
				}else if(strcmp(aux, "BNE") == 0){
					strcpy(aux, "BEQ");
				} else if(strcmp(aux, "BLT") == 0){
					strcpy(aux, "BGE");
				}else  if(strcmp(aux, "BLE") == 0){
					strcpy(aux, "BGT");
				}else	if(strcmp(aux, "BGT") == 0){
					strcpy(aux, "BLE");
				}else{
					strcpy(aux, "BLT");
				}
}

void verificaCondicion(){
	if(condicionCompuesta){
		condicionCompuesta = 0;
		if(banderaOR){
				char aux1[5];
				char aux2[5];
				desapilar(&simbolosASS, aux1);
				desapilar(&simbolosASS, aux2);

				invertirSimbolo(aux2);
				
				banderaOR = 0;
				rellenarPolacaChar(&polacaLista, desapilarEntero(&condicionesOR), aux1);
				rellenarPolacaChar(&polacaLista, desapilarEntero(&condicionesOR), aux2);
				rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
				rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), siguientePolaca);
		}else{
		
		rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
		rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
		}	
	}
	else{
		rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
	} 
}

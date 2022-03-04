%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "y.tab.h"
#include "funciones.h"



tabla tablaSimbolos;
tabla constantes;

t_lista polacaLista;
t_cola cola;
t_pila pilaSint;
t_pila condicionesOR;
t_pila simbolosASS;
t_pila rellenar;

char simboloComparacion[4];

int posicionPolaca = 0;
int condicionCompuesta = 0;
int posicionComparacion = 0;
int banderaOR = 0;
int siguientePolaca = 0;
int condicionNot = 0;
int posicionPolacaWhile = 0;

char *prueba;

int yylex();
void yyerror(const char *);
void verificaCondicion();
void invertirSimbolo(char* );

extern char* yytext;
extern int yylineno;

extern FILE* yyin;

%}


%union{
  char* strVal;
}

%token <strVal> ID

%token LL_CRR		
%token LL_ABR		
%token CR_CRR		
%token CR_ABR		
%token PR_CRR		
%token PR_ABR	

%token <strVal> CONST_INT 	
%token <strVal> CONST_R 	
%token <strVal> CONST_STR	

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

%token <strVal> REAL		
%token <strVal> INT	
%token <strVal> STRING_T

%token END_STMT

%token GET		
%token DISPLAY

%token CONST_NOMBRE  	

%right ASIGN /* assignment */
%left SUM MIN /* left associative, same precedence. a-b-c will be (a-b)-c */
%left MULT DIV /* left associative, higher precedence. */

%nonassoc IF	
%nonassoc ELSE


%%

prg_: prg {
						printf("Compilacion exitosa!\n");
						generarAssembler(&tablaSimbolos, &polacaLista, posicionPolaca);
					};    

prg: bloq
	 | prg bloq
	 ;

bloq: iteracion
    | seleccion
    | asignacion
    | imprimir
    | leer
    | declaracion
    ;

declaracion:  VAR CR_ABR dupla_asig CR_CRR {   
								char id[100];
								char auxString[100];
								char tipoVar[20];
								while(!pila_vacia(&pilaSint) || !cola_vacia(&cola) ) {
									desapilar(&pilaSint, id);
									desacolar(&cola, tipoVar);
									if(strcmp(tipoVar, "String") == 0)
										insertar(id, ES_STRING, &tablaSimbolos,NO_ES_CONSTANTE, tipoVar );
									else
										insertar2(id, SIN_VALOR, &tablaSimbolos,NO_ES_CONSTANTE, tipoVar, auxString);
								}
							}
						;   

dupla_asig: ID COMA dupla_asig  COMA tipo { apilar(&pilaSint, $1); }
          | ID CR_CRR AS CR_ABR tipo { apilar(&pilaSint, $1); }
          ;

tipo: REAL {acolar(&cola, "REAL"); }
    | INT { acolar(&cola, "Integer"); }
    | STRING_T { acolar(&cola, "String"); }
    ;

iteracion: WHILE  { 
							posicionPolacaWhile = posicionPolaca; 
							enlistar(&polacaLista, "WHILE", posicionPolaca); 
							posicionPolaca++; 
						} condicion LL_ABR prg { 
							if(posicionPolacaWhile > 0) {
								char cadena[15];
								enlistar(&polacaLista, "BI", posicionPolaca); posicionPolaca++;
								enlistar(&polacaLista, itoa(posicionPolacaWhile+1, cadena, 10), posicionPolaca); posicionPolaca++;
								posicionPolacaWhile = 0;	
							}
						} LL_CRR { verificaCondicion(); }
         ;

seleccion: seleccion_con_else
         | seleccion_sin_else
         ;

seleccion_con_else: IF condicion argumento_sel  ELSE { verificaCondicion(); } 
										argumento_sel
                	| IF PR_ABR condicion PR_CRR argumento_sel  ELSE { verificaCondicion(); } 
										argumento_sel
                	;

seleccion_sin_else: IF condicion argumento_sel { verificaCondicion(); }
                  | IF PR_ABR condicion PR_CRR argumento_sel { verificaCondicion(); }
									;

argumento_sel: LL_ABR prg LL_CRR
             | bloq
             ;

/* =========================
 * ASIGNACION
 * ========================= */

asignacion: ID ASIGN expresion { 	
							enlistar(&polacaLista, "=", posicionPolaca); posicionPolaca++;
							char auxString[100];
							if(esta_en_Lista(&constantes,$1) == 1){
								insertar2($1, SIN_VALOR, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString);  
							}else{
								strcpy(auxString, "_");
								strcat(auxString, $1);
								strcat(auxString, "_esddfloat");
								prueba = str_replace(".", "_", auxString);
								strcpy(auxString, prueba);
							}
							enlistar(&polacaLista, auxString, posicionPolaca); posicionPolaca++;
						}
          | CONST_NOMBRE  ID ASIGN expresion {  
							enlistar(&polacaLista, ":", posicionPolaca); posicionPolaca++;
							char auxString[100];
							char auxConst[100];
							//strcpy(auxConst, "const_");
							//strcat(auxConst, $2);
							insertar2($2, SIN_VALOR, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString); 
							tuplaTabla* nuevo;
							nuevo = (tuplaTabla*) malloc(sizeof(tuplaTabla));
							if(!nuevo){
								printf("Error, no hay memoria\n.");
								return -1;
							}
							strcpy(nuevo->constValor, $2);
							enlistar_en_ordenValor(&constantes, nuevo );
							enlistar(&polacaLista, auxString, posicionPolaca); posicionPolaca++;
						}
          ;

/* =========================
 * IO
 * ========================= */

imprimir: DISPLAY {enlistar(&polacaLista, "DISPLAY", posicionPolaca); posicionPolaca++; } displayable;

displayable: expresion | CONST_STR;

leer: GET ID {  
				enlistar(&polacaLista, "GET", posicionPolaca); posicionPolaca++;
				char auxString[100];
				if(esta_en_Lista(&constantes,$2) == 1){
					insertar2($2, CON_VALOR, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString); 
				} 
				else {
					strcpy(auxString, "_");
					strcat(auxString, $2);
					strcat(auxString, "_esddfloat");
					prueba = str_replace(".", "_", auxString);
					strcpy(auxString, prueba);
				}
				enlistar(&polacaLista, auxString, posicionPolaca); posicionPolaca++; 
			}
    ;

/* =========================
 * CONDICIONES
 * ========================= */

condicion: comparacion
				 | condicion AND {
							condicionCompuesta++; 
							enlistar(&polacaLista, "_TOKENAND_", posicionPolaca);
							posicionPolaca++;
						} comparacion
         | condicion OR {
							condicionCompuesta++; 
							enlistar(&polacaLista, "_TOKENOR_", posicionPolaca);
							posicionPolaca++;
						} comparacion { 
							banderaOR++; 
							siguientePolaca = posicionPolaca+1; 
						}
         | NOT { condicionNot ++; } comparacion
         ;

comparacion: expresion comparador termino {
								enlistar(&polacaLista, "CMP", posicionPolaca); posicionPolaca++;
								if(condicionNot > 0){
									invertirSimbolo(simboloComparacion);
									condicionNot = 0;
								}
								enlistar(&polacaLista, simboloComparacion, posicionPolaca); 
								apilarEntero(&condicionesOR, posicionPolaca);
								apilar(&simbolosASS, simboloComparacion); posicionPolaca++;
								enlistar(&polacaLista, "  ", posicionPolaca );  
								apilarEntero(&rellenar, posicionPolaca);  posicionPolaca++;
							}
					 | PR_ABR	expresion comparador termino	PR_CRR  {
								enlistar(&polacaLista, "CMP", posicionPolaca); posicionPolaca++;
								if(condicionNot > 0){
									invertirSimbolo(simboloComparacion);
									condicionNot = 0;
								}
								enlistar(&polacaLista, simboloComparacion, posicionPolaca); 
								apilarEntero(&condicionesOR, posicionPolaca);
								apilar(&simbolosASS, simboloComparacion); posicionPolaca++;
								enlistar(&polacaLista, "  ", posicionPolaca );  
								apilarEntero(&rellenar, posicionPolaca);  posicionPolaca++;
						 	}
					 ;


comparador: NEQ { strcpy(simboloComparacion, "BEQ"); }
    		  | EQ { strcpy(simboloComparacion, "BNE"); }
    		  | GEQ { strcpy(simboloComparacion, "BLT"); }
    		  | GT { strcpy(simboloComparacion, "BLE"); }
    		  | LEQ { strcpy(simboloComparacion, "BGT"); }
    		  | LT { strcpy(simboloComparacion, "BGE"); }
          ;


/* =========================
 * ARITMETICA
 * ========================= */

expresion: expresion SUM termino {
							enlistar(&polacaLista, "+", posicionPolaca); 
							posicionPolaca++; 
						}
      	 | expresion MIN termino {
							enlistar(&polacaLista, "-", posicionPolaca); 
							posicionPolaca++; 
						}
      	 | termino
      	 ;

termino: termino MULT elemento {  
						enlistar(&polacaLista, "*", posicionPolaca); 
						posicionPolaca++; 
					}
       | termino DIV elemento {  
						enlistar(&polacaLista, "/", posicionPolaca); 
						posicionPolaca++; 
					}
       | elemento
       ;

elemento: PR_ABR expresion PR_CRR
        | ID {
						char auxString[100];
						if(esta_en_Lista(&constantes,$1) == 1){
							insertar2($1, CON_VALOR, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString); 
						} 
						else {
							strcpy(auxString, "_");
							strcat(auxString, $1);
							strcat(auxString, "_esddfloat");
							prueba = str_replace(".", "_", auxString);
							strcpy(auxString, prueba);
						}
						enlistar(&polacaLista, auxString, posicionPolaca); 
						posicionPolaca++;  
					}
     	  | CONST_INT { 
						char auxString[100];
						insertar2($1, CON_VALOR, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString); 
						enlistar(&polacaLista, auxString, posicionPolaca); 
						posicionPolaca++; 
					}
      	| CONST_R {
						char auxString[100];
						insertar2($1, CON_VALOR, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString); 
						enlistar(&polacaLista, auxString, posicionPolaca);  
						posicionPolaca++;
					}
      	| CONST_STR { 
						char auxString[40];
						char caditoa[40];
						char aux[40];
						int i;
						//strcpy(aux, $1);
						//strcat(aux, itoa(posicionPolaca, caditoa, 10));
						strcpy(aux, $1);
						for(i = 0; aux[i]; i++){
							aux[i] = tolower(aux[i]);
						} 
						insertar2(aux, ES_STRING, &tablaSimbolos,NO_ES_CONSTANTE,"-", auxString); 
						for(i = 0; auxString[i]; i++){
							auxString[i] = tolower(auxString[i]);
						} 
						enlistar(&polacaLista, auxString, posicionPolaca); posicionPolaca++;   		
					}
      	;
%%
/* end of grammar */


int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL) {
    printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else {
		crearTabla(&constantes);
		crearTabla(&tablaSimbolos);
		crear_lista(&polacaLista);
		crear_pila(&pilaSint);
		crear_pila(&simbolosASS);
		crear_pila(&rellenar);
		crear_cola(&cola);
		crear_pila(&condicionesOR);
		yyparse();
  }
	fclose(yyin);
  return 0;
}

void verificaCondicion(){
	if(condicionCompuesta){
		condicionCompuesta = 0;
		if(banderaOR){
				char aux1[10];
				char aux2[10];
				desapilar(&simbolosASS, aux1);
				desapilar(&simbolosASS, aux2);
				invertirSimbolo(aux2);
				banderaOR = 0;
				rellenarPolacaChar(&polacaLista, desapilarEntero(&condicionesOR), aux1);
				rellenarPolacaChar(&polacaLista, desapilarEntero(&condicionesOR), aux2);
				rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);			
				rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), siguientePolaca);				
		}else {
			rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
			rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
		}	
	}
	else{
		rellenarPolaca(&polacaLista, desapilarEntero(&rellenar), posicionPolaca+1);
	}
}

void yyerror(const char* s)
{
	printf("Error Sintactico\n");
	system ("Pause");
	exit (1);
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitarias\Pila.h"
#include "utilitarias\Cola.h"
#include "utilitarias\Lista.h"

#define CON_VALOR 1
#define SIN_VALOR 0
#define ES_STRING 2
#define ES_CONSTANTE 3
#define NO_ES_CONSTANTE 5
#define ARCHIVO_DATA "data.txt"


typedef struct tupla{
	char* lexema;
	char* valor;
	char* tipo;
	char constante[3];
	char longitud[30];
	char constValor[200];
	char lex[200];
	struct tupla* siguiente;

}tuplaTabla;


typedef  tuplaTabla* tabla;


///////////////////////////////////////////////////////////////////

int insertar(char* , int ,tabla*  , int ,  char*);

int enlistar_en_orden(tabla* ,tuplaTabla* );

int vaciar_lista_TS(tabla* l);

void crearTabla(tabla* lista);

void eliminarCaracter(char *str, char garbage);

int mi_strlen(char* cadena);

int generarAssembler(t_lista *, t_lista* , int );

int esOperadorBinario(char *);

int esOperadorUnario(char *);

int insertar2(char* , int ,tabla*  , int , char* , char* );

char* str_replace(char* , char* , char* );

void invertirSimbolo2(char* );

void invertirCondicion(char* );

int enlistar_en_ordenValor(tabla* l,tuplaTabla* d);

int esta_en_Lista(tabla* l,char* d);

//////////////////////////////////////////////////////////////////



void crearTabla(tabla* lista){
	*lista = NULL;

}



int vaciar_lista_TS(tabla* l)
{
    tuplaTabla* viejo;
    char varAux[20];
    int bandera_0 = 0;
    int bandera_1 = 0;
    int bandConstante = 0;
    FILE* pf = fopen("ts.txt","w+");
    if(!pf){
    	printf("No se pudo abrir el archivo;\n");
    	return 0;
    }

	FILE *af = fopen(ARCHIVO_DATA,"w+");
	if(!af){
		printf("No se pudo abrir el archivo data.txt\n");
		return 0;
	}

   	fprintf(pf,"%s","LEXEMA\t\t\t\tVALOR\t\t\t\tCONSTANTE\t\t\tLONGITUD\t\t\tTIPO\n");

    while(*l)
    {
        viejo=*l;
        *l=viejo->siguiente;
        if(strcmp(viejo->valor, "0") == 0 || strcmp(viejo->valor, "0.0") == 0){
        	bandera_0 = 1;
        }
        if(strcmp(viejo->valor, "1") == 0 || strcmp(viejo->valor, "1.0") == 0){
        	bandera_1 = 1;
        }
        if(strstr(viejo->constante, "SI") != NULL){
        	bandConstante = 1;
        }
        fprintf(pf,"%s\t\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t%s\n", viejo->lexema, viejo->valor, viejo->constante, viejo->longitud, viejo->tipo);

   		if(strchr(viejo->valor, '"') != NULL){
   				fprintf(af,"%s\tdb\t%s , '$' , %s dup  (?)\n", viejo->lexema, viejo->valor, viejo->longitud);

   		}else{
   			if(strchr(viejo->valor, '-') != NULL){
   				if(bandConstante){
   					fprintf(af,"%s\tdd\t?\t;esddfloat\n", viejo->lexema);
   					bandConstante = 0;
   				}else{
   					fprintf(af,"%s\tdd\t?\t;esddfloat\n", viejo->lexema);
   				}
   			}
   			else{	
 				if(strchr(viejo->valor, '.') == NULL){
 					if(bandConstante){
 						fprintf(af,"%s\tdd\t%s.0\t;esddfloat\n",viejo->lexema, viejo->valor);
 						bandConstante = 0;
 					}else{
 						fprintf(af,"%s\tdd\t%s.0\t;esddfloat\n",viejo->lexema, viejo->valor);
 					}
 				}else{
 					if(bandConstante){
 						fprintf(af,"%s\tdd\t%s\t;esddfloat\n",viejo->lexema, viejo->valor);
 						bandConstante = 0;
 					}else{
 						fprintf(af,"%s\tdd\t%s\t;esddfloat\n",viejo->lexema, viejo->valor);
 					}
 				}

   			}

   		}

        free(viejo);
    }
    if(!bandera_1){
    	fprintf(af,"_1_esddfloat\tdd\t1.0\t;esddfloat\n");
    }
    if(!bandera_0){
    	fprintf(af,"_0_esddfloat\tdd\t0.0\t;esddfloat\n");
    }
    strcpy(varAux, "_varaux");
    fprintf(af,"_@max1\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max2\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max3\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max4\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max5\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max6\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max7\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max8\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max9\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@max10\tdd\t?\t;esddfloat\n");
    fprintf(af,"_@aux\tdd\t?\t;esddfloat\n");
    fprintf(af,"%s\tdd\t?\t;esddfloat\n", varAux);

    fclose(pf);
    fclose(af);
    return 1;

}


int insertar(char* lexemaE, int valor,tabla*  tablaSimbolos, int constante, char* tipo2){
	char auxiliar[35];
	tuplaTabla* nuevo;
	int resultado = 0;
	nuevo = (tuplaTabla*) malloc(sizeof(tuplaTabla));
	if(!nuevo){
		printf("Error, no hay memoria\n.");
		return -1;
	}

	/* SI ES UN STRING, LE SACO LAS COMILLAS, RESERVO LA MEMORIA Y ASIGNO LOS VALORES A LOS CAMPOS DE LA TUPLA*/
	if(valor == ES_STRING){
			nuevo->lexema = (char*) malloc(sizeof(char) * strlen(lexemaE) + 1);
			if(!(nuevo->lexema)){
				printf("Error, no hay memoria\n.");
				return -1;
			}
			strcpy(auxiliar, lexemaE);
			eliminarCaracter(auxiliar, '"');
			eliminarCaracter(auxiliar, ' ');
			strcpy(nuevo->lexema, "_");
			strcat(nuevo->lexema, auxiliar);
			nuevo->valor = (char*) malloc(sizeof(char) * strlen(lexemaE) + 1);
			if(!(nuevo->valor)){
				printf("Error, no hay memoria\n.");
				return -1;
			}
			if(constante == ES_CONSTANTE){
				strcpy(nuevo->constante,"SI");
			}else{
				strcpy(nuevo->constante,"NO");

			}
			strcpy(nuevo->valor, lexemaE);
			itoa(strlen(lexemaE), nuevo->longitud, 10);
	}else{/* SI NO ES UN STRING VERIFICO SI ES CON VALOR O SIN VALOR. PARA AMBOS CASOS ASIGNO EL NOMBRE AL LEXEMA Y RESERVO LA MEMORIA*/
		nuevo->lexema = (char*) malloc(sizeof(char) * strlen(lexemaE) + 2);

		if(!(nuevo->lexema)){
			printf("Error, no hay memoria\n.");
			return -1;
		}

		strcpy(nuevo->lexema, "_");
		strcpy(nuevo->longitud, "-");

		strcat(nuevo->lexema, lexemaE);
		nuevo->lexema = str_replace(".", "_", nuevo->lexema);
		


		nuevo->valor = NULL;
		if(constante != ES_CONSTANTE)
				strcpy(nuevo->constante,"NO");
		if(constante == ES_CONSTANTE){
				strcpy(nuevo->constante,"SI");
			}


		/*SI ES CON VALOR GUARDO EL VALOR DEL LEXEMA EN LA TUPLA*/
		if(valor == CON_VALOR){

			nuevo->valor = (char*) malloc(sizeof(char) * strlen(lexemaE) + 1);

			if(!(nuevo->valor)){
				printf("Error, no hay memoria\n.");
				return -1;
			}
			if(constante == ES_CONSTANTE){
				strcpy(nuevo->constante,"SI");
			}

			strcpy(nuevo->valor, lexemaE);

		}

	}
	if( valor== SIN_VALOR){
		nuevo->valor = (char*) malloc(sizeof(char) * 3);
		if(!(nuevo->valor)){
				printf("Error, no hay memoria\n.");
				return -1;
		}
		strcpy(nuevo->valor, "-");

	}

	nuevo->tipo = (char*) malloc(sizeof(char) * strlen(tipo2) + 2);
		if(!(nuevo->tipo)){
			printf("Error, no hay memoria\n.");
			return -1;
		}
		strcpy(nuevo->tipo,tipo2);

	/*LO INSERTO EN LA LISTA DE MANERA ORDENADA*/

	resultado = enlistar_en_orden(tablaSimbolos, nuevo);

	if(resultado == 0){
		free(nuevo);
		return 0;
	}

	return 1;


}



int insertar2(char* lexemaE, int valor,tabla*  tablaSimbolos, int constante, char* tipo2, char* retorno){
	char auxiliar[35];
	tuplaTabla* nuevo;
	int resultado = 0;
	nuevo = (tuplaTabla*) malloc(sizeof(tuplaTabla));
	if(!nuevo){
		printf("Error, no hay memoria\n.");
		return -1;
	}

	/* SI ES UN STRING, LE SACO LAS COMILLAS, RESERVO LA MEMORIA Y ASIGNO LOS VALORES A LOS CAMPOS DE LA TUPLA*/
	if(valor == ES_STRING){
			nuevo->lexema = (char*) malloc(sizeof(char) * strlen(lexemaE) + 1);
			if(!(nuevo->lexema)){
				printf("Error, no hay memoria\n.");
				return -1;
			}

			strcpy(auxiliar, lexemaE);
			eliminarCaracter(auxiliar, '"');
			strcpy(nuevo->lexema, "_");
			strcat(nuevo->lexema, auxiliar);
			eliminarCaracter(nuevo->lexema, ' ');
			if(strchr(nuevo->lexema, '0') != NULL){
				nuevo->lexema = str_replace("0", "_0", nuevo->lexema);
			}
			nuevo->lexema = str_replace(".", "_", nuevo->lexema);
			nuevo->lexema = str_replace("!", "", nuevo->lexema);
			nuevo->lexema = str_replace(":", "", nuevo->lexema);
			nuevo->lexema = str_replace("<", "", nuevo->lexema);
			nuevo->lexema = str_replace("<=", "", nuevo->lexema);
			nuevo->lexema = str_replace(">", "", nuevo->lexema);
			nuevo->lexema = str_replace(">=", "", nuevo->lexema);
			nuevo->lexema = str_replace("==", "", nuevo->lexema);
			nuevo->lexema = str_replace("<>", "", nuevo->lexema);
			nuevo->lexema = str_replace("=", "", nuevo->lexema);
			nuevo->lexema = str_replace("?", "", nuevo->lexema);



			strcpy(retorno, nuevo->lexema);

			nuevo->valor = (char*) malloc(sizeof(char) * strlen(lexemaE) + 1);
			if(!(nuevo->valor)){
				printf("Error, no hay memoria\n.");
				return -1;
			}
			if(constante == ES_CONSTANTE){
				strcpy(nuevo->constante,"SI");
			}else{
				strcpy(nuevo->constante,"NO");

			}
			strcpy(nuevo->valor, lexemaE);
			itoa(strlen(lexemaE), nuevo->longitud, 10);
	}else{/* SI NO ES UN STRING VERIFICO SI ES CON VALOR O SIN VALOR. PARA AMBOS CASOS ASIGNO EL NOMBRE AL LEXEMA Y RESERVO LA MEMORIA*/
		nuevo->lexema = (char*) malloc(sizeof(char) * strlen(lexemaE) + 25);

		if(!(nuevo->lexema)){
			printf("Error, no hay memoria\n.");
			return -1;
		}

		strcpy(nuevo->lexema, "_");
		strcpy(nuevo->longitud, "-");

		strcat(nuevo->lexema, lexemaE);
		strcat(nuevo->lexema, "_esddfloat");


		nuevo->lexema = str_replace(".", "_", nuevo->lexema);

		
		nuevo->valor = NULL;
		if(constante != ES_CONSTANTE)
				strcpy(nuevo->constante,"NO");
		if(constante == ES_CONSTANTE){
				strcpy(nuevo->constante,"SI");
			}


		/*SI ES CON VALOR GUARDO EL VALOR DEL LEXEMA EN LA TUPLA*/
		if(valor == CON_VALOR){

			nuevo->valor = (char*) malloc(sizeof(char) * strlen(lexemaE) + 1);
				if(!(nuevo->valor)){
				printf("Error, no hay memoria\n.");
				return -1;
			}
			if(constante == ES_CONSTANTE){
				strcpy(nuevo->constante,"SI");
			}
			strcpy(retorno, nuevo->lexema);
			strcpy(nuevo->valor, lexemaE);


		}

	}
	if( valor== SIN_VALOR){
		nuevo->valor = (char*) malloc(sizeof(char) * 3);
		if(!(nuevo->valor)){
				printf("Error, no hay memoria\n.");
				return -1;
		}
		strcpy(retorno, nuevo->lexema);
		strcpy(nuevo->valor, "-");

	}

	nuevo->tipo = (char*) malloc(sizeof(char) * strlen(tipo2) + 2);
		if(!(nuevo->tipo)){
			printf("Error, no hay memoria\n.");
			return -1;
		}
		strcpy(nuevo->tipo,tipo2);

	/*LO INSERTO EN LA LISTA DE MANERA ORDENADA*/
	resultado = enlistar_en_orden(tablaSimbolos, nuevo);

	if(resultado == 0){
		free(nuevo);
		return 0;
	}

	return 1;


}

void eliminarCaracter(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}


int enlistar_en_orden(tabla* l,tuplaTabla* d)
{
    tabla pm;
    int resultado = 0;
   while(*l && (resultado=strcmp((*l)->lexema,d->lexema))<=0)
   {
   		if(resultado == 0){
   			return 0;
   		}
       	l=&(*l)->siguiente;
   }
   if(!*l)
        {

            d->siguiente=NULL;
            *l=d;
            return 1;
        }
    d->siguiente=*l;
    *l=d;

    return 1;
}


int enlistar_en_ordenValor(tabla* l,tuplaTabla* d)
{
    tabla pm;
    int resultado = 0;
   while(*l && (resultado=strcmp((*l)->constValor,d->constValor))<=0)
   {
   		if(resultado == 0){
   			return 0;
   		}
       	l=&(*l)->siguiente;
   }
   if(!*l)
        {

            d->siguiente=NULL;
            *l=d;
            return 1;
        }
    d->siguiente=*l;
    *l=d;

    return 1;
}


int esta_en_Lista(tabla* l,char* d)
{
    tabla pm;
    int resultado = 0;
   while(*l && (resultado=strcmp((*l)->constValor,d))<=0)
   {
   		if(resultado == 0){

   			return 0;
   		}
       	l=&(*l)->siguiente;
   }
  
    return 1;
}




int mi_strlen(char* cadena){

	int i = 0;
	while(*cadena){
		i++;
		cadena++;
	}

	return ( i <= 30 )? 1:0;
}



int generarAssembler(t_lista *polaca, t_lista* intermedia, int posPolaca){
	char cadena1[200];
	char cadena2[200];
	char posSalto[200];
	char segundoOperando[200];
	char primerOperando[200];
	char operador[200];
	char primerSalto[200];
	char segundoSalto[200];
	char aux[150];
	char aux2[150];
	char aux3[150];
	char varAux[30];
	char* prueba;
	int nroPos = 1;
	int i = 1;
	int bandWhile = 0;
	int bandAND = 0;
	int bandOR = 0;
	int cierraComp = 0;
	int fSize = 0;

	t_pila pila;
	t_pila salto;
	t_pila etiquetas;
	t_pila whileEtiq;

	crear_pila(&pila);
	crear_pila(&salto);
	crear_pila(&etiquetas);
	crear_pila(&whileEtiq);

	vaciar_lista_TS(polaca);
	vaciar_lista_INTERMEDIO(intermedia, posPolaca);
	strcpy(varAux, "_varaux");

	FILE *af = fopen("Final.asm","w+");
	if(!af){
		printf("No se pudo abrir el archivo assembler.txt\n");
		return 0;
	}

	FILE *pf = fopen("auxiliar.txt","r+");
	if(!pf){
		printf("No se pudo abrir el archivo auxiliar.txt\n");
		return 0;
	}

	FILE *qf = fopen(ARCHIVO_DATA,"r+");
	if(!qf){
		printf("No se pudo abrir el archivo data.txt\n");
		return 0;
	}

	fprintf(af,"include macros2.asm\n");
	fprintf(af,"include number.asm\n");
	fprintf(af,".MODEL LARGE\n.386\n.STACK 200h\n\n");

	fprintf(af,".DATA\n;variables de la tabla de simbolos\n\n");
	while(fgets(cadena1, 200, qf) != NULL){
		fprintf(af,"%s", cadena1);
	}

	fprintf(af,"\n\n\n\n");  // agrego saltos de linea al archivo assembler

	fprintf(af,".CODE\n;comienzo de la zona de codigo\n\n\nstart:\n");
	fprintf(af,"MOV EAX,@DATA\n");
	fprintf(af,"MOV DS,EAX\n");
	fprintf(af,"MOV ES,EAX\n\n\n");

	while(fgets(cadena1, 200, pf) != NULL){
		nroPos++;
		eliminarCaracter(cadena1, '\n');
		apilar(&pila, cadena1);	
		verTope(&pila, cadena1);
		if(strcmp(cadena1, "WHILE") == 0){
			strcpy(aux, "_Etiq");
			strcat(aux, itoa(nroPos, aux2, 10));
			apilar(&whileEtiq, aux);
			fprintf(af,"%s:\n", aux);
			bandWhile = 1;
		}

		if(strcmp(cadena1, "BI") == 0){
			desapilar(&whileEtiq, aux);
			fprintf(af,"JMP %s\n", aux);
			fgets(cadena1, 200, pf);
			nroPos++;
		}
		
		if(esOperadorBinario(cadena1)){
			if(strcmp(cadena1, "+") == 0){
				desapilar(&pila, aux2); // SACO EL +
				desapilar(&pila, aux2); // SACO EL PRIMER OPERANDO
				strcpy(aux, "FLD ");
				strcat(aux, aux2);
				desapilar(&pila, aux2);

				strcpy(aux3, "FLD ");
				strcat(aux3, aux2);
				if(strcmp(aux, aux3) == 0){
								strcpy(aux3, "FLD ");
								strcat(aux3, "_0_esddfloat");
				}

				fprintf(af,"%s\n",aux);
				fprintf(af,"%s\n",aux3);
				fprintf(af,"FADD\n");
				fprintf(af, "FSTP %s\n",varAux);
				apilar(&pila, varAux);
				i++;
			}
			if(strcmp(cadena1, "-") == 0){
				desapilar(&pila, aux2);
				desapilar(&pila, aux2);
				strcpy(aux3, "FLD ");
				strcat(aux3, aux2);
				desapilar(&pila, aux2);
				strcpy(aux, "FLD ");
				strcat(aux, aux2);
				if(strcmp(aux, aux3) == 0){
					strcpy(aux3, "FLD ");
					strcat(aux3, "_0_esddfloat");
				}
				fprintf(af,"%s\n",aux);
				fprintf(af,"%s\n",aux3);
				fprintf(af,"FSUB\n");
				fprintf(af, "FSTP %s\n",varAux);
				apilar(&pila, varAux);
				i++;
			}
			if(strcmp(cadena1, "*") == 0){
				desapilar(&pila, aux2);
				desapilar(&pila, aux2);
				strcpy(aux3, "FLD ");
				strcat(aux3, aux2);
				desapilar(&pila, aux2);
				strcpy(aux, "FLD ");
				strcat(aux, aux2);
				if(strcmp(aux, aux3) == 0){
					strcpy(aux3, "FLD ");
					strcat(aux3, "_1_esddfloat");
				}
				fprintf(af,"%s\n",aux);
				fprintf(af,"%s\n",aux3);
				fprintf(af,"FMUL \n");
				fprintf(af, "FSTP %s\n",varAux);
				apilar(&pila, varAux);
				i++;
			}
			if(strcmp(cadena1, "/") == 0){
				desapilar(&pila, aux2);
				desapilar(&pila, aux2);
				strcpy(aux3, "FLD ");
				strcat(aux3, aux2);
				desapilar(&pila, aux2);
				strcpy(aux, "FLD ");
				strcat(aux, aux2);
				if(strcmp(aux, aux3) == 0){
					strcpy(aux3, "FLD ");
					strcat(aux3, "_1_esddfloat");
				}
				fprintf(af,"%s\n",aux);
				fprintf(af,"%s\n",aux3);
				fprintf(af,"FDIV \n");
				fprintf(af, "FSTP %s\n",varAux);
				apilar(&pila, varAux);
				i++;
			}

			if(strcmp(cadena1, ":") == 0){
				desapilar(&pila, aux2); // desapilo el :
				desapilar(&pila, aux2); // desapilo el valor a asignar
				
				fgets(cadena1, 200, pf); // leo valor al que le asigno
        		nroPos++;
				
					//printf("ESTOY EN ASIGNACION, AUX TIENE %s\n", aux2);
					strcpy(aux, "FLD ");
					strcat(aux, aux2);
        			fprintf(af,"%s\n",aux);

					strcpy(aux, "FSTP ");
					strcat(aux, cadena1);
					fprintf(af,"%s",aux);
				
			}
			if(strcmp(cadena1, "CMP") == 0){  // DUDA CON ESTO PARA SABER COMO ASIGNAR SIMBOLOS EN COMPARACIONES
				
				fgets(operador, 200, pf); nroPos++;  // VIENE EL PRIMER BLE, BEQ, ETC...
				fgets(posSalto, 200, pf); nroPos++;  // GUARDO POSICION DEL SALTO



				fSize = ftell(pf);
				fgets(cadena2, 200, pf); //ME FIJO CON CADENA2 SI HAY CONDICION COMPUESTA
				fseek(pf,  fSize, SEEK_SET);

				equivalenteCondicion(operador); // PASO EL OPERADOR DE LA CONDICION A ASSEMBLER

				//ASIGNO ETIQUETA AL PRIMER SALTO
				strcpy(primerSalto, "_Etiq");
				strcat(primerSalto, posSalto);

				desapilar(&pila, primerOperando); // DESAPILO EL CMP
				desapilar(&pila, primerOperando); // DESAPILO EL PRIMER OPERANDO
				desapilar(&pila, segundoOperando); // DESAPILO EL SEGUNDO OPERANDO 

				//ESCRIBO EN ARCHIVO ASSEMBLER LA PRIMER CONDICION

				fprintf(af,"FLD %s\n",primerOperando);
				fprintf(af,"FLD %s\n",segundoOperando);

				if(strstr(primerOperando, "_@max") != NULL || strstr(primerOperando, "_@aux") != NULL || 
					strstr(segundoOperando, "_@max") != NULL || strstr(segundoOperando, "_@aux") != NULL){
				

						fprintf(af,"FXCH\n");
					
					}
				fprintf(af,"FCOM\n");
				fprintf(af,"FSTSW AX\n");
				fprintf(af,"SAHF\n");

				



				if(strstr(cadena2, "_TOKENOR_") != NULL){

					fprintf(af,"%s  %s",operador, primerSalto); // AGREGO PRIMER SALTO


					fgets(cadena2, 200, pf); nroPos++; //SALTO EL _TOKENOR_

					fgets(primerOperando, 200, pf);   nroPos++;// LEO EL PRIMER OPERANDO DE LA SEGUNDA CONDICION
					fgets(segundoOperando, 200, pf);  nroPos++;// LEO EL SEGUNDO OPERANDO DE LA SEGUNDA CONDICION
					fgets(operador, 200, pf);  nroPos++;// SALTO EL CMP
					fgets(operador, 200, pf);  nroPos++;// LEO EL OPERADOR DE LA SEGUNDA CONDICION
					fgets(posSalto, 200, pf);  nroPos++;// GUARDO POSICION DEL SALTO
					

					//ASIGNO ETIQUETA AL PRIMER SALTO
					strcpy(segundoSalto, "_Etiq");
					strcat(segundoSalto, posSalto);


					fprintf(af,"FLD %s",primerOperando);
					fprintf(af,"FLD %s",segundoOperando);

					fprintf(af,"FCOM\n");
					fprintf(af,"FSTSW AX\n");
					fprintf(af,"SAHF\n");
					equivalenteCondicion(operador);


					//le borro el \n al primer salto
					prueba = str_replace("\n", "", primerSalto);
					strcpy(primerSalto, prueba);

					fprintf(af,"%s  %s",operador, segundoSalto); // AGREGO SEGUNDO SALTO		
					fprintf(af,"%s:\n", primerSalto);
					//printf("%s\n",segundoSalto);
					apilar(&etiquetas, segundoSalto);
					apilarEntero(&salto, atoi(posSalto));


				}else{ if(strstr(cadena2, "_TOKENAND_") != NULL){

					fprintf(af,"%s  %s",operador, primerSalto); // AGREGO PRIMER SALTO


					fgets(cadena2, 200, pf);  nroPos++; //SALTEO EL _TOKENAND_

					fgets(primerOperando, 200, pf);  nroPos++; // LEO EL PRIMER OPERANDO DE LA SEGUNDA CONDICION
					fgets(segundoOperando, 200, pf); nroPos++; // LEO EL SEGUNDO OPERANDO DE LA SEGUNDA CONDICION
					fgets(operador, 200, pf);  nroPos++;// SALTO EL CMP
					fgets(operador, 200, pf);  nroPos++; // LEO EL OPERADOR DE LA SEGUNDA CONDICION
					fgets(posSalto, 200, pf);  nroPos++;// GUARDO POSICION DEL SALTO

					//ASIGNO ETIQUETA AL PRIMER SALTO

					strcpy(segundoSalto, "_Etiq");
					strcat(segundoSalto, posSalto);


					fprintf(af,"FLD %s",primerOperando);
					fprintf(af,"FLD %s",segundoOperando);
					if(bandWhile != 1){
						fprintf(af,"FXCH\n");
					}else{
						bandWhile = 0;
					}
					fprintf(af,"FCOM\n");
					fprintf(af,"FSTSW AX\n");
					fprintf(af,"SAHF\n");
					
					equivalenteCondicion(operador);
					fprintf(af,"%s  %s",operador, segundoSalto); // AGREGO SEGUNDO SALTO

					//LE AGREGO DOS PUNTOS AL FINAL DE AL ETIQUETA PARA ALMACENAR.	
					apilar(&etiquetas, segundoSalto);
					apilarEntero(&salto, atoi(posSalto));
	

					}else{
						fprintf(af,"%s  %s",operador, primerSalto); // AGREGO PRIMER SALTO
						apilar(&etiquetas, primerSalto);
						apilarEntero(&salto, atoi(posSalto));
					}
				}
				


			}


		}
		if(esOperadorUnario(cadena1)){
			if(strcmp(cadena1, "PUT") == 0){
				fgets(cadena1, 200, pf);   //LEO DE LA POLACA LO QUE PRINTEO
        		nroPos++;
        	//	fprintf(af,"displayString %s",cadena1);  // solo sirve para cuando vienen strings.
        		prueba = str_replace("\n", "", cadena1);
        		strcpy(cadena1, prueba);
        		if(strstr(cadena1, "_esddfloat") != NULL){
        			fprintf(af,"DisplayFloat %s , 2\n",cadena1);
        			fprintf(af,"newline 1\n");
        		}else{
        			fprintf(af,"displayString %s \n",cadena1);
        			fprintf(af,"newline 1\n");
        		}
			}
			if(strcmp(cadena1, "GET") == 0){
				fgets(cadena1, 200, pf);   //LEO DE LA POLACA DONDE GUARDO
        		nroPos++;
        		fprintf(af,"getFloat %s",cadena1); 

			}



		}


		if( verTopeEntero(&salto) == nroPos ){
			if(cierraComp != -1 ){
				desapilar(&etiquetas, aux);
				prueba = str_replace("\n", ":", aux);
				strcpy(aux, prueba);

				fprintf(af,"%s\n", aux);
				apilar(&etiquetas, aux);
			}

		}
	}



	
	fprintf(af,"MOV EAX, 4c00h  ; termina la ejecucion\n");
	fprintf(af,"INT 21h\n");
	fprintf(af,"END start;\n;\n");

	fclose(pf);
	fclose(af);
	fclose(qf);
	return 1;
}


int esOperadorBinario(char *d){
	if(strcmp(d,"+") == 0 || strcmp(d,"-") == 0 || strcmp(d,"/") == 0 || strcmp(d,"*") == 0 || strcmp(d,"BGE") == 0 || strcmp(d,"BGT") == 0 
			|| strcmp(d,"BLE") == 0 || strcmp(d,"BLT") == 0 || strcmp(d,"BNE") == 0 || strcmp(d,"BEQ") == 0 || strcmp(d,":") == 0 || strcmp(d,"CMP") == 0 ){

		return 1;
	}

	return 0;
}


int esOperadorUnario(char *d){
	if(strcmp(d, "PUT") == 0 || strcmp(d, "GET") == 0){
		return 1;
	}

	return 0;
}


char* str_replace(char* search, char* replace, char* subject) {
	int i, j, k;
	
	int searchSize = strlen(search);
	int replaceSize = strlen(replace);
	int size = strlen(subject);

	char* ret;

	if (!searchSize) {
		ret = malloc(size + 1);
		for (i = 0; i <= size; i++) {
			ret[i] = subject[i];
		}
		return ret;
	}
	
	int retAllocSize = (strlen(subject) + 1) * 2; 
	ret = malloc(retAllocSize);

	int bufferSize = 0; 
	char* foundBuffer = malloc(searchSize); 
	
	for (i = 0, j = 0; i <= size; i++) {
		
		if (retAllocSize <= j + replaceSize) {
			retAllocSize *= 2;
			ret = (char*) realloc(ret, retAllocSize);
		}
		
		else if (subject[i] == search[bufferSize]) {
			foundBuffer[bufferSize] = subject[i];
			bufferSize++;

			if (bufferSize == searchSize) {
				bufferSize = 0;
				for (k = 0; k < replaceSize; k++) {
					ret[j++] = replace[k];
				}
			}
		}
		else {
			for (k = 0; k < bufferSize; k++) {
				ret[j++] = foundBuffer[k];
			}
			bufferSize = 0;
			
			ret[j++] = subject[i];
		}
	}
	free(foundBuffer);
	
	return ret;
}


void invertirSimbolo2(char* aux){
    
    if(strstr(aux, "BEQ") != NULL){
        strcpy(aux, "BNE");
    }else if(strstr(aux, "BNE") != NULL){
        strcpy(aux, "BEQ");
    } else if(strstr(aux, "BLT") != NULL){
        strcpy(aux, "BGE");
    }else  if(strstr(aux, "BLE") != NULL){
        strcpy(aux, "BGT");
    }else   if(strstr(aux, "BGT") != NULL){
        strcpy(aux, "BLE");
    }else{
        strcpy(aux, "BLT");
    }
}


void equivalenteCondicion(char* aux){
	if(strstr(aux, "BEQ") != NULL){
		strcpy(aux, "JE");
	}else if(strstr(aux, "BNE") != NULL){
        strcpy(aux, "JNE");
    } else if(strstr(aux, "BLT") != NULL){
        strcpy(aux, "JB");
    }else  if(strstr(aux, "BLE") != NULL){
        strcpy(aux, "JNA");
    }else   if(strstr(aux, "BGT") != NULL){
        strcpy(aux, "JA");
    }else{
        strcpy(aux, "JAE");
    }
}




void invertirCondicion(char* aux){
	if(strstr(aux, "JNE") != NULL){
		strcpy(aux, "JE");
	}else if(strstr(aux, "JE") != NULL){
        strcpy(aux, "JNE");
    } else if(strstr(aux, "JNA") != NULL){
        strcpy(aux, "JB");
    }else  if(strstr(aux, "JB") != NULL){
        strcpy(aux, "JNA");
    }else   if(strstr(aux, "JAE") != NULL){
        strcpy(aux, "JA");
    }else{
        strcpy(aux, "JAE");
    }
}
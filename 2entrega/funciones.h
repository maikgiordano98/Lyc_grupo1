#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CON_VALOR 1
#define SIN_VALOR 0
#define ES_STRING 2
#define ES_CONSTANTE 3
#define NO_ES_CONSTANTE 5



typedef struct tupla{
	char* lexema;
	char* valor;
	char* tipo;
	char constante[3];
	char longitud[30];
	struct tupla* siguiente;

}tuplaTabla;


typedef  tuplaTabla* tabla;




int insertar(char* , int ,tabla*  , int ,  char*);

int enlistar_en_orden(tabla* ,tuplaTabla* );

int vaciar_lista_TS(tabla* l);

void crearTabla(tabla* lista);

void eliminarCaracter(char *str, char garbage);

int mi_strlen(char* cadena);



void crearTabla(tabla* lista){
	*lista = NULL;

}



int vaciar_lista_TS(tabla* l)
{
    tuplaTabla* viejo;
    FILE* pf = fopen("ts.txt","w+");
    if(!pf){
    	printf("No se pudo abrir el archivo;\n");
    	return 0;
    }

   	fprintf(pf,"%s","LEXEMA\t\t\t\tVALOR\t\t\t\tCONSTANTE\t\t\tLONGITUD\t\t\tTIPO\n");

    while(*l)
    {
        viejo=*l;
        *l=viejo->siguiente;
        fprintf(pf,"%s\t\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t%s\n", viejo->lexema, viejo->valor, viejo->constante, viejo->longitud, viejo->tipo);

        free(viejo);
    }
    fclose(pf);
    return 1;

}


int insertar(char* lexemaE, int valor,tabla*  tablaSimbolos, int constante, char* tipo2){

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

			eliminarCaracter(lexemaE, '"');
			strcpy(nuevo->lexema, "_");
			strcat(nuevo->lexema, lexemaE);
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

		
		nuevo->valor = NULL;
		if(constante != ES_CONSTANTE)
				strcpy(nuevo->constante,"NO");


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

int mi_strlen(char* cadena){

	int i = 0;
	while(*cadena){
		i++;
		cadena++;
	}

	return ( i <= 30 )? 1:0;
}


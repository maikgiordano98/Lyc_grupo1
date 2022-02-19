#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct f_nodo
{
    struct f_nodo* ant;
    char dato[20];
    int nro;
}r_nodo;

typedef r_nodo* t_pila;


////////// PILA //////////
void crear_pila(t_pila*);
int apilar(t_pila*,char*);
int desapilar(t_pila*,char*);
int pila_vacia(t_pila*);
void vaciar_pila(t_pila*);
int apilarTODO(t_pila*, char*, int);
//////////////////////////


void crear_pila(t_pila* p)
{
    *p=NULL;
}

int apilar(t_pila* p,char* d)
{

    r_nodo* nuevo=(r_nodo*)malloc(sizeof(r_nodo));

    if(!nuevo){
    	printf("No se pudo reservar memoria\n");
        return 0;
    }
    strcpy(nuevo->dato,d);
    nuevo->ant=*p;
    *p=nuevo;

    return 1;
}


int apilarTODO(t_pila* p, char* d, int nro){

 r_nodo* nuevo=(r_nodo*)malloc(sizeof(r_nodo));

    if(!nuevo){
        printf("No se pudo reservar memoria\n");
        return 0;
    }
    strcpy(nuevo->dato,d);
    nuevo->nro = nro;
    nuevo->ant=*p;
    *p=nuevo;

    return 1;


}

int apilarEntero(t_pila* p,int d)
{

    r_nodo* nuevo=(r_nodo*)malloc(sizeof(r_nodo));
    if(!nuevo){
        printf("No se pudo reservar memoria\n");
        return 0;
    }
    nuevo->nro = d;
    nuevo->ant=*p;
    *p=nuevo;

    return 1;
}

int desapilar(t_pila* p,char* d)
{
    r_nodo* viejo;
    if(!*p)
        return 0;
    viejo=(r_nodo*)malloc(sizeof(r_nodo));
    viejo=*p;
    strcpy(d,viejo->dato);
    *p=viejo->ant;
    free(viejo);
    return 1;
}

int desapilarEntero(t_pila* p)
{
    r_nodo* viejo;
    int retorno = 0;
    if(!*p)
        return 0;
    viejo=(r_nodo*)malloc(sizeof(r_nodo));
    viejo=*p;
    retorno = viejo->nro;
    *p=viejo->ant;
    free(viejo);
    return retorno;
}

int pila_vacia(t_pila* p)
{
    return !*p;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/////////////////////////
typedef struct s_nodo
{
    struct s_nodo* sig;
    char dato[20];
}t_nodo;
////////////////////////
typedef struct
{
    t_nodo* primero;
    t_nodo* ultimo;
}t_cola;
////////// COLA ////////////
void crear_cola(t_cola*);
void vaciar_cola(t_cola*);
int acolar(t_cola*,char*);
int desacolar(t_cola*,char*);
int cola_vacia(t_cola*);
int cola_llena(t_cola*);
///////////////////////////

void crear_cola(t_cola* c)
{
    c->primero=NULL;
    c->ultimo=NULL;
}

void vaciar_cola(t_cola* c)
{
    t_nodo* viejo;
    while(!c->primero)
    {
        viejo=c->primero;
        c->primero=viejo->sig;
        free(viejo);
    }
}

int acolar(t_cola* c,char* d)
{
    t_nodo* nuevo=(t_nodo*)malloc(sizeof(t_nodo));
    if(!nuevo){
        printf("No se pudo reservar memoria\n");
        return 0;
    }   
    strcpy(nuevo->dato,d);
    nuevo->sig=NULL;
    if(!c->primero)
    {
        c->primero=nuevo;
        c->ultimo=nuevo;
        return 1;
    }
        c->ultimo->sig=nuevo;
        c->ultimo=nuevo;
    return 1;
}

int desacolar(t_cola* c,char* d)
{
    t_nodo* viejo;
    if(!c->primero)
        return 0;
    viejo=c->primero;
    strcpy(d,viejo->dato);
    c->primero=viejo->sig;
    free(viejo);
    return 1;
}

int cola_vacia(t_cola* c)
{
    return !c->primero;
}

int cola_llena(t_cola* c)
{
    t_nodo* nuevo=(t_nodo*)malloc(sizeof(t_nodo));
    free(nuevo);
    return !nuevo;
}
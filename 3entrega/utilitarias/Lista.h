#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct l_nodo
{
    struct l_nodo* sig;
    int nroPolaca;
    char elemento[200];

}l_nodo;
typedef l_nodo* t_lista;


void crear_lista(t_lista*);
int lista_vacia(t_lista* );
int enlistar(t_lista* ,char*, int );
int desenlistar(t_lista *,char *);
int lista_llena(t_lista* );
int vaciar_lista_INTERMEDIO(t_lista*, int );




void crear_lista(t_lista* l)
{
    *l=NULL;
}

int lista_vacia(t_lista* l)
{
    return !*l;
}

int enlistar(t_lista* l,char* d, int posicion)
{
    t_lista* nodoaux = l;
    l_nodo* nuevo = (l_nodo*)malloc(sizeof(l_nodo));
    if(!nuevo){
        printf("NO RESERVO MEMORIA\n");
        return 0;
    }

    nuevo->sig=NULL;
    strcpy(nuevo->elemento,d);
    nuevo->nroPolaca = posicion;
    if(!*l){
        *l = nuevo;
    }else{
        while((*nodoaux)->sig != NULL){
            nodoaux = &(*nodoaux)->sig;
        }
        (*nodoaux)->sig = nuevo;
    }
    return 1;
}

int desenlistar(t_lista *l,char *d)
{
    if(!*l)
        return 0;
    l_nodo* viejo;
    viejo=*l;
    strcpy(d,viejo->elemento);
    *l=viejo->sig;
    free(viejo);
    return 1;
}

int lista_llena(t_lista* l)
{
    l_nodo* aux=(l_nodo*)malloc(sizeof(l_nodo));
    return !aux;
}

int vaciar_lista_INTERMEDIO(t_lista* l, int posiciones)
{
    l_nodo* aux;
    int i = 1;
    FILE* pf = fopen("intermedia.txt","w+");
    FILE* auxpf = fopen("auxiliar.txt","w+");
    if(!pf){
        printf("No se pudo abrir el archivo;\n");
        return 0;
    }
    if(!auxpf){
        printf("No se pudo abrir el archivo;\n");
        return 0;
    }

    while(*l)
    {
        aux=*l;
        *l=aux->sig;
        fprintf(pf,"%d\t%s\n",i, aux->elemento);
        fprintf(auxpf,"%s\n", aux->elemento);
        i++;
        free(aux);
    }
    fclose(pf);
    fclose(auxpf);
    return 1;
}


void rellenarPolaca(t_lista *l, int posicion, int valor){

    t_lista* aux = l;
    char cadena[5];
    while(*aux && (((*aux)->nroPolaca-posicion) != 0)  ){
        aux = &(*aux)->sig;
    }
    if(!*aux){
        printf("LISTA VACIA!\n");
    }
    if(((*aux)->nroPolaca-posicion) == 0){
        itoa(valor, cadena, 10);
        strcpy((*aux)->elemento, cadena );
    }
}


void rellenarPolacaChar(t_lista *l, int posicion, char* valor){

    t_lista* aux = l;
    while(*aux && (((*aux)->nroPolaca-posicion) != 0)  ){
        aux = &(*aux)->sig;
    }
    if(!*aux){
        printf("LISTA VACIA!\n");
    }
    if(((*aux)->nroPolaca-posicion) == 0){
        strcpy((*aux)->elemento, valor );
    }
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "y.tab.h"

FILE *yyin;



void tokenInvalido(char *s);
int errorLexico(void);

%}

%option noyywrap
%option yylineno

/* patrones */
DIGITO [0-9]
LETRA [[:alpha:]]

/* constantes */
CONST_INT 			{DIGITO}+
CONST_R 		({DIGITO}+\.{DIGITO}*)|({DIGITO}*\.{DIGITO}+)
CONST_STR		\"(\\.|[^"\\])*\"

/* flow control */
IF		"if"|"IF"|"If"
ELSE	"else"|"ELSE"|"Else"
WHILE	"while"|"WHILE"

/* operadores logicos */
NEQ		"!="
EQ		"=="
GEQ		">="
GT		">"
LT		"<"
LEQ		"<="
NOT		"!"
AND		"&&"
OR		"||"

/* llaves, parentesis y corchetes */
LL_CRR	"}"
LL_ABR	"{"
PR_CRR	")"
PR_ABR	"("
CR_CRR	"]"
CR_ABR	"["

/* operadores aritmeticos + - / * */
DIV	 	"/"
SUM	 	"+"
MIN	 	"-"
MULT 	"*"

/* asignacion */
ASIGN "="

/* declaracion */
VAR				"var"
AS				"as"|"AS"
COMA			","

/* fin de instruccion */
END_STMT			";"


/* tipos de dato */
INT				"int"
REAL			"real"
STRING_T	"string"

/* IO */
DISPLAY			"DISPLAY"
GET				"get"

COMENTARIO \*\/.*\/\*

/* identificador comienza con letra, contiene letras y digitos */
ID {LETRA}({LETRA}|{DIGITO})*

/* Temas especiales */
/* Constantes con Nombre */

CONST_NOMBRE "const"|"CONST"


%%
{CONST_INT}			{if (atoi (yytext) < -32768 || atoi (yytext) > 32767){tokenInvalido(yytext);}else{yylval.strVal = strdup(yytext); return CONST_INT;} }
{CONST_R}		{if (atof (yytext) < -3.4E+38 || atof (yytext) > 3.4E+38){tokenInvalido(yytext);}else{yylval.strVal = strdup(yytext); return CONST_R;} }
{CONST_STR}		{if(strlen(yytext)<=32){yylval.strVal = strdup(yytext); return CONST_STR;}
						else{printf("\nCADENA MAYOR A 30 CARACTERES EN LA LINEA %d\n",yylineno); errorLexico();}}
{ELSE}			{return ELSE;}
{IF}				{return IF;}
{NEQ}		{return NEQ;}
{EQ}			{return EQ;}
{GEQ}		{return GEQ;}
{GT}			{return GT;}
{LT}			{return LT;}
{LEQ}		{return LEQ;}
{LL_CRR}			{return LL_CRR;}
{LL_ABR}			{return LL_ABR;}
{PR_CRR}			{return PR_CRR;}
{PR_ABR}			{return PR_ABR;}
{CR_CRR}			{return CR_CRR;}
{CR_ABR}			{return CR_ABR;}
{DIV}		{return DIV;}
{SUM}			{return SUM;}
{MIN}			{return MIN;}
{ASIGN}				{return ASIGN;}
{MULT}			{return MULT;}
{WHILE}			{return WHILE;}
{END_STMT}			{return END_STMT;}
{COMA}		{return COMA;}
{REAL}			{return REAL;}
{INT}			{return INT;}
{STRING_T}			{return STRING_T;}
{VAR}				{return VAR;}
{AS}				{return AS;}
{DISPLAY}			{return DISPLAY;}
{GET}				{return GET;}
{CONST_NOMBRE}			{return CONST_NOMBRE;}
{AND}			{return AND;}
{OR}			{return OR;}
{NOT}			{return NOT;}
{COMENTARIO}
{ID}				{yylval.strVal = strdup(yytext); return ID;}
[.]					{tokenInvalido(yytext);}
"\t"
"\n"
" "
%%


int errorLexico(void){
printf("Error lexico\n");
	system("Pause");
	exit(1);
}

void tokenInvalido(char *s){
	printf("Error en la linea %d, token invalido %s .\n", yylineno, s);
}
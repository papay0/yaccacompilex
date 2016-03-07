%{

#include "compiler.h"
#include "y.tab.h"
#include <stdio.h>
int getMode();
#define p(x, s) if(getMode() == 0) printf(x, s)

%}
tNumber 	(\+|\-)?([0-9]+)(e(\+|\-)[0-9]+)*
tID 		([a-zA-Z_]+[0-9a-zA-Z_]*)
tINT 		"int"
tCHAR		"char"
tPrint 		"print"
tIf 		"if"
tWhile 		"while"
tReturn 	"return"
tSemi 		";"
tComa 		","
tAffect 	"="
tEquals  	"=="
tPlus 		"+"
tMinus 		"-"
tMult 		"*"
tDiv 		"/"
tPO 		"("
tPC 		")"
tAO 		"{"
tAC 		"}"
tAnd		"&&"
tAmpersand	"&"
tOr			"||"
tNot		"!"
tNotEquals 	"!="
%%


{tINT} 			{ p("INT{%s} ", yytext); 	return tINT; };
{tCHAR}			{ p("CHAR{%s} ", yytext); 	return tCHAR; };
{tPrint} 		{ p("Print{%s} ", yytext); 	return tPrint; };
{tIf} 			{ p("If{%s} ", yytext); 	return tIf; };
{tWhile} 		{ p("While{%s} ", yytext); 	return tWhile; };
{tReturn} 		{ p("Return{%s} ", yytext); return tReturn; };
{tID} 			{ 
	// TODO gérer les fuites mémoires
	char* newstr = malloc(sizeof(strlen(yytext)+1));	
	strcpy(newstr, yytext);
	p("ID{%s} ", yytext); 	yylval.string = newstr;
	return tID; 
};
{tSemi} 		{ p("Semi{%s} ", yytext); 	return tSemi; };
{tComa} 		{ p("Coma{%s} ", yytext); 	return tComa; };
{tEquals}		{ p("Equals{%s} ", yytext);	return tEquals; };
{tNotEquals} 	{ p("Not Equals{%s} ", yytext);	return tNotEquals; };
{tAffect} 		{ p("Affect{%s} ", yytext); 	return tAffect; };
{tNot}			{ p("Not{%s} ", yytext);	return tNot; };
{tPlus} 		{ p("Plus{%s} ", yytext); 	return tPlus; };
{tMinus} 		{ p("Minus{%s} ", yytext); 	return tMinus;};
{tNumber} {
	yylval.number = atoi(yytext);
	p("Number{%s} ", yytext);
	return tNumber;
};
{tMult} 		{ p("Mult{%s} ", yytext); 	return tMult; };
{tDiv} 			{ p("Div{%s} ", yytext); 	return tDiv; };
{tPO} 			{ p("PO{%s} ", yytext); 	return tPO; };
{tPC} 			{ p("PC{%s} ", yytext); 	return tPC; };
{tAO} 			{ p("AO{%s} ", yytext); 	return tAO; };
{tAC} 			{ p("AC{%s} ", yytext); 	return tAC; };
{tAnd}			{ p("And{%s} ", yytext);	return tAnd;};
{tAmpersand}	{ p("Ampersand{%s}", yytext); return tAmpersand; };
{tOr}			{ p("Or{%s} ", yytext);		return tOr;};

. ;
%%

%{

#include "compiler.h"
#include "y.tab.h"
#include <stdio.h>
int getMode();
#define p(x, s) if(getMode() == 0) printf(x, s)

%}
tComment 	"//".*
tCommentS	"/*"
tNumber 	(\+|\-)?([0-9]+)(e(\+|\-)[0-9]+)*
tID 		([a-zA-Z_]+[0-9a-zA-Z_]*)
tINT 		"int"
tCHAR		"char"
tPrint 		"print"
tIf 		"if"
tElse		"else"
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
tCO			"["
tCC 		"]"
tAnd		"&&"
tAmpersand	"&"
tOr			"||"
tNot		"!"
tNotEquals 	"!="
%%


{tCommentS}     { consume_comment(); }
{tComment}		{  }
{tINT} 			{ p("INT{%s} ", yytext); 	return tINT; };
{tCHAR}			{ p("CHAR{%s} ", yytext); 	return tCHAR; };
{tPrint} 		{ p("Print{%s} ", yytext); 	return tPrint; };
{tIf} 			{ p("If{%s} ", yytext); 	return tIf; };
{tElse} 			{ p("Else{%s} ", yytext); 	return tElse; };
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
{tCO} 			{ p("CO{%s} ", yytext); 	return tCO; };
{tCC} 			{ p("CC{%s} ", yytext); 	return tCC; };
{tAnd}			{ p("And{%s} ", yytext);	return tAnd;};
{tAmpersand}	{ p("Ampersand{%s}", yytext); return tAmpersand; };
{tOr}			{ p("Or{%s} ", yytext);		return tOr;};

. ;
%%

void consume_comment()
{
    int c, prev;

    while ((c = input()) != 0)
	{
		// fin du commentaire
		if(c == '/' && prev == '*') return;
		// incrémentation du numéro de ligne si on le mange.
		if (c == '\n') yylineno++;
		// fin du fichier => le commentaire n'est pas fermé.
		if (c == 0) break;
		prev = c;
    }
    yyerror("erreur: commentaire non fini.");
}

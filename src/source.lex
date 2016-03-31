%{

#include "compiler.h"
#include "y.tab.h"
#include <stdio.h>
int getMode();
#define p(x, s) if(getMode() == 0) printf(x, s)

%}
tComment 	"//".*
tCommentS	"/*"
tNumber 	(\-)?([0-9]+)(e(\+|\-)[0-9]+)*
tID 		([a-zA-Z_]+[0-9a-zA-Z_]*)
tINT 		"int"
tCHAR		"char"
tVOID       "void"
tPrint 		"print"
tIf 		"if"
tElse		"else"
tElseif "else if"
tWhile 		"while"
tReturn 	"return"
tMalloc     "malloc"
tFree       "free"
tAssert     "assert"
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
tBO         "<"
tBC         ">"
tAnd		"&&"
tAmpersand	"&"
tOr			"||"
tNot		"!"
tInf    "<"
tSup    ">"
tNotEquals 	"!="
%%


{tCommentS}     { consume_comment(); }
{tComment}		{  }
{tINT} 			{ p("INT{%s} ", yytext); 	return tINT; };
{tCHAR}			{ p("CHAR{%s} ", yytext); 	return tCHAR; };
{tVOID}			{ p("VOID{%s} ", yytext); 	return tVOID; };
{tPrint} 		{ p("Print{%s} ", yytext); 	return tPrint; };
{tIf} 			{ p("If{%s} ", yytext); 	return tIf; };
{tElse} 		{ p("Else{%s} ", yytext); 	return tElse; };
{tElseif}       { p("Else if{%s} ", yytext); 	return tElseif; };
{tWhile} 		{ p("While{%s} ", yytext); 	return tWhile; };
{tReturn} 		{ p("Return{%s} ", yytext); return tReturn; };
{tAssert}       { p("Assert{%s} ", yytext); return tAssert; };
{tMalloc}       { p("Malloc{%s} ", yytext); return tMalloc; };
{tFree}         { p("Free{%s} ", yytext); return tFree; };

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
{tInf}      { p("Inf{%s} ", yytext);	return tInf; };
{tSup}      { p("Sup{%s} ", yytext);	return tSup; };
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
{tBO} 			{ p("BO{%s} ", yytext); 	return tBO; };
{tBC} 			{ p("BC{%s} ", yytext); 	return tBC; };
{tAnd}			{ p("And{%s} ", yytext);	return tAnd;};
{tAmpersand}	{ p("Ampersand{%s}", yytext); return tAmpersand; };
{tOr}			{ p("Or{%s} ", yytext);		return tOr;};
\n 				{ yylineno++; }
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

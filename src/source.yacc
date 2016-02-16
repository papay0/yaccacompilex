%token tINT 
%token tAnd tOr tEquals tNotEquals tNot
%token tPrint tIf tWhile tReturn 
%token tSemi tComa tAffect tPlus tMinus tMult tDiv
%token tPO tPC tAO tAC
%token <number> tNumber 
%token <string> tID

%type  <number> Expr
%type  <number> Affect
%type  <number> FuncCallExpr
%error-verbose

%right tAffect
%left  tAnd tOr
%left  tEquals tNotEquals
%left  tPlus tMinus
%left  tMult tDiv
 
%start Input 

%union 
{
        int number;
        char *string;
}

%{
	#include <stdio.h>
	#include "compiler.h"
	void yyerror(char const * errorText);
%}

%%

Input 		: 	FuncDecl Input
			| FuncDecl ;

FuncDecl 	:	Type tID tPO TypedParams tPC Body;

Type 		:   	tINT;	

Body 		: 	tAO InstList tAC
			| tAO tAC;

InstList	: 	Inst InstList
			| Inst;

Inst		:  	IVarDecl 
			| IVarDeclAff 
			| IVarAff
			| IFuncCall
			| If 	
			| While		
			| Return
			| Print;

IFuncCall	: 	FuncCallExpr tSemi;

IVarDeclAff	: 	Type IDList tAffect Expr tSemi;

IVarDecl	: 	Type tID tSemi {
	stable_add(symbols, $2, 0, ctx.depth, sizeof(int));
};

IVarAff 	: 	Affect tSemi;


Cond 		: 	Expr ;
If		: 	tIf tPO Cond tPC Body;
While		: 	tWhile tPO Cond tPC Body;
Return		: 	tReturn Expr tSemi;
Print		: 	tPrint tPO Expr tPC tSemi;
Affect		: 	tID tAffect Expr { $$ = $3; };
Expr 		:	Affect 
			| Expr tEquals Expr
			| Expr tNotEquals Expr
			| Expr tAnd Expr 
			| Expr tOr Expr 
			| Expr tPlus Expr 
			| Expr tMinus Expr 
			| Expr tMult Expr 
			| Expr tDiv Expr 
			| FuncCallExpr
			| tNumber 
			| tID 
			;
FuncCallExpr	: 	tID tPO Params tPC { $$ = 0; };
TypedParams 	:	TypedParam STypedParams
			| ;
STypedParams	:	tComa TypedParam STypedParams 
			| tComa TypedParam;

TypedParam	:	Type tID;

Params 		: 	Expr SParams
			| ;

SParams 	: 	tComa Expr SParams
			| tComa Expr;

IDList 		: 	tID SParams
			| ;

IDList  	: 	tComa tID SParams
			| tComa tID;
%%

void yyerror(char const * errorText) { }
int getMode();

int main(int argc, char** argv) 
{
	ctx_init();
	if(getMode() == 0)
		while(1) { yylex(); }

	if(getMode() == 1)
		while(1)
		{ 
			yyparse();
			stable_print(symbols);
			printf("New parse...\n");
		}

	return 0;
}

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
%type  <type> Type
%type  <type> VarDeclType
%error-verbose

%right tAffect
%left  tAnd tOr
%left  tEquals tNotEquals
%left  tPlus tMinus
%left  tMult tDiv
 
%start Input 

%union 
{	
	int type;
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

IVarDecl	: 	VarDeclType IDList tSemi { 
	for(int i = 0; i < idbuffer_size(); i++)
	{
		// TODO : 0xDODO => size ou type
		printf("stable_add %s %p\n", idbuffer_get(i), idbuffer_get(i));
		stable_add(symbols, idbuffer_get(i), -1, ctx.depth, 0xD0D0);
	}
};

IDList 		: 	VarDeclID SIDList 
			| VarDeclID;

SIDList  	: 	tComa VarDeclID SIDList
			| tComa VarDeclID 
			;

VarDeclID	:	tID {
	idbuffer_add($1);
};

VarDeclType	:	tINT {
	idbuffer_init();
	idbuffer_settype(0);
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
			| tID { $$ = 0; } 
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

Type 		:   	tINT { $$ = 0; };	
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

%token tINT 
%token tAnd tOr tEquals tNotEquals tNot
%token tPrint tIf tWhile tReturn 
%token tSemi tComa tAffect tPlus tMinus tMult tDiv
%token tPO tPC tAO tAC
%token <number> tNumber 
%token <string> tID

%type  <expression> Expr
%type  <expression> Affect
%type  <expression> FuncCallExpr
%type  <type> Type
%type  <type> PrimType
%type  <type> VarDeclType
%error-verbose

%right tAffect
%left  tAnd tOr
%left  tEquals tNotEquals
%left  tPlus tMinus
%left  tMult tDiv
 
%start Input 

%{
	#include <stdio.h>
	#include "compiler.h"
	void yyerror(char const * errorText);
%}

%union 
{	
        int number;
        char *string;
	type_t* type;
	expression_t expression;
}


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

IVarDeclAff	: 	VarDecl tAffect Expr tSemi;

IVarDecl	:	VarDecl tSemi;

VarDecl		: 	VarDeclType IDList { 
	for(int i = 0; i < idbuffer_size(); i++)
	{
		// TODO : 0xDODO => size ou type
		printf("stable_add %s %p\n", idbuffer_get(i), idbuffer_get(i));
		stable_add(symbols, idbuffer_get(i), -1, ctx.depth, $1);
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

VarDeclType	:	Type {
	idbuffer_init();
	idbuffer_settype(0);
	$$ = $1;
};

IVarAff 	: 	Affect tSemi;


Cond 		: 	Expr ;
If		: 	tIf tPO Cond tPC Body;
While		: 	tWhile tPO Cond tPC Body;
Return		: 	tReturn Expr tSemi;
Print		: 	tPrint tPO Expr tPC tSemi;
Affect		: 	tID tAffect Expr {

};

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
			| tID {  
  symbol_t* sym = stable_find(symbols, $1);
  $$.type = sym->type;
  $$.address = sym->address;
} 
			;
FuncCallExpr	: 	tID tPO Params tPC {

};

TypedParams 	:	TypedParam STypedParams
			| ;

STypedParams	:	tComa TypedParam STypedParams 
			| tComa TypedParam;

TypedParam	:	Type tID;

Params 		: 	Expr SParams
			| ;

SParams 	: 	tComa Expr SParams
			| tComa Expr;

Type 		:   	PrimType {
  $$ = $1; 
}			| Type tMult {
  $$ = type_create_ptr($1);
};

PrimType	:	tINT { 
  $$ = type_create_primitive("int");
}

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

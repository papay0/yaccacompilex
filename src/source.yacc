%token tINT tCHAR tVOID
%token tAnd tOr tEquals tNotEquals tNot
%token tPrint tIf tElse tElseif tWhile tReturn tAssert tMalloc tFree
%token tSemi tComa tAffect tPlus tMinus tMult tDiv tInf tSup tAmpersand
%token tPO tPC tAO tAC tCO tCC tBO tBC
%token <number> tNumber
%token <string> tID
%type  <expression> Expr
%type  <expression> Cond_if
%type  <expression> Cond_while
%type  <expression> Affect
%type  <expression> FuncCallExpr
%type  <expression> Free
%type  <expression> Malloc



%type  <type> Type
%type  <type> PrimType
%type  <type> VarDeclType
%type  <type> FuncDeclType
%type  <type> PtrType
%type  <type> FuncType
%type  <string> FuncProto

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
        #include "ltable.h"
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

Input           :      	GlobalDecls AreaSeparator FuncDecls;

AreaSeparator	:	tMinus tMinus { do_end_of_declarations(); };

GlobalDecls	:	IVarDecl GlobalDecls 
			| IVarDeclAff GlobalDecls
			| ;

FuncDecls	:	FuncDecl FuncDecls
			| ;

FuncDecl        :       FuncImplProto { stackbuff_pop(argbuff); } Body { do_end_of_function_impl();  }
			| FuncProto tSemi { do_end_of_function_decl(); }

FuncImplProto	:	FuncProto { do_func_implementation($1); };

FuncProto	:	FuncDeclType tID tPO TypedParams tPC { do_func_declaration($2, $1); $$ = $2; }

Body            :       BodyStart InstList BodyEnd
                        | BodyStart BodyEnd;

BodyStart       :       tAO { stable_block_enter(symbols); };
BodyEnd         :       tAC { stable_block_exit(symbols); };

InstList        :       Inst InstList 
                        | Inst;

Inst            :       IVarDecl
			| Expr
                        | IVarDeclAff
                        | IVarAff
                        | IFuncCall { } 
                        | If
			| Affect
			| Free tSemi
			| Malloc tSemi
                        | While
                        | Return
			| Assert
                        | Print
			| error tSemi { handle_syntax_error(); yyerrok; };

IFuncCall       :       FuncCallExpr tSemi { do_func_call_instruction($1); };
IVarDeclAff     :       VarDecl tAffect Expr tSemi { do_variable_affectations(&$3); };
IVarDecl        :       VarDecl tSemi { do_end_of_variable_declaration(); };

VarDecl         :       VarDeclType IDList { do_variable_declarations($1); }
			| VarDeclType tID tCO tNumber tCC { do_array_declaration($1, $2, $4); };

IDList          :       VarDeclID SIDList
                        | VarDeclID;

SIDList         :       tComa VarDeclID SIDList
                        | tComa VarDeclID
                        ;

VarDeclID       :       tID { stackbuff_addstr(vardeclbuff, $1); };
VarDeclType     :       Type { stackbuff_push(vardeclbuff); $$ = $1; };

IVarAff         :       Affect tSemi;


Cond_if         :       Expr {do_if($1);} ;
Cond_while      :       Expr {do_while($1);};
If              :       tIf tPO Cond_if tPC Body { do_body(); }
                        | tIf tPO Cond_if tPC Body_if_else Else
                        | tIf tPO Cond_if tPC Body_if_elsif Elseif;
Elseif          :       tElseif tPO Cond_if tPC Body_elsif {do_end_elsif();}
                        | tElseif tPO Cond_if tPC Body_elsif Elseif
                        | tElseif tPO Cond_if tPC Body_elsif Else;
Else            :       tElse Body {do_body_else();};
Body_if_else    :       Body {do_body_if_else();};
Body_if_elsif   :       Body {do_body_if_elsif();}
Body_elsif      :       Body {do_body_elsif();};
Body_elsif_else :       Body {do_body_elsif_else();}
While           :       tWhile POWhile Cond_while PCWhile Body {do_body_while($3);};

Return          :       tReturn Expr tSemi { do_return($2); };
Assert		: 	tAssert Expr tSemi { do_assert($2); };
Malloc		:	tMalloc tPO Expr tPC  { do_malloc($3, &$$); };
Free		:	tFree   tPO Expr tPC { do_free($3, &$$); };
Print           :       tPrint tPO Expr tPC tSemi { do_print($3); }
			| tPrint tPO tID tComa Expr tPC tSemi { do_dprint($3, $5); };

Affect          :       tID tAffect Expr { do_affect($1, $3, &$$, DOAFFECT_NONE); }
			| tMult tID tAffect Expr { do_affect($2, $4, &$$, DOAFFECT_DEREFERENCE); }
			| tMult Expr tAffect Expr { do_affect_dereference($2, $4, &$$, NULL); }
			| Expr tCO Expr tCC tAffect Expr{ do_affect_dereference($1, $6, &$$, $3); };

Expr            :       tPO Expr tPC          { $$ = $2;}
                        | Expr tCO Expr tCC     { do_indexing($1, $3, &$$); }
                        | tMult Expr            { do_unary_operation($2, &$$, "COPA"); }
                        | tAmpersand tID        { do_reference($2, &$$); }
                        | Expr tEquals Expr     { do_operation($1, $3, &$$, "EQ"); }
                        | Expr tNotEquals Expr  { do_operation($1, $3, &$$, "NEQ"); }
                        | Expr tAnd Expr        { do_operation($1, $3, &$$, "AND"); }
                        | Expr tOr Expr         { do_operation($1, $3, &$$, "OR"); }
                        | Expr tInf Expr         { do_operation($1, $3, &$$, "INF"); }
                        | Expr tSup Expr         { do_operation($1, $3, &$$, "SUP"); }
                        | Expr tPlus Expr       { do_operation($1, $3, &$$, "ADD"); }
                        | Expr tMinus Expr      { do_operation($1, $3, &$$, "SUB"); }
                        | Expr tMult Expr       { do_operation($1, $3, &$$, "MUL"); }
                        | Expr tDiv Expr        { do_operation($1, $3, &$$, "DIV"); }
                        | Expr tBO Expr         { do_operation($1, $3, &$$, "INF"); }
                        | Expr tBC Expr         { do_operation($1, $3, &$$, "SUP"); }
			| tNot Expr		{ do_unary_operation($2, &$$, "NOT"); }
			| Malloc
			| Free			
			| tPO Type tPC Expr	{ $4.type = $2; $$ = $4; }
                        | FuncCallExpr 		{ $$ = $1; }
                        | tNumber { do_loadliteral($1, &$$); }
                        | tID { do_loadsymbol($1, &$$); }
                        ;

FuncCallExpr    :       tID tPO Params tPC { do_func_call($1, &$$); };

TypedParams     :       STypedParams
                        | ;

STypedParams    :       TypedParam tComa STypedParams
                        | TypedParam;

TypedParam      :       Type tID { stackbuff_add(argbuff, $1); stackbuff_add(argbuff, $2); };
FuncDeclType	:	Type { stackbuff_push(argbuff); };


Params          :       SParams
                        | { stackbuff_push(argbuff); };

SParams         :       Expr { istream_printf(".pusharg\n");} 
		     		tComa SParams { do_func_pushparam($1, 0); }
                        | Expr { stackbuff_push(argbuff); do_func_pushparam($1, 1); };

Type            :       PrimType
                        | PtrType
                        | FuncType;

FuncType        :       Type tPO tMult tPC tPO TypeList tPC { $$ = do_makefunctype($1); };

TypeList        :       STypeList {  }
                        | { };

STypeList       :       Type tComa STypeList { stackbuff_add(functypebuff, $1); }
                        | Type { stackbuff_push(functypebuff); stackbuff_add(functypebuff, $1); };


PtrType         :       Type tMult { $$ = type_create_ptr($1); };

PrimType        :       tINT    { $$ = type_create_primitive("int"); }
                        | tCHAR { $$ = type_create_primitive("char"); }
			| tVOID { $$ = type_create_primitive("void"); };

POWhile         :       tPO {do_before_while();};
PCWhile         :       tPC {do_after_while();};

%%

void yyerror(char const * errorText) { print_warning("%s\n", errorText); }
int getMode();

int main(int argc, char** argv)
{
  	//ltable_t* labels_global;
	// test_stable(); return 0;
	if(getMode() == 0)
		while(1) { yylex(); }

	if(getMode() == 1)
	{
		ctx_init();
		yyparse();
		ctx_close();
		update_label(labels);
	}
	return 0;
}

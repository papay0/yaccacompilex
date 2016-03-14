%token tINT tCHAR
%token tAnd tOr tEquals tNotEquals tNot
%token tPrint tIf tElse tWhile tReturn
%token tSemi tComa tAffect tPlus tMinus tMult tDiv tAmpersand
%token tPO tPC tAO tAC tCO tCC
%token <number> tNumber
%token <string> tID
%type  <expression> Expr
%type  <expression> Cond_if
%type  <expression> Cond_while
%type  <expression> Affect
%type  <expression> FuncCallExpr
%type  <type> Type
%type  <type> PrimType
%type  <type> VarDeclType
%type  <type> PtrType
%type  <type> FuncType

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

Input           :       FuncDecl Input
                        | FuncDecl ;

FuncDecl        :       Type tID tPO TypedParams tPC Body;


Body            :       BodyStart InstList BodyEnd
                        | BodyStart BodyEnd;

BodyStart       :       tAO { stable_block_enter(symbols); };
BodyEnd         :       tAC { stable_block_exit(symbols); };

InstList        :       Inst InstList
                        | Inst;

Inst            :       IVarDecl
                        | IVarDeclAff
                        | IVarAff
                        | IFuncCall
                        | If
                        | While
                        | Return
                        | Print;

IFuncCall       :       FuncCallExpr tSemi;

IVarDeclAff     :       VarDecl tAffect Expr tSemi { do_variable_affectations(&$3); };
IVarDecl        :       VarDecl tSemi;

VarDecl         :       VarDeclType IDList { do_variable_declarations($1); } ;

IDList          :       VarDeclID SIDList
                        | VarDeclID;

SIDList         :       tComa VarDeclID SIDList
                        | tComa VarDeclID
                        ;

VarDeclID       :       tID { idbuffer_addstr($1); };

VarDeclType     :       Type { idbuffer_init(); $$ = $1; };

IVarAff         :       Affect tSemi;


Cond_if         :       Expr {do_if($1);} ;
Cond_while      :       Expr {do_while($1);};
If              :       tIf tPO Cond_if tPC Body { do_body(); }
                        | tIf tPO Cond_if tPC Body Else;
Else            :       tElse Body;
While           :       tWhile POWhile Cond_while PCWhile Body {do_body_while($3);};
Return          :       tReturn Expr tSemi;
Print           :       tPrint tPO Expr tPC tSemi;
Affect          :       tID tAffect Expr { do_affect($1, $3, 0); $$.address = $3.address; };

Expr            :       Affect
                        | tPO Expr tPC          { $$ = $2;}
                        | Expr tCO Expr tCC     { do_indexing($1, $3, &$$); }
                        | tMult Expr            { do_unary_operation($2, &$$, "COPA"); }
                        | tAmpersand tID        { do_reference($2, &$$); }
                        | Expr tEquals Expr     { do_operation($1, $3, &$$, "EQ"); }
                        | Expr tNotEquals Expr  { do_operation($1, $3, &$$, "NEQ"); }
                        | Expr tAnd Expr        { do_operation($1, $3, &$$, "AND"); }
                        | Expr tOr Expr         { do_operation($1, $3, &$$, "OR"); }
                        | Expr tPlus Expr       { do_operation($1, $3, &$$, "ADD"); }
                        | Expr tMinus Expr      { do_operation($1, $3, &$$, "SUB"); }
                        | Expr tMult Expr       { do_operation($1, $3, &$$, "MUL"); }
                        | Expr tDiv Expr        { do_operation($1, $3, &$$, "DIV"); }
                        | FuncCallExpr
                        | tNumber { do_loadliteral($1, &$$); }
                        | tID { do_loadsymbol($1, &$$); }
                        ;

FuncCallExpr    :       tID tPO Params tPC {

};

TypedParams     :       TypedParam STypedParams
                        | ;

STypedParams    :       tComa TypedParam STypedParams
                        | tComa TypedParam;

TypedParam      :       Type tID;

Params          :       Expr SParams
                        | ;

SParams         :       tComa Expr SParams
                        | tComa Expr;

Type            :       PrimType
                        | PtrType
                        | FuncType;

FuncType        :       Type tPO tMult tPC tPO TypeList tPC { $$ = do_makefunctype($1); };

TypeList        :       STypeList {  }
                        | { };

STypeList       :       Type tComa STypeList { idbuffer_add($1); }
                        | Type { idbuffer_init(); idbuffer_add($1); };


PtrType         :       Type tMult {
  $$ = type_create_ptr($1);
};

PrimType        :       tINT    { $$ = type_create_primitive("int"); }
                        | tCHAR { $$ = type_create_primitive("char"); }

POWhile         :       tPO {do_before_while();};
PCWhile         :       tPC {do_after_while();};

%%

void yyerror(char const * errorText) { }
int getMode();

int main(int argc, char** argv)
{
  //ltable_t* labels_global;
	// test_stable(); return 0;
	if(getMode() == 0)
		while(1) { yylex(); }

	if(getMode() == 1)
		{
      //labels_global = ltable_new();
			//ctx_init(labels_global);
      ctx_init();
			yyparse();
			stable_print(symbols);
			ltable_print(labels);
			ctx_close();
      update_label(labels);
		}

	return 0;
}

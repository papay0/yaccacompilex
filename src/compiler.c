#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "instruction_stream.h"
#include "warning.h"
#include "stable.h"

extern int yylineno;


/* --------------------------
 *  STRUCTURE DU CODE GENERE
 * --------------------------
 * 1.   INIT. GLOBALES
		xx: AFC ...
		xx. JMP bootstrap
 * 2.   CODE FONCTIONS
		xx: int truc() { }
		xx: .main
		xx: int main() { }
 * 3.   INIT. FUNCPTRS
		xx: .bootstrap
		xx: COP ...
		xx: JMP main
 */

// Retourne vrai si le symbole donné est null.
// Si c'est le cas, effectue sa déclaration implicite.
int check_null(symbol_t** symbol, char* name)
{
	if(*symbol == NULL)
	{
		print_warning("symbol %s has not been declared.\n", name);
		print_wnotes("\tnote: implicit declaration of symbol %s, type defaults to int.\n", name);
		// Déclaration implicite du symbole.
		type_t* type = type_create_primitive("int");
		stable_add(symbols, name, type);
		*symbol = stable_find(symbols, name);
		return 1;
	}
	return 0;
}


void ctx_init(char* infile, char* outfile)
{
	ctx.verbose = 0;
	ctx.outfile = outfile;
	tempaddr_init();
	symbols = stable_new();
	labels = ltable_new();
	globals = gtable_new();
	functypebuff = stackbuff_new();
	vardeclbuff = stackbuff_new();
	argbuff = stackbuff_new();
	istream_open(outfile);
	if(infile != NULL)
		istream_printf(".file %s\n", infile);
	istream_printf(".area start\n");
}

void ctx_close()
{
	gtable_printtostream(globals, labels);
	istream_close();
	if(ctx.verbose)
	{
		stable_print(symbols);
		ltable_print(labels);
	}
	printf("Code written to %s !\n", ctx.outfile);
}

int do_operation(expression_t e1, expression_t e2,
	expression_t* r, char* opname)
{
	int addr1 = e1.address;
	int addr2 = e2.address;
	tempaddr_unlock(symbols, addr1);
	tempaddr_unlock(symbols, addr2);
	int newaddr = tempaddr_lock(symbols);
	istream_printf("%s %d %d %d\n", opname, newaddr, addr1, addr2);
	r->address = newaddr;

	if(!type_compatible(e1.type, e2.type, type_getoptype(opname)))
	{
		print_warning("incompatible-pointer-types\n");
		print_wnotes("\tnote: cannot perform operation %s on types '", opname);
		type_sprint(print_wnotes, e1.type);
		print_wnotes("' and '");
		type_sprint(print_wnotes,e2.type);
		print_wnotes("'.\n");
	}
	r->type = e1.type; // FIXME
	return newaddr;
}

void handle_syntax_error()
{
	// print_warning("syntax error before token ';'. Instruction discarded.");
}

// A la fin des déclarations, effectue le saut vers la section "bootstrap".
void do_end_of_declarations()
{
	istream_printf("JMP %d\n", ltable_get_bootstrap(labels));
}

void do_return(expression_t retval)
{
	// On met la valeur au dessus du contexte
	int addr = tempaddr_lock(symbols);
	tempaddr_unlock(symbols, addr);
	istream_printf("COP 1 %d\n", retval.address);
	// On appelle RET
	istream_printf("RET\n");
}

void do_assert(expression_t exp)
{
	istream_printf("ASRT %d\n", exp.address);
	tempaddr_unlock(symbols, exp.address);
}

void do_print(expression_t val)
{
	tempaddr_unlock(symbols, val.address);
	istream_printf("PRI %d\n", val.address);
}

void do_dprint(char* name, expression_t val)
{
	tempaddr_unlock(symbols, val.address);
	istream_printf("PRID %d %s\n", val.address, name);
}

// Rajoute le RET à la fin d'une fonction.
void do_end_of_function_impl()
{
	istream_printf("RET\n");
}

void do_end_of_function_decl()
{
	stackbuff_pop(argbuff);
}

void check()
{
	tempaddr_unlock_all(symbols);	
}

void do_malloc(expression_t size, expression_t* expr)
{
	istream_printf("MAL %d %d\n", size.address, size.address);
	expr->type = type_create_primitive("int");
	expr->address = size.address;
}

void do_free(expression_t addr, expression_t* expr)
{
	istream_printf("FRE %d\n", addr.address);
	expr->type = type_create_primitive("void");
	expr->address = 0;
}

void do_func_call_instruction(expression_t fc_expr)
{
	tempaddr_unlock(symbols, fc_expr.address); 
}

void do_func_pushparam(expression_t expr, int pusharg)
{
	expression_t* cpy = malloc(sizeof(expression_t));
	memcpy(cpy, &expr, sizeof(expression_t));
	stackbuff_add(argbuff, cpy);
	if(pusharg)
		istream_printf(".pusharg\n");
}

void do_func_call(char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	check_null(&symbol, name);
	functype_t* functype = (functype_t*)symbol->type;
	// On vérifie que ce qu'on appelle est bien une fonction.
	if(symbol->type->kind == TYPE_KIND_FUNCTION)
	{
		// Vérification du nombre d'arguments.
		if(functype->argc == stackbuff_size(argbuff))
		{
			for(int i = 0; i < stackbuff_size(argbuff); i++)
			{
				int id = stackbuff_size(argbuff) - i - 1;
				expression_t* expr = (expression_t*)stackbuff_get(argbuff, id);
				type_t* type = expr->type;
				type_t* type2 = functype->arg_types[i];

				// Vérification du type des arguments
				if(!type_equals(type, type2))
				{
					print_warning("call to function %s of type ", name);
					type_sprint(print_wnotes,symbol->type);
					print_wnotes(": expected argument %d of type ", i);
					type_sprint(print_wnotes,type2);
					print_wnotes(" but got ");
					type_sprint(print_wnotes,type);
					print_wnotes(" instead.\n");
				}
			}
		}
		else
		{
			print_warning("call to function %s of type ", name);
			type_sprint(print_wnotes,symbol->type);
			print_wnotes(": expected %d arguments, got %d\n", functype->argc, stackbuff_size(argbuff));
		}
	}
	else
	{
		print_warning("the variable %s is not a function and cannot be called.\n", name);
		

	}
	
	// *** Structure de la pile :
	// 1. Paramètres
	// 2. @retour				<- ctx - 2
	// 3. sp					<- ctx - 1
	// 4. contexte				<- ctx 
	// 5. Variables locales.	
	//		var1				<- ctx + 1
	//		varn				<- sp
	// ***

	
	int addrs[stackbuff_size(argbuff)];
	
	// Tout en haut au dessus des args : @retour de la fonction
	int stack_retaddr = tempaddr_lock(symbols);
	
	// On libère les addresses des arguments
	for(int i = 0; i < stackbuff_size(argbuff); i++)
	{
		expression_t* expr = (expression_t*)stackbuff_get(argbuff,i);
		tempaddr_unlock(symbols, expr->address);
	}

	// On lock l'addresse de la variable temporaire
	int stack_tempvar = tempaddr_lock(symbols);

	int stack_sp = stack_retaddr + 1;
	int stack_ctx = stack_retaddr + 2;
	int stack_retval = stack_retaddr + 3;

	// On libère l'@ tout en haut de la pile
	tempaddr_unlock(symbols, stack_retaddr);

	// L'instruction call va sauvegarder le contexte + le sp.
	int funcaddr = symbol->address;
	istream_printf(".stacksize %d\n", stack_retaddr);
	istream_printf(".popargs %d\n", stackbuff_size(argbuff));
	istream_printf("AFC %d %d\n", stack_retaddr,  get_pc()+2);

	if(stable_isglobal(symbols, name))
	{
		istream_printf("CALL @%d\n", funcaddr);
	}
	else
	{
		istream_printf("CALL %d\n", funcaddr);
	}

	// Copie de la valeur de retour dans la variable temporaire réservée.
	istream_printf("COP %d %d\n", stack_tempvar, stack_retval);

	// Retour de la fonction
	r->address = stack_tempvar;
	if(symbol->type->kind == TYPE_KIND_FUNCTION)
		r->type = functype->return_type;
	else
		r->type = symbol->type; // hacky

	// on en a terminé avec le buffer de paramètres actuel.
	stackbuff_pop(argbuff);
}

void do_body_for_the_if_else_this_name_is_like_shit_I_know(){
    int last_index = -1;
    for (int i = 0; i < labels->index; i++) {
        if (labels->labels[i] == -1) {
            last_index = i;
        }
    }
    labels->labels[last_index] = get_pc()+1;
    ltable_print(labels);
}

void do_body_for_the_if_elsif_this_name_is_like_shit_I_know(){
    int last_index = -1;
    for (int i = 0; i < labels->index; i++) {
        if (labels->labels[i] == -1) {
            last_index = i;
        }
    }
    labels->labels[last_index] = get_pc();
    ltable_print(labels);
}

void do_body_if_elsif() {
    do_body_for_the_if_else_this_name_is_like_shit_I_know();
    istream_printf("JMP %d\n", labels->index);
    printf("--> do_body_if_elsif : JMP %d\n", labels->index);
    ltable_add(labels, -1);
}

void do_body_if_else() {
    do_body_for_the_if_else_this_name_is_like_shit_I_know();
    istream_printf("JMP %d\n", labels->index);
    printf("--> do_body_if_else : JMP %d\n", labels->index);
    ltable_add(labels, -1);
}

void do_end_elsif(){
    int index = do_body_return_index();
    labels->labels[index] = get_pc();
}

void do_body_elsif_else(){
    do_body_for_the_if_else_this_name_is_like_shit_I_know();
    istream_printf("JMP %d\n", labels->index);
    printf("--> do_body_if_else : JMP %d\n", labels->index);
    ltable_add(labels, -1);
    do_end_elsif();
}

void do_body_elsif() {
     int index = do_body_return_index();
     int get_pc_var = get_pc();
     printf(">>> do_body_elsif : index = %d, get_pc = %d\n", index, get_pc_var);
     labels->labels[index] = get_pc_var+1; // pour modifier le JMF du haut

    do_body_for_the_if_elsif_this_name_is_like_shit_I_know(); // pour modifier le JMP du haut
    istream_printf("JMP %d\n", labels->index);
    printf("--> do_body_if_elsif : JMP %d\n", labels->index);
    ltable_add(labels, -1);

}

void do_body_else() {
    int index = do_body_return_index();
    int get_pc_var = get_pc();
    printf(">>> do_body_else : index = %d, get_pc = %d\n", index, get_pc_var);
    labels->labels[index] = get_pc_var;
}


void do_if(expression_t cond) {
	// labels->index est l'index courant du tableau de labels.
	// Puis j'incremente l'index dans le ltable_add();
	istream_printf("JMF %d %d\n", cond.address, labels->index);
	ltable_add(labels, -1);
}

void do_while(expression_t cond) {
	istream_printf("JMF %d %d\n", cond.address, labels->index);
	ltable_add(labels, -1);
}

void do_after_while() {
}

void do_body_while(expression_t cond) {
	int index = do_body_return_index()-1;
	istream_printf("JMP %d\n", index);
}

void do_before_while() {
	ltable_add(labels, get_pc());
}
	// [1] --> create label (get_pc()) // DONE (= do_before_while())
	// [2] --> JMF (-1)
	// [3] --> JMP [1]
	// [4] --> mettre PC dans le dernier -1 (comme do_body())
	// while([1]   1==2)[2]{
	// 	printf("TRUC TRUC\n");
	// 	[3]
	// }
	// [4]
	// int a = 3;

void do_body(){
	int last_index = -1;
	for (int i = 0; i < labels->index; i++) {
		if (labels->labels[i] == -1) {
			last_index = i;
		}
	}
	labels->labels[last_index] = get_pc();
	ltable_print(labels);
}

int do_body_return_index(){
	int last_index = -1;
	for (int i = 0; i < labels->index; i++) {
		if (labels->labels[i] == -1) {
		last_index = i;
		}
	}
	labels->labels[last_index] = get_pc()+1;
	ltable_print(labels);

	// Affiche la table des labels en mode verbose
	if(ctx.verbose)
		ltable_print(labels);

	return last_index;


}



int do_unary_operation(expression_t e1,
	expression_t* r, char* opname)
{
	int addr1 = e1.address;
	tempaddr_unlock(symbols, addr1);
	int newaddr = tempaddr_lock(symbols);
	//printf("%s %d %d\n", opname, newaddr, addr1);
	istream_printf("%s %d %d\n", opname, newaddr, addr1);
	r->address = newaddr;
	// TODO : type check
	if(strcmp(opname, "COPA") == 0)
	{
		if(e1.type->kind != TYPE_KIND_POINTER)
		{
			print_warning("invalid-dereference: trying to dereference non-pointer type '");
			type_sprint(print_wnotes,e1.type);
			print_wnotes("'.\n");
			print_wnotes("\tnote: type of expression defaults to '");
			type_sprint(print_wnotes,e1.type);
			print_wnotes("'.\n");
			r->type = e1.type;
		}
		else
		{
			ptrtype_t* ptr = (ptrtype_t*)e1.type;
			r->type = ptr->type;
		}
	}
	else
	{
		r->type = e1.type; // FIXME
	}
	return newaddr;
}

void check_type_affect(type_t* exprtype, type_t* dest)
{
	if(!type_compatible(dest, exprtype, OPTYPE_AFFECT))
	{
		print_warning("incompatible-pointer-types : \n");
		print_wnotes("\tnote: types '");
		type_sprint(print_wnotes,dest);
		print_wnotes("' and '");
		type_sprint(print_wnotes,exprtype);
		print_wnotes("' are not compatible for affectation.\n");
	}
}

void do_affect_dereference(expression_t dst, expression_t src,
	 expression_t* r, expression_t* offset)
{
	if(offset != NULL)
	{
		expression_t tmp;
		do_operation(dst, *offset, &tmp, "ADD");
		istream_printf("COPB %d %d\n", tmp.address, src.address);
		r->address = tmp.address;
		r->type = tmp.type; // FIXME

	}
	else
	{
		istream_printf("COPB %d %d\n", dst.address, src.address);
		r->address = dst.address;
		r->type = dst.type; // FIXME
	}

}
void do_affect(char* name, expression_t expr, expression_t* r, int op)
{
	symbol_t* symbol = stable_find(symbols, name);
	check_null(&symbol, name);
 	
	int addr = expr.address;
	int addr2 = symbol->address;
	if((op & DOAFFECT_DEREFERENCE) != 0) 
	{
		if(stable_isglobal(symbols, name))
		{
			istream_printf("COPB @%d %d\n", addr2, addr);
		}
		else
		{
			istream_printf("COPB %d %d\n", addr2, addr);
		}
	} 
	else 
	{
		if(stable_isglobal(symbols, name))
		{
			istream_printf("COP @%d %d\n", addr2, addr);
		}
		else
		{
			istream_printf("COP %d %d\n", addr2, addr);
		}
	}

	check_type_affect(expr.type, symbol->type);
	// On affecte la valeur retour à l'expression de droite.
	r->address = expr.address;
	r->type = expr.type;
}
void do_loadliteral(int literalValue, expression_t* r)
{
		int addr = tempaddr_lock(symbols);
		istream_printf("AFC %d %d\n", addr, literalValue);
		r->address = addr;
		r->type = type_create_primitive("int");
}


void do_loadsymbol( char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	check_null(&symbol, name);

	int addr = tempaddr_lock(symbols);

	// Addressage global si symbole global ou profondeur à 0.
	if(stable_isglobal(symbols, name))
		istream_printf("COP %d @%d\n", addr, symbol->address);
	else
		istream_printf("COP %d %d\n", addr, symbol->address);
	
	r->type = symbol->type;
	r->address = addr;
}

void do_variable_declarations(type_t* type)
{
	for(int i = 0; i < stackbuff_size(vardeclbuff); i++)
	{
		char* symbol_name = stackbuff_get(vardeclbuff, i);
		symbol_t* symbol =  stable_add(symbols, (char*)symbol_name, type);
		istream_printf(".local %d %s\n", symbol->address, symbol_name);
	}
}

void do_end_of_variable_declaration()
{
	// Appelé après la fin d'une déclaration de variable (sans affectation).
	stackbuff_pop(vardeclbuff);
}

void do_variable_affectations(expression_t* expr)
{
	for(int i = 0; i < stackbuff_size(vardeclbuff); i++)
	{
		char* symbol = (char*)stackbuff_get(vardeclbuff, i);
		expression_t tmp;
		do_affect(symbol, *expr, &tmp, DOAFFECT_NONE);
		tempaddr_unlock(symbols, tmp.address);
	}
	stackbuff_pop(vardeclbuff);
}

void do_func_declaration(char* name, type_t* return_type) 
{
	int size = stackbuff_size(argbuff)/2;
	type_t** args = malloc(sizeof(type_t*)*size);
	// Ici idbuffer contient :
	// i : type du symbole, i+1 : nom du symbole
	for(int i = 0; i < stackbuff_size(argbuff); i+=2)
	{
		args[i/2] = stackbuff_get(argbuff, i);
	}

	// On crée le type et on ajoute la fonction à la table des symboles.
	type_t* functype = type_create_func(return_type, args, size);
	stable_add(symbols, name, functype);	
	stable_setflags(symbols, name, SYMBOL_FUNC);
	istream_printf(".function %s %d\n", name, size);
}

void do_func_implementation(char* name)
{
	// Déclaration des arguments de la fonction dans la table des symboles
	stable_block_enter(symbols);
	for(int i = 0; i < stackbuff_size(argbuff); i+=2)
	{
		symbol_t* s = stable_add(symbols, stackbuff_get(argbuff, i+1), stackbuff_get(argbuff, i));
		// -3 : @retour, contexte, sp
		// idbuffer_size() + i / 2 => dans l'ordre d'ajout
		s->address = -3 - (stackbuff_size(argbuff) - i)/2 + 1;
	}
	// Ajout du contexte + @retour
	symbol_t* ra = stable_add(symbols, "<@>", type_create_primitive("int"));
	symbol_t* sp = stable_add(symbols, "<sp>", type_create_primitive("int"));
	symbol_t* ctx = stable_add(symbols, "<ctx>", type_create_primitive("int"));	
	ra->address = -2;
	sp->address = -1;
	ctx->address = 0;

	stable_block_exit_dirtyhack(symbols);
	stable_setflags(symbols, name, SYMBOL_FUNC | SYMBOL_INITIALIZED);

	// stable_print(symbols);
	// Affectation du pointeur de la fonction au PC
	symbol_t* symbol = stable_find(symbols, name);
	gtable_add(globals, symbol->address, get_pc()); 

	// S'il s'agit de la fonction main
	// On ajoute le label main à la table des labels.
	if(strcmp(name, "main") == 0)
	{
		// Cas particulier : on ajoute l'@ du symbole 
		// pour le call !
		ltable_set_main(labels, symbol->address);
	}
}

void do_array_declaration(type_t* type, char* name, int size)
{
	type_t* arrtype = type_create_ptr(type);
	symbol_t* symbol = stable_add(symbols, name, arrtype);
	for(int j = 0; j < size; j++)
	{
		stable_add(symbols, "<array>", arrtype);
	}
	istream_printf(".array %s %d %d\n", symbol->name, symbol->address, size);
	istream_printf("PTR %d %d\n", symbol->address, symbol->address+1);
}

type_t* do_makefunctype(type_t* return_type)
{
	int size = stackbuff_size(functypebuff);
	int argc = size;
  	type_t** args = (type_t**)malloc(sizeof(type_t*)*argc);
  	for(int i = 0; i < size; i++)
	{
		int id = size - i - 1;
    	args[id] = stackbuff_get(functypebuff, i);
	}
	stackbuff_pop(functypebuff);
	type_t* func = type_create_func(return_type, args, argc);
	return func;
}

void do_reference(char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	check_null(&symbol, name);

	r->type = type_create_ptr(symbol->type);
	r->address = tempaddr_lock(symbols);

	if(stable_isglobal(symbols, name))
	{
		istream_printf("PTR %d @%d\n", r->address, symbol->address);
	}
	else
	{
		istream_printf("PTR %d %d\n", r->address, symbol->address);
	}

}

void do_indexing(expression_t array, expression_t index, expression_t* r)
{
	expression_t tmp;
	do_operation(array, index, &tmp, "ADD");
	do_unary_operation(tmp, r, "COPA");
}

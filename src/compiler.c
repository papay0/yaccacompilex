#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "instruction_stream.h"
#include "warning.h"
#include "stable.h":

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
		print_wnotes("note: implicit declaration of symbol %s, type defaults to int.\n", name);
		// Déclaration implicite du symbole.
		type_t* type = type_create_primitive("int");
		int addr = stable_add(symbols, name, type);
		*symbol = stable_find(symbols, name);
		return 1;
	}
	return 0;
}

void ctx_init()
{
	ctx.verbose = 0;
	tempaddr_init();
	symbols = stable_new();
	labels = ltable_new();
	globals = gtable_new();
	istream_open();
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
	printf("Code written to bin/yaccacompilex !\n");
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
		print_wnotes("note: cannot perform operation %s on types '", opname);
		type_print(e1.type);
		print_wnotes("' and '");
		type_print(e2.type);
		print_wnotes(".\n");
	}
	r->type = e1.type; // FIXME le type peut dépendre de l'opération
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
void do_end_of_function()
{
	istream_printf("RET\n");
}

void check()
{
	tempaddr_unlock_all(symbols);	
}


void do_func_call_instruction(expression_t fc_expr)
{
	tempaddr_unlock(symbols, fc_expr.address); 
}

void do_func_pushparam(expression_t expr, int pusharg)
{
	printf("expression add : %d\n ", expr.address);
	expression_t* cpy = malloc(sizeof(expression_t));
	memcpy(cpy, &expr, sizeof(expression_t));
	parambuffer_add(cpy);
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
		if(functype->argc == parambuffer_size())
		{
			for(int i = 0; i < parambuffer_size(); i++)
			{
				expression_t* expr = (expression_t*)parambuffer_get(i);

				printf("param %p\n", expr);
				type_t* type = expr->type;
				type_t* type2 = functype->arg_types[i];

				// Vérification du type des arguments
				if(!type_equals(type, type2))
				{
					print_warning("call to function %s of type ", name);
					type_print(symbol->type);
					print_wnotes(": expected argument %d of type ", i);
					type_print(type2);
					print_wnotes(" but got ");
					type_print(type);
					print_wnotes(" instead.\n");
				}
			}
		}
		else
		{
			print_warning("call to function %s of type ", name);
			type_print(symbol->type);
			print_wnotes(": expected %d arguments, got %d\n", functype->argc, parambuffer_size());
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

	// On récupère l'adresse la plus haute <=> stack pointer
	int top = tempaddr_lock(symbols);
	tempaddr_unlock(symbols, top);

	// Itération sur tous les paramètres
	for(int i = 0; i < parambuffer_size(); i++)
	{
		expression_t* expr = (expression_t*)parambuffer_get(i);
		// Chacune de ces variables est déjà stockée sur la pile
		// en temps que variable temporaire.
		
		// Déverouillage des addresses temporaires	
		tempaddr_unlock(symbols, expr->address);
	}

	// On récupère l'@ de la valeur de retour
	int retvalue_addr = tempaddr_lock(symbols);

	// On ajoute l'@ de retour
	istream_printf(".ra\n");
	istream_printf("AFC %d %d\n", top, get_pc()+2);


	// L'instruction call va sauvegarder le contexte.
	int funcaddr = symbol->address;
	istream_printf("CALL @%d\n", funcaddr);
	istream_printf(".popargs %d\n", parambuffer_size());

	// Copie de la valeur de retour dans la variable temporaire réservée.
	istream_printf("COP %d %d\n", retvalue_addr, top+3);

	// Retour de la fonction
	// La valeur de retour de la fonction est positionnée à CTX+1
	// top   = @ de l'@retour
	// top+1 = @ du sp
	// top+2 = @ du ctx
	// top+3 = @ du ctx+1
	r->address = retvalue_addr;
	if(symbol->type->kind == TYPE_KIND_FUNCTION)
		r->type = functype->return_type;
	else
		r->type = symbol->type; // hacky

	// on en a terminé avec le buffer de paramètres actuel.
	parambuffer_pop();
}

void do_if(expression_t cond){
	istream_printf("JMF %d %d\n", cond.address, labels->index);
	ltable_add(labels, -1);
}

void do_body(){
	int last_index = -1;
	for (int i = 0; i < labels->index; i++) {
			if (labels->labels[i] == -1) {
				last_index = i;
			}
	}
	labels->labels[last_index] = get_pc();

	// Affiche la table des labels en mode verbose
	if(ctx.verbose)
		ltable_print(labels);
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
			type_print(e1.type);
			print_wnotes("'.\n");
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
		print_wnotes("note: types '");
		type_print(dest);
		print_wnotes("' and '");
		type_print(exprtype);
		print_wnotes("' are not compatible for affectation.\n");
	}
}

void do_affect(char* name, expression_t expr, int op)
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
	if((op & DOAFFECT_UNLOCK) != 0) 
	{ 
		tempaddr_unlock(symbols, addr);
	}
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
	for(int i = 0; i < idbuffer_size(); i++)
	{
		stable_add(symbols, (char*)idbuffer_get(i), type);
		istream_printf(".local %s\n", idbuffer_get(i));
	}
}

void do_variable_affectations(expression_t* expr)
{
	for(int i = 0; i < idbuffer_size(); i++)
	{
		char* symbol = (char*)idbuffer_get(i);
		do_affect(symbol, *expr, DOAFFECT_UNLOCK);
	}
}

void do_func_declaration(char* name, type_t* return_type) 
{
	int size = idbuffer_size()/2;
	type_t** args = malloc(sizeof(type_t*)*size);
	// Ici idbuffer contient :
	// i : type du symbole, i+1 : nom du symbole
	for(int i = 0; i < idbuffer_size(); i+=2)
	{
		args[i/2] = idbuffer_get(i);
	}
	type_t* functype = type_create_func(return_type, args, size);
	stable_add(symbols, name, functype);	
	stable_setflags(symbols, name, SYMBOL_FUNC);
	istream_printf(".function %s\n", name);
}

void do_func_implementation(char* name)
{
	// Déclaration des arguments de la fonction dans la table des symboles
	stable_block_enter(symbols);
	for(int i = 0; i < idbuffer_size(); i+=2)
	{
		symbol_t* s = stable_add(symbols, idbuffer_get(i+1), idbuffer_get(i));
		// -3 : @retour, contexte, sp
		// idbuffer_size() + i / 2 => dans l'ordre d'ajout
		s->address = -3 - (idbuffer_size() - i)/2 + 1;
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

	stable_print(symbols);
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
	stable_add(symbols, name, arrtype);
	for(int j = 1; j < size; j++)
	{
		stable_add(symbols, "<array>", arrtype);
	}
}

type_t* do_makefunctype(type_t* return_type)
{
  	type_t** args = (type_t**)malloc(sizeof(type_t*)*idbuffer_size());
  	for(int i = 0; i < idbuffer_size(); i+=2)
	{
    	args[idbuffer_size() - i - 1] = idbuffer_get(i);
	}
	type_t* func = type_create_func(return_type, args, idbuffer_size());
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

/* arr[i] *(arr+i) */
/*
int main()
{
	stable_t* stable = stable_new();
	stable_add(stable, "a", 5, 0, 4);
	stable_add(stable, "b", 130, 0, 4);
	stable_add(stable, "c", 12, 1, 4);
	stable_setflags(stable, "b", SYMBOL_INITIALIZED | SYMBOL_CONST);
	stable_remove(stable, -1);
	stable_add(stable, "d", 12, 0, 4);
	stable_print(stable);
}
*/

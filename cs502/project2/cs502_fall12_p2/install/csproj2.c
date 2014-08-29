#include <stdio.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cgraph.h"
#include "string.h"
#include "hashtab.h"
#include "real.h"
#include "tree-iterator.h"

#define DUMMY -7

struct sym_record
{
  char name[50];
  int ptr; // for pointer, the record it actually pointing to; default 0
  int is_init; // default 0   if ptr == -7 is_init ==-7, its a fake record for function seperation
};

struct sym_record symtab[200];
int sym_len;

struct sym_record symtab_copy[200];
int sym_len_copy;

struct func_record 
{
    char name[50];
    char list[100];
    int is_logging;
};

struct func_record func_list[200];
int func_num;



void save_symtab()
{
    sym_len_copy = sym_len;
    int i;
    for(i=0;i<200;i++)
	symtab_copy[i] = symtab[i];
}

void load_symtab()
{
    sym_len = sym_len_copy;
    int i;
    for(i=0;i<200;i++)
	symtab[i] = symtab_copy[i];
}

void print_result(FILE *fout)
{
    int i;
    for (i=1;i<func_num;i++)
    {
	if (strlen(func_list[i].list)>0)
	{
	    func_list[i].list[strlen(func_list[i].list)-1] = '\0';
	    fprintf(fout, "%s:%s\n", func_list[i].name, func_list[i].list);    
	}
    }
    
    if (strlen(func_list[0].list)>0)
    {
	func_list[0].list[strlen(func_list[0].list)-1] = '\0';
	fprintf(fout, "%s:%s\n", func_list[0].name, func_list[0].list);
    }
}

int find_sym(char *name)
{
    // check local
    int i = sym_len-1;    
    while(symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)!=0) 
	i--;
    if (symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)==0)
    {
	return i;
    }
    else
    {
	// check global 
	i = 0;
	while(symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)!=0)
	    i++;
	if (symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)==0)
	{
	    return i;
	}
    }
    return -1;
}

int is_str_logged(char *list, char* name)
{
    char str[100];
    strcpy(str, name);
    strcat(str, ",");
    if (strstr(list, str)!=NULL)
	return 1;
    else
	return 0;
}

void check_var_init(char *name)
{
    int i = sym_len-1;
    // check local
    while(symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)!=0) i--;
    if (symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)==0)
    {
	if((symtab[i].ptr==0 && !symtab[i].is_init) 
	    || (symtab[i].ptr>0 && !symtab[symtab[i].ptr].is_init))
	{
	    //log into the current function
	    int num = find_current_func();
	    if (!is_str_logged(func_list[num].list, name))
	    {
		strcat(func_list[num].list, name);
		strcat(func_list[num].list, ",");
	    }    
	}
	return;
    }
    
    // check global 
    i = 0;
    while(symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)!=0) i++;
    if (symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)==0)
    {
	if((symtab[i].ptr==0 && !symtab[i].is_init) 
	    || (symtab[i].ptr>0 && !symtab[symtab[i].ptr].is_init))
	{
	    //log into the global function
	    int num = find_current_func();
	    if (!is_str_logged(func_list[0].list, name))
	    {
		strcat(func_list[0].list, name);
		strcat(func_list[0].list, ",");
	    }
	}
    }    
}

void unparse_component_ref(tree node, FILE *fout)
{
    tree left_node = TREE_OPERAND(node, 0);
    tree right_node = TREE_OPERAND(node, 1);    
    unparse_expr(left_node, fout);
    unparse_expr(right_node, fout);
}


int find_current_func()
{
    int i = sym_len-1;    
    while(symtab[i].is_init!=DUMMY) i--;
    char func_name[100];        
    strcpy(func_name, symtab[i].name);
    for (i=0;i<func_num;i++)
    {
	if (strcmp(func_list[i].name, func_name)==0)
	    return i;
    }
    return 0;
}

void unparse_indirect_ref(tree node, FILE *fout)
{
    unparse_expr(TREE_OPERAND(node, 0), fout);
}


void unparse_constructor_expr(tree node, FILE *fout)
{
    if (TREE_CODE(TREE_TYPE(node)) == ARRAY_TYPE)
    {
	unsigned HOST_WIDE_INT ix;
	tree field, val;
	int i = 0;
	FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (node), ix, field, val)
	{
	    if (val)
	    {
		unparse_expr(val, fout);    
	    }
	    i++;
	}
    }
    else if (TREE_CODE(TREE_TYPE(node)) == RECORD_TYPE)    
    {
	unsigned HOST_WIDE_INT ix;
	tree field, val;
	int i = 0;
	FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (node), ix, field, val)
	{
	    if (val)
	    {
		unparse_expr(val, fout);    
	    }
	    i++;
	}
    }
}

void unparse_expr(tree node, FILE *fout)
{
    int i;
    char type_name[100];
    char str[100];
    tree left_node, right_node;
    
    switch(TREE_CODE(node))
    {
	case INTEGER_CST:    
	    break;
	    
	case REAL_CST: 
	    break;
	
	case STRING_CST:
	    break;
	
	case PARM_DECL:
	case VAR_DECL: 
	    //check the symtab to see if it is defined    
	    // &a need to be handled saperately 
	    check_var_init(IDENTIFIER_POINTER(DECL_NAME(node))); 
	    break;
    
	case CALL_EXPR:
	    unparse_call_expr(node, fout);
	    break;

	case ADDR_EXPR:
	    
	    if (TREE_CODE(TREE_OPERAND(node, 0)) == VAR_DECL || 
		TREE_CODE(TREE_OPERAND(node, 0)) == PARM_DECL)
		
	    {
	    }
	    else
	    {
		unparse_expr(TREE_OPERAND(node, 0), fout);
	    }    
	    break;  
	case NOP_EXPR:
	case FIX_TRUNC_EXPR:
	    unparse_expr(TREE_OPERAND(node, 0), fout);
	    break;
	case PLUS_EXPR:
	case MINUS_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    right_node = TREE_OPERAND(node, 1);    
	    unparse_expr(left_node, fout);
	    unparse_expr(right_node, fout);
	    break;
	case TRUNC_DIV_EXPR:
	case RDIV_EXPR:
	case MULT_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    if (TREE_CODE(left_node)==PLUS_EXPR || TREE_CODE(left_node)==MINUS_EXPR)
	    {
		unparse_expr(left_node, fout);
	    }
	    else 
		unparse_expr(left_node, fout);
	    right_node = TREE_OPERAND(node, 1);    
	    if (TREE_CODE(right_node)==PLUS_EXPR || TREE_CODE(right_node)==MINUS_EXPR)
	    {
		unparse_expr(right_node, fout);
	    }
	    else
		unparse_expr(right_node, fout);    
	    break;    
	case TRUTH_NOT_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    if (TREE_CODE(left_node)==PARM_DECL || TREE_CODE(left_node)==VAR_DECL)
		unparse_expr(left_node, fout);
	    else
	    {
		unparse_expr(left_node, fout);
	    }    
	    break;    
	case TRUTH_ANDIF_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    if (TREE_CODE(left_node)==TRUTH_ORIF_EXPR)
	    {
		unparse_expr(left_node, fout);
	    }
	    else 
		unparse_expr(left_node, fout);
	    right_node = TREE_OPERAND(node, 1);    
	    if (TREE_CODE(right_node)==TRUTH_ORIF_EXPR)
	    {
		unparse_expr(right_node, fout);
	    }
	    else
		unparse_expr(right_node, fout);	    
	    break;    
	case GT_EXPR:
	case LT_EXPR:
	case GE_EXPR:
	case LE_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    if (TREE_CODE(left_node)==EQ_EXPR || 
		TREE_CODE(left_node)==NE_EXPR || 
		TREE_CODE(left_node)==TRUTH_ANDIF_EXPR || 
		TREE_CODE(left_node)==TRUTH_ORIF_EXPR)
	    {
		unparse_expr(left_node, fout);
	    }
	    else 
		unparse_expr(left_node, fout);
	    
	    right_node = TREE_OPERAND(node, 1);    
	    if (TREE_CODE(right_node)==TRUTH_ORIF_EXPR)
	    {
		unparse_expr(right_node, fout);
	    }
	    else
		unparse_expr(right_node, fout);
	    break;
	    
	case EQ_EXPR:
	case NE_EXPR:
	case TRUTH_ORIF_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    right_node = TREE_OPERAND(node, 1);    
	    unparse_expr(left_node, fout);
	    unparse_expr(right_node, fout);
	    break;    
	case CONSTRUCTOR:
	    unparse_constructor_expr(node, fout);
	    break;    
	case INDIRECT_REF:
	    unparse_indirect_ref(node, fout);
	    break;
	case COMPONENT_REF:
	    unparse_component_ref(node, fout);
	    break;
	case FIELD_DECL:    
	    break;
	case ARRAY_REF:
	    left_node = TREE_OPERAND(node, 0);
	    right_node = TREE_OPERAND(node, 1);    
	    unparse_expr(left_node, fout);
	    unparse_expr(right_node, fout);
	    break;
	default:
	    debug_tree(node);
	    break;
    }
   
}

struct cgraph_node *find_func_by_name(char *str)
{
    struct cgraph_node * node = cgraph_nodes;
    char func_name[100];    
    strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(node->decl)));    
    if (strcmp(func_name, str)==0) return node->decl;
    while (node->next)
    {
	node = node->next;
	strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(node->decl)));
	if (strcmp(func_name, str)==0) return node->decl;
    }
    return NULL;
}

void print_symtab()
{
    int i;
    printf("===========================================\n");
    printf("\tname\tpoint to\tis_init\n");
    for (i=0;i<sym_len;i++)
    {
	printf("\t");
	if (i==0 || symtab[i].ptr == DUMMY) printf("===");
	printf("%s", symtab[i].name);
	printf("\t     %d\t\t%d\n", symtab[i].ptr, symtab[i].is_init);
    }
    printf("===========================================\n");
}

void symtab_push(char *s)
{
    strcpy(symtab[sym_len].name, s);
    symtab[sym_len].ptr = 0;
    symtab[sym_len].is_init = 0;
    sym_len++;
}

void unparse_call_expr(tree node, FILE *fout)
{
    
    //print call name 
    tree tmp = CALL_EXPR_FN(node);
    while (tmp && TREE_CODE(tmp)!=FUNCTION_DECL)
	tmp = TREE_OPERAND(tmp, 0);
    char func_name[100];
    strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(tmp)));
    //printf("-----%s------\n", func_name);
    
    //print call parameter
    if (strcmp(func_name, "scanf")==0)
    {
	tree arg;
	call_expr_arg_iterator iter;
	FOR_EACH_CALL_EXPR_ARG (arg, iter, node)
	{
	    while (TREE_CODE(arg)==ADDR_EXPR) arg = TREE_OPERAND(arg, 0);
	    
	    if (TREE_CODE(arg)==VAR_DECL || 
		TREE_CODE(arg)==PARM_DECL) 
	    {
		char * name = IDENTIFIER_POINTER(DECL_NAME(arg));
		int num = find_sym(name);
		symtab[num].is_init = 1;
		if (symtab[num].ptr>0)
		{
		    symtab[symtab[num].ptr].is_init = 1;
		}
	    }
	}
	return;
    }
    
    if (strcmp(func_name, "printf")==0)
    {
	tree arg;
	call_expr_arg_iterator iter;
	FOR_EACH_CALL_EXPR_ARG (arg, iter, node)
	{
	    unparse_expr(arg, fout);    
	} 
	return;
    }
    
    //other functions, need to dfs into    
    tree arg;
    call_expr_arg_iterator iter;
    int temp = sym_len+1;
    FOR_EACH_CALL_EXPR_ARG (arg, iter, node)
    {
	if (TREE_CODE(arg)==ADDR_EXPR)
	{
	    while (TREE_CODE(arg)==ADDR_EXPR) arg = TREE_OPERAND(arg, 0);
	    if (TREE_CODE(arg)==VAR_DECL || 
		TREE_CODE(arg)==PARM_DECL) 
	    {
		char * name = IDENTIFIER_POINTER(DECL_NAME(arg));
		int num = find_sym(name);
		symtab[temp].ptr = num;
	    }
	}
	else
	    unparse_expr(arg, fout);
	temp++;
    } 
    
    analysis_func(find_func_by_name(func_name), fout);    
}

void print_var_tree(tree node, int level, FILE *fout)
{
    int tmp;
    char* tmpstr;
    
    switch (TREE_CODE(node))
    {
	case FIELD_DECL:
	case VAR_DECL:
	    
	    symtab_push(IDENTIFIER_POINTER(DECL_NAME(node)));
	    if (TREE_CODE(TREE_TYPE(node)) == ARRAY_TYPE ||
		TREE_CODE(TREE_TYPE(node)) == RECORD_TYPE) 
    	    {
		symtab[sym_len-1].is_init = 1;
	    }
	    
	    if (DECL_INITIAL(node))
	    {
		if (TREE_CODE(DECL_INITIAL(node))==REAL_CST || 
		    TREE_CODE(DECL_INITIAL(node))==INTEGER_CST ||
		    TREE_CODE(DECL_INITIAL(node))==CONSTRUCTOR)
		{
		    symtab[sym_len-1].is_init = 1;
		    unparse_expr(DECL_INITIAL(node), fout);
		}
		else
		{
		    // addr_expr   e.g. int * a = &d;
		    if (TREE_CODE(DECL_INITIAL(node))==ADDR_EXPR)
		    {
			// find the previous var and link them up
			if (TREE_CODE(TREE_OPERAND(DECL_INITIAL(node), 0))==VAR_DECL || 
			    TREE_CODE(TREE_OPERAND(DECL_INITIAL(node), 0))==PARM_DECL) 
			{
			    char * name = IDENTIFIER_POINTER(DECL_NAME(TREE_OPERAND
			    (DECL_INITIAL(node), 0)));
			    // check local
			    int i = sym_len-1;    
			    while(symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)!=0) 
				i--;
			    if (symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)==0)
			    {
				symtab[sym_len-1].is_init = symtab[i].is_init;
				symtab[sym_len-1].ptr = i;
			    }
			    else
			    {
				// check global 
				i = 0;
				while(symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)!=0)
				    i++;
				if (symtab[i].is_init!=DUMMY && strcmp(symtab[i].name, name)==0)
				{
				    symtab[sym_len-1].is_init = symtab[i].is_init;
				    symtab[sym_len-1].ptr = i;
				}
			    }
			}
			else
			{
			    debug_tree(DECL_INITIAL(node));
			}
		    }
		    else
			unparse_expr(TREE_OPERAND(DECL_INITIAL(node), 0), fout);
		}
	    }
	    break;
	case REAL_TYPE:    
	case INTEGER_TYPE:
	    print_var_tree(DECL_NAME(TYPE_NAME(node)), level, fout);   
	    break;  
	case IDENTIFIER_NODE:
	    break;
	case RECORD_TYPE:
	    print_var_tree(TYPE_NAME(node), level, fout);	    
	    break;
	case POINTER_TYPE:
	    tmp = 0;
	    tree tmp_arg = node;
	    while(TREE_CODE(tmp_arg)==POINTER_TYPE) 
	    {
		tmp++;
		tmp_arg = TREE_TYPE(tmp_arg);    
	    }
	    print_var_tree(tmp_arg, level, fout);    
	    while (tmp) 
	    {
		tmp--;
	    }
	    break;
	default:    
	    debug_tree(node);
	    break;
    }      
}

void unparse_decl_expr(tree node, int level, FILE *fout)
{
    print_var_tree(TREE_OPERAND(node, 0), level, fout);
}

void unparse_modify_expr(tree node, FILE *fout)
{
    tree left_node = TREE_OPERAND(node, 0);    
    if (TREE_CODE(left_node) == COMPONENT_REF||
	TREE_CODE(left_node) == ARRAY_REF)
    {
	unparse_expr(left_node, fout);
    }
    else if (TREE_CODE(left_node) == INDIRECT_REF)
    {
	while (TREE_CODE(left_node) == INDIRECT_REF)
	{
	    left_node = TREE_OPERAND(left_node, 0);
	}
	int num = find_sym(IDENTIFIER_POINTER(DECL_NAME(left_node)));
	symtab[num].is_init = 1;
	if (symtab[num].ptr>0)
	{
	    symtab[symtab[num].ptr].is_init = 1;
	}
    }
    else
    {
	int num = find_sym(IDENTIFIER_POINTER(DECL_NAME(left_node)));
	symtab[num].is_init = 1;
    }
    
    tree right_node = TREE_OPERAND(node, 1);
    unparse_expr(right_node, fout);
}


void unparse_cond_expr(tree node, int level, FILE *fout)
{
    int i;
    //condition
    tree arg0 = TREE_OPERAND(node, 0);
    unparse_expr(arg0, fout);
    
    //arg1
    save_symtab();
    tree arg1 = TREE_OPERAND(node, 1);
    if (TREE_CODE(arg1) == STATEMENT_LIST || TREE_CODE(arg1) == BIND_EXPR)
    {
	unparse_stmt(arg1, level, fout);
    }
    else
	unparse_stmt(arg1, level+1, fout);
    load_symtab();
    //arg2
    save_symtab();
    tree arg2 = TREE_OPERAND(node, 2);
    if (arg2)
    {
	if (TREE_CODE(arg2) == STATEMENT_LIST || TREE_CODE(arg2) == BIND_EXPR)
	{
	    unparse_stmt(arg2, level, fout);
	}
	else
	    unparse_stmt(arg2, level+1, fout);
    } 
    load_symtab();
}

void unparse_stmt_list(tree node, int level, FILE *fout)
{
    int i;
    save_symtab();
    tree_stmt_iterator si;    
    for(si = tsi_start(node); !tsi_end_p(si);tsi_next(&si))
    {
	unparse_stmt(tsi_stmt(si), level+1, fout);  
    }
    load_symtab();
}

void unparse_switch_expr(tree node, int level, FILE *fout)
{
    int i;        
    unparse_expr(TREE_OPERAND(node, 0), fout);
    unparse_stmt(TREE_OPERAND(node, 1), level, fout);    
}

void unparse_case_label_expr(tree node, int level, FILE *fout)
{
    
    if (TREE_OPERAND(node, 0))
    {
       unparse_expr(TREE_OPERAND(node, 0), fout);
    }
}

void unparse_stmt(tree node, int level, FILE *fout)
{
    int i;
    //printf("------------------------\n\n\n");    
    switch (TREE_CODE(node))
    {
	case CALL_EXPR:
	    unparse_call_expr(node, fout);
	    break;
	case MODIFY_EXPR:
	    unparse_modify_expr(node, fout);
	    break;
	case DECL_EXPR:
	    unparse_decl_expr(node, level, fout);	    
	    break;
	
	case RETURN_EXPR:
	    if (TREE_OPERAND(node, 0))
	    {
		unparse_expr(TREE_OPERAND(TREE_OPERAND(node, 0),1), fout);
	    }
	    break;
	case COND_EXPR:
	    unparse_cond_expr(node, level, fout);    
	    break;
	case STATEMENT_LIST:
	    unparse_stmt_list(node,level,fout);
	    break;
	case BIND_EXPR:
	    if (TREE_CODE(BIND_EXPR_BODY(node)) == STATEMENT_LIST)
	    {
		unparse_stmt_list(BIND_EXPR_BODY(node), level, fout);
	    }
	    else
	    {
		unparse_stmt(BIND_EXPR_BODY(node), level+1, fout);
	    }    
	    break;    
	case GOTO_EXPR:
	    break;    
	case LABEL_EXPR:
	    break;
	case SWITCH_EXPR:
	    unparse_switch_expr(node, level, fout);
// 	    break;
	case CASE_LABEL_EXPR:
	    unparse_case_label_expr(node, level, fout);
	    break;
	case PREDECREMENT_EXPR:
            unparse_expr(TREE_OPERAND(node, 0), fout);    
	    break;
	case PREINCREMENT_EXPR:
            unparse_expr(TREE_OPERAND(node, 0), fout);            
	    break;	    
	case POSTDECREMENT_EXPR:
            unparse_expr(TREE_OPERAND(node, 0), fout);
	    break;
	case POSTINCREMENT_EXPR:
            unparse_expr(TREE_OPERAND(node, 0), fout);
	    break;
	default:
	    debug_tree(node);
	    break;
    }    
    //printf("------------------------\n\n\n");        
}

int is_func_recorded(char * name)
{
    int i;
    for (i=0;i<func_num;i++)
    {
	if (strcmp(func_list[i].name, name)==0) 
	    return 1;
    }
    return 0;
}

void analysis_func(tree node, FILE *fout)
{
    int i;
    // check if this func is in output list, if not, add it
    char func_name[100];
    
    strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(node)));    
    if (!is_func_recorded(func_name))
    {
	strcpy(func_list[func_num].name, func_name);
	strcpy(func_list[func_num].list, "");
	func_list[func_num].is_logging = 1;
	func_num++;
    }
    // add function dummy into symtab
    strcpy(symtab[sym_len].name, func_name);    
    symtab[sym_len].ptr = DUMMY;
    symtab[sym_len].is_init = DUMMY;
    sym_len++;
    
    // add function agurments into symtab
    tree arg = DECL_ARGUMENTS(node);
    while(arg)
    {
	//type
	int tmp = 0;
	tree tmp_arg = arg;
	symtab[sym_len].is_init = 1;
	while(TREE_CODE(TREE_TYPE(tmp_arg))==POINTER_TYPE) 
	{
	    // pointer type
	    tmp++;
	    tmp_arg = TREE_TYPE(tmp_arg);    
	    symtab[sym_len].is_init = 0;
	}

// 	//name 
	strcpy(symtab[sym_len].name, IDENTIFIER_POINTER(DECL_NAME(arg)));
	sym_len++;
	arg = TREE_CHAIN(arg);
    }    
    
    // handle function statements    
    tree_stmt_iterator si; 
    tree statement_list = BIND_EXPR_BODY(DECL_SAVED_TREE(node)); 
    if (TREE_CODE(statement_list) == STATEMENT_LIST)
    {
	for(si = tsi_start(statement_list); !tsi_end_p(si);tsi_next(&si))
	{
	    unparse_stmt(tsi_stmt(si), 1, fout);   
	}
    }
    else
    {
	unparse_stmt(statement_list, 1, fout);
    }

    print_symtab();    
    
    //exit this function, clean up symtab; set function is_logging = 0;
    for (i=1;i<func_num;i++)
    {
	if (strcmp(func_list[i].name, func_name)==0)
	{
	    func_list[i].is_logging = 0;
	    break;
	}
    }
    
    while (symtab[sym_len-1].is_init!=DUMMY)
    {
	sym_len--;
    }
    sym_len--;
    
    //printf("===================\n\n\n");    
}


void unparse_global_variables(struct varpool_node *node, FILE *fout)
{
    if (node->next)
    {
	unparse_global_variables(node->next, fout);
    }
    print_var_tree(node->decl, 0, fout);
    //debug_tree(node->decl);    
}

void cs502_proj2()
{
    FILE * fout;
    fout = fopen ("output.txt","w");
    sym_len = 0;
    symtab_push("Glob");
    func_num = 0;
    strcpy(func_list[func_num].name, "Global");
    strcpy(func_list[func_num].list, "");
    func_list[func_num].is_logging = 1;
    func_num++;
    
    //global
    if (varpool_nodes)
    	unparse_global_variables(varpool_nodes, fout);
    
    //dfs starting from main
    tree node = find_func_by_name("main");
    analysis_func(node, fout);
    
    print_symtab();
    print_result(fout);
    
    fclose(fout);
}




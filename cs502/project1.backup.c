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

void print_var_tree(tree node, FILE *fout);
void unparse_expr(tree node, FILE *fout);

void unparse_call_expr(tree node, FILE *fout)
{
    //print call name 
    tree tmp = CALL_EXPR_FN(node);
    while (tmp && TREE_CODE(tmp)!=FUNCTION_DECL)
	tmp = TREE_OPERAND(tmp, 0);
    char func_name[100];
    strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(tmp)));
    fprintf(fout, "%s", func_name);
    
    //print call parameter
    fprintf(fout, "(");
    
    tree arg;
    call_expr_arg_iterator iter;
    FOR_EACH_CALL_EXPR_ARG (arg, iter, node)
    {
	unparse_expr(arg, fout);
	if (more_call_expr_args_p(&iter))
	{
	    fprintf(fout, ", ");
	}
    }
    
    fprintf(fout, ")");
}

void unparse_expr(tree node, FILE *fout)
{        
    char type_name[100];
    char str[100];
    switch(TREE_CODE(node))
    {
	case INTEGER_CST:    
	    strcpy(type_name, IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(TREE_TYPE(node)))));
	    if (strcmp(type_name,"int")==0)
		fprintf(fout, "%d", TREE_INT_CST_LOW(node));
	    
	    if (strcmp(type_name,"char")==0)
		fprintf(fout, "'%c'", TREE_INT_CST_LOW(node));
	    
	    break;
	    
	case REAL_CST: 
	    real_to_decimal (str, &TREE_REAL_CST(node), sizeof (str), 0, 1);
	    fprintf(fout, "%.8s", str);
	    break;
	    
	case CALL_EXPR:
	    unparse_call_expr(node, fout);
	    break;
	    
	default:
	    debug_tree(node);
	    break;
    }
}

void unparse_decl_expr(tree node, FILE *fout)
{
    print_var_tree(TREE_OPERAND(node, 0), fout);
}

void unparse_modify_expr(tree node, FILE *fout)
{
    
    tree left_node = TREE_OPERAND(node, 0);
    fprintf(fout, "%s = ", IDENTIFIER_POINTER(DECL_NAME(left_node)));
    tree right_node = TREE_OPERAND(node, 1);
    unparse_expr(right_node, fout);
}

void unparse_stmt(tree node, int level, FILE *fout)
{
    int i;
    for (i=0;i<4;i++) fprintf(fout, " ");
    
    printf("------------------------\n\n\n");    
    switch (TREE_CODE(node))
    {
	case CALL_EXPR:
	    unparse_call_expr(node, fout);
	    break;
	case MODIFY_EXPR:
	    unparse_modify_expr(node, fout);
	    break;
	case DECL_EXPR:
	    unparse_decl_expr(node, fout);
	    break;
	case RETURN_EXPR:
	    fprintf(fout, "return ");
	    if (TREE_OPERAND(node, 0))
	    {
		unparse_expr(TREE_OPERAND(TREE_OPERAND(node, 0),1), fout);
		
	    }
	    break;
	default:
	    break;
    }
    fprintf(fout, ";\n");
    printf("------------------------\n\n\n");    
}

void print_function_tree(tree node, FILE *fout)
{
    printf("===================\n\n\n");        
    // print function type 
    char type_name[100];
    strcpy(type_name, IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(TREE_TYPE(TREE_TYPE(node))))));
    fprintf(fout, "%s ", type_name);
    // print function name 
    char func_name[100];
    strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(node)));
    fprintf(fout, "%s", func_name);
    // print function arguments
    fprintf(fout, "(");    
    tree arg = DECL_ARGUMENTS(node);
    while(arg)
    {
	//type
	strcpy(type_name, IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(TREE_TYPE(arg)))));
	fprintf(fout, "%s ", type_name);
	//name 
	strcpy(func_name, IDENTIFIER_POINTER(DECL_NAME(arg)));
	fprintf(fout, "%s", func_name);
	
	arg = TREE_CHAIN(arg);
	if (arg) fprintf(fout, ", ");
    }    
    fprintf(fout, ")\n");    
    // print function statements    
    fprintf(fout, "{\n");    
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
    
    fprintf(fout, "}\n");    
    printf("===================\n\n\n");    
}

void print_var_tree(tree node, FILE *fout)
{
    //printf("===================\n\n\n");
    switch (TREE_CODE(node))
    {
	case VAR_DECL:
	    print_var_tree(TREE_TYPE(node), fout);
	    fprintf(fout, " ");
	    
	    print_var_tree(DECL_NAME(node), fout);
	    /*
	    if (DECL_VALUE_EXPR(node))
	    {
		//debug_tree(DECL_VALUE_EXPR(node));
		unparse_expr(TREE_OPERAND(DECL_VALUE_EXPR(node), 0),fout);
	    }
	    */
	    
	    if (DECL_INITIAL(node))
	    {
		fprintf(fout, " = ");    
		if (TREE_CODE(DECL_INITIAL(node))==REAL_CST || 
		    TREE_CODE(DECL_INITIAL(node))==INTEGER_CST)
		{
		    unparse_expr(DECL_INITIAL(node), fout);
		}
		else
		    unparse_expr(TREE_OPERAND(DECL_INITIAL(node), 0), fout);
		/*
		//debug_tree(DECL_NAME(TYPE_NAME(TREE_TYPE(node))));
		char temp[100];
		strcpy(temp, IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(TREE_TYPE(node)))));
		
		if (strcmp(temp,"float")==0)
		{
		    char str[100];
		    real_to_decimal (str, &TREE_REAL_CST(DECL_INITIAL(node)), sizeof (str), 0, 1);
		    fprintf(fout, " = %.5s", str);
		}
		
		if (strcmp(temp,"int")==0)
		{
		    fprintf(fout, " = %d", TREE_INT_CST_LOW(DECL_INITIAL(node)));
		}
		
		if (strcmp(temp,"char")==0)
		{    
		    fprintf(fout, " = '%c'", TREE_INT_CST_LOW(DECL_INITIAL(node)));
		}
		*/
	    }
	    break;
	case REAL_TYPE:    
	case INTEGER_TYPE:
	    print_var_tree(DECL_NAME(TYPE_NAME(node)), fout);   
	    break;  
	case IDENTIFIER_NODE:
	    fprintf(fout, "%s", IDENTIFIER_POINTER(node));
	    break;
	
	default:
	    break;
    }      
}

void unparse_global_variables(struct varpool_node *node, FILE *fout)
{
    if (node->next)
    {
	unparse_global_variables(node->next, fout);
    }
    print_var_tree(node->decl, fout);
    fprintf(fout, ";\n");
}

void unparse_functions(struct cgraph_node *node, FILE *fout)
{
    if (node->next)
    {
	unparse_functions(node->next, fout);
    }
    print_function_tree(node->decl, fout);
    fprintf(fout, "\n");
}

void cs502_proj1()
{
    FILE * fout;
    fout = fopen ("unparse_code.c","w");
    
    unparse_global_variables(varpool_nodes, fout);
    fprintf(fout, "\n");
    unparse_functions(cgraph_nodes, fout);
    
    fclose(fout);
}


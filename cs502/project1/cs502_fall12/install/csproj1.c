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

char struct_name[1000][100];
int struct_name_num = 0;

void print_var_tree(tree node, int level, FILE *fout);
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

void unparse_constructor_expr(tree node, FILE *fout)
{
    fprintf(fout, "{");    
    if (TREE_CODE(TREE_TYPE(node)) == ARRAY_TYPE)
    {
	//printf("=========================================\n");
	//debug_tree(TREE_TYPE(node));
	//printf("=========================================\n");
	unsigned HOST_WIDE_INT ix;
	tree field, val;
	int i = 0;
	FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (node), ix, field, val)
	{
	    if (field)
	    {
		//printf("=========== FIELD %d ============\n", i);
		//debug_tree(field);
	    }
	    
	    if (val)
	    {
		//printf("=========== VALUE %d ============\n", i);
		//debug_tree(val);
		if (i)
		    fprintf(fout, ", ");
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
	    if (field)
	    {
		//printf("=========== FIELD %d ============\n", i);
		//debug_tree(field);
	    }
	    
	    if (val)
	    {
		//printf("=========== VALUE %d ============\n", i);
		//debug_tree(val);
		if (i)
		    fprintf(fout, ", ");
		unparse_expr(val, fout);    
	    }
	    
	    i++;
	}
    }
    
    fprintf(fout, "}");
}

void unparse_indirect_ref(tree node, FILE *fout)
{
    fprintf(fout, "*");
    unparse_expr(TREE_OPERAND(node, 0), fout);
}

unparse_component_ref(tree node, FILE *fout)
{
    tree left_node = TREE_OPERAND(node, 0);
    tree right_node = TREE_OPERAND(node, 1);    
    if (TREE_CODE(left_node)==INDIRECT_REF) fprintf(fout, "(");
    unparse_expr(left_node, fout);
    if (TREE_CODE(left_node)==INDIRECT_REF) fprintf(fout, ")");
    unparse_expr(right_node, fout);
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
	    strcpy(type_name, IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(TREE_TYPE(node)))));
	    if (strcmp(type_name,"int")==0)
		fprintf(fout, "%d", TREE_INT_CST_LOW(node));
	    
	    if (strcmp(type_name,"char")==0)
		fprintf(fout, "'%c'", TREE_INT_CST_LOW(node));
	    
	    break;
	    
	case REAL_CST: 
	    real_to_decimal (str, &TREE_REAL_CST(node), sizeof (str), 0, 1);
	    fprintf(fout, "%s", str);
	    break;
	
	case STRING_CST:
	    fprintf(fout, "\"");
	    strcpy(str, TREE_STRING_POINTER(node));
	    for (i=0;i<strlen(str);i++) 
	    {
		switch(str[i])
		{
		    case '\n':
			fprintf(fout,"\\n");
			break;
		    case '\\':
			fprintf(fout,"\\\\");
			break;
		    case '\"':
			fprintf(fout,"\\\"");
			break;
		    case '\'':
			fprintf(fout,"\\\'");
			break;
		    default:
			fprintf(fout,"%c", str[i]);
			break;
		}
	    }
	    //fprintf(fout, "%s", @TREE_STRING_POINTER(node));
	    fprintf(fout, "\"");
	    break;
	
	case PARM_DECL:
	case VAR_DECL: 
	    fprintf(fout, "%s", IDENTIFIER_POINTER(DECL_NAME(node)));
	    break;
	    
	case CALL_EXPR:
	    unparse_call_expr(node, fout);
	    break;
	
	case ADDR_EXPR:
	    if (TREE_CODE(TREE_OPERAND(node, 0)) == VAR_DECL)
		fprintf(fout, "&");
	    unparse_expr(TREE_OPERAND(node, 0), fout);
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
	    if (TREE_CODE(node) == PLUS_EXPR) fprintf(fout, " + ");
	    if (TREE_CODE(node) == MINUS_EXPR) fprintf(fout, " - ");    
	    unparse_expr(right_node, fout);
	    break;
	
	case TRUNC_DIV_EXPR:
	case RDIV_EXPR:
	case MULT_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    if (TREE_CODE(left_node)==PLUS_EXPR || TREE_CODE(left_node)==MINUS_EXPR)
	    {
		fprintf(fout, "(");
		unparse_expr(left_node, fout);
		fprintf(fout, ")");
	    }
	    else 
		unparse_expr(left_node, fout);
	    
	    if (TREE_CODE(node) == MULT_EXPR) fprintf(fout, " * ");
	    if (TREE_CODE(node) == RDIV_EXPR || TREE_CODE(node) == TRUNC_DIV_EXPR) fprintf(fout, " / ");
	    
	    right_node = TREE_OPERAND(node, 1);    
	    if (TREE_CODE(right_node)==PLUS_EXPR || TREE_CODE(right_node)==MINUS_EXPR)
	    {
		fprintf(fout, "(");
		unparse_expr(right_node, fout);
		fprintf(fout, ")");
	    }
	    else
		unparse_expr(right_node, fout);
	    
	    break;
	    
	case TRUTH_NOT_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    fprintf(fout, "!");
	    if (TREE_CODE(left_node)==PARM_DECL || TREE_CODE(left_node)==VAR_DECL)
		unparse_expr(left_node, fout);
	    else
	    {
		fprintf(fout, "(");
		unparse_expr(left_node, fout);
		fprintf(fout, ")");
	    }
	    
	    break;
	    
	case TRUTH_ANDIF_EXPR:
	    left_node = TREE_OPERAND(node, 0);
	    if (TREE_CODE(left_node)==TRUTH_ORIF_EXPR)
	    {
		fprintf(fout, "(");
		unparse_expr(left_node, fout);
		fprintf(fout, ")");
	    }
	    else 
		unparse_expr(left_node, fout);
	    
	    fprintf(fout, " && ");
	    
	    right_node = TREE_OPERAND(node, 1);    
	    if (TREE_CODE(right_node)==TRUTH_ORIF_EXPR)
	    {
		fprintf(fout, "(");
		unparse_expr(right_node, fout);
		fprintf(fout, ")");
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
		fprintf(fout, "(");
		unparse_expr(left_node, fout);
		fprintf(fout, ")");
	    }
	    else 
		unparse_expr(left_node, fout);
	    
	    if (TREE_CODE(node) == GT_EXPR) fprintf(fout, ">");  
	    if (TREE_CODE(node) == LT_EXPR) fprintf(fout, "<");  
	    if (TREE_CODE(node) == GE_EXPR) fprintf(fout, ">=");  
	    if (TREE_CODE(node) == LE_EXPR) fprintf(fout, "<=");  
	    
	    right_node = TREE_OPERAND(node, 1);    
	    if (TREE_CODE(right_node)==TRUTH_ORIF_EXPR)
	    {
		fprintf(fout, "(");
		unparse_expr(right_node, fout);
		fprintf(fout, ")");
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
	    if (TREE_CODE(node) == TRUTH_ORIF_EXPR) fprintf(fout, " || ");  
	    if (TREE_CODE(node) == EQ_EXPR) fprintf(fout, " == ");  
	    if (TREE_CODE(node) == NE_EXPR) fprintf(fout, " != ");  
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
	    fprintf(fout, ".%s", IDENTIFIER_POINTER(DECL_NAME(node)));
	    break;
	case ARRAY_REF:
	    left_node = TREE_OPERAND(node, 0);
	    right_node = TREE_OPERAND(node, 1);    
	    unparse_expr(left_node, fout);
	    fprintf(fout, "[");
	    unparse_expr(right_node, fout);
	    fprintf(fout, "]");
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
    
    if (TREE_CODE(left_node) == INDIRECT_REF ||
	TREE_CODE(left_node) == COMPONENT_REF||
	TREE_CODE(left_node) == ARRAY_REF)
    {
	unparse_expr(left_node, fout);
	fprintf(fout, " = ");
    }
    else
    {
	fprintf(fout, "%s = ", IDENTIFIER_POINTER(DECL_NAME(left_node)));
    }
    
    tree right_node = TREE_OPERAND(node, 1);
    unparse_expr(right_node, fout);
}

void unparse_cond_expr(tree node, int level, FILE *fout)
{
    int i;
    //condition
    
    fprintf(fout, "if (");
    tree arg0 = TREE_OPERAND(node, 0);
    unparse_expr(arg0, fout);
    fprintf(fout, ")\n");
    //arg1
    tree arg1 = TREE_OPERAND(node, 1);
    if (TREE_CODE(arg1) == STATEMENT_LIST || TREE_CODE(arg1) == BIND_EXPR)
    {
	unparse_stmt(arg1, level, fout);
    }
    else
	unparse_stmt(arg1, level+1, fout);
    //arg2
    tree arg2 = TREE_OPERAND(node, 2);
    if (arg2)
    {
	for (i=0;i<4*level;i++) fprintf(fout, " ");
	fprintf(fout, "else\n");
	if (TREE_CODE(arg2) == STATEMENT_LIST || TREE_CODE(arg2) == BIND_EXPR)
	{
	    unparse_stmt(arg2, level, fout);
	}
	else
	    unparse_stmt(arg2, level+1, fout);
    } 
    
}

void unparse_stmt_list(tree node, int level, FILE *fout)
{
    int i;
    fprintf(fout, "{\n");
    
    tree_stmt_iterator si;    
    for(si = tsi_start(node); !tsi_end_p(si);tsi_next(&si))
    {
	unparse_stmt(tsi_stmt(si), level+1, fout);  
    }
    
    for (i=0;i<4*level;i++) fprintf(fout, " ");
    fprintf(fout, "}");   
}

void unparse_goto_expr(tree node, FILE *fout)
{
    fprintf(fout, "goto ");
    char label_name[100];
    tree label = TREE_OPERAND(node, 0);
    if (DECL_NAME(label))
    {
	strcpy(label_name, IDENTIFIER_POINTER(DECL_NAME(label)));
	fprintf(fout, "%s", label_name);
    }
    else
    {
	if (DECL_UID(label))
	{
	    fprintf(fout, "D%u", DECL_UID (label));
	}
    }
}

void unparse_label_expr(tree node, FILE *fout)
{
    tree label = TREE_OPERAND(node, 0);    
    char label_name[100];
    if (DECL_NAME(label))
    {
	strcpy(label_name, IDENTIFIER_POINTER(DECL_NAME(label)));
	fprintf(fout, "%s", label_name);
    }
    else
    {
	if (DECL_UID(label))
	{
	    fprintf(fout, "D%u", DECL_UID (label));
	}
    }
    
    fprintf(fout, ":");
}

void unparse_switch_expr(tree node, int level, FILE *fout)
{
    int i;        
    fprintf(fout, "switch (");
    unparse_expr(TREE_OPERAND(node, 0), fout);
    fprintf(fout, ")\n");    
    unparse_stmt(TREE_OPERAND(node, 1), level, fout);    
}

void unparse_case_label_expr(tree node, int level, FILE *fout)
{
    if (TREE_OPERAND(node, 0))
    {
       fprintf(fout, "case ");
       unparse_expr(TREE_OPERAND(node, 0), fout);
       fprintf(fout, ":");
    }
    else
    {
       fprintf(fout, "default:");
    }   
}

void unparse_stmt(tree node, int level, FILE *fout)
{
    int i;
    for (i=0;i<4*level;i++) fprintf(fout, " ");
    //printf("------------------------\n\n\n");    
    switch (TREE_CODE(node))
    {
	case CALL_EXPR:
	    unparse_call_expr(node, fout);
	    fprintf(fout, ";\n");
	    break;
	case MODIFY_EXPR:
	    unparse_modify_expr(node, fout);
	    fprintf(fout, ";\n");
	    break;
	case DECL_EXPR:
	    unparse_decl_expr(node, level, fout);
	    fprintf(fout, ";\n");
	    break;
	case RETURN_EXPR:
	    fprintf(fout, "return ");
	    if (TREE_OPERAND(node, 0))
	    {
		unparse_expr(TREE_OPERAND(TREE_OPERAND(node, 0),1), fout);
		
	    }
	    fprintf(fout, ";\n");
	    break;
	case COND_EXPR:
	    unparse_cond_expr(node, level, fout);    
	    break;
	case STATEMENT_LIST:
	    unparse_stmt_list(node,level,fout);
	    fprintf(fout, "\n");
	    break;
	case BIND_EXPR:
	    if (TREE_CODE(BIND_EXPR_BODY(node)) == STATEMENT_LIST)
	    {
		unparse_stmt_list(BIND_EXPR_BODY(node), level, fout);
		fprintf(fout, "\n");
	    }
	    else
	    {
		fprintf(fout, "{\n");
		unparse_stmt(BIND_EXPR_BODY(node), level+1, fout);
		for (i=0;i<4*level;i++) fprintf(fout, " ");
		fprintf(fout, "}\n");   
	    }    
	    break;
	case GOTO_EXPR:
	    unparse_goto_expr(node, fout);
	    fprintf(fout, ";\n");
	    break;
	case LABEL_EXPR:
	    unparse_label_expr(node, fout);
	    fprintf(fout, "\n");
	    break;
	case SWITCH_EXPR:
	    unparse_switch_expr(node, level, fout);
	    fprintf(fout, "\n");
	    break;
	case CASE_LABEL_EXPR:
	    unparse_case_label_expr(node, level, fout);
	    fprintf(fout, "\n");	    
	    break;
	case PREDECREMENT_EXPR:
            fprintf(fout, "--");
            unparse_expr(TREE_OPERAND(node, 0), fout);
       	    fprintf(fout, ";\n");            
	    break;
	case PREINCREMENT_EXPR:
            fprintf(fout, "++");
            unparse_expr(TREE_OPERAND(node, 0), fout);            
       	    fprintf(fout, ";\n");
	    break;	    
	case POSTDECREMENT_EXPR:
            unparse_expr(TREE_OPERAND(node, 0), fout);
            fprintf(fout, "--;\n");
	    break;
	case POSTINCREMENT_EXPR:
            unparse_expr(TREE_OPERAND(node, 0), fout);
            fprintf(fout, "++;\n");
	    break;
	default:
	    debug_tree(node);
	    break;
    }
    
    //printf("------------------------\n\n\n");    
}

void print_function_tree(tree node, FILE *fout)
{
    //printf("===================\n\n\n");        
    
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
	int tmp = 0;
	tree tmp_arg = arg;
	while(TREE_CODE(TREE_TYPE(tmp_arg))==POINTER_TYPE) 
	{
	    tmp++;
	    tmp_arg = TREE_TYPE(tmp_arg);    
	}

	strcpy(type_name, IDENTIFIER_POINTER(DECL_NAME(TYPE_NAME(TREE_TYPE(tmp_arg)))));
	fprintf(fout, "%s", type_name);
	while (tmp) 
	{
	    fprintf(fout,"*");
	    tmp--;
	}
	fprintf(fout, " ");
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
    //printf("===================\n\n\n");    
}

void print_array_type(tree node, int level, FILE *fout)
{
    tree tmp = TREE_TYPE(node);
    int len;
    
    while (TREE_CODE(tmp) == ARRAY_TYPE)
    {
          tmp = TREE_TYPE(tmp);
    }
    print_var_tree(tmp, level, fout);
    fprintf(fout, " ");
    
    print_var_tree(DECL_NAME(node), level, fout);

    for (tmp = TREE_TYPE(node); TREE_CODE (tmp) == ARRAY_TYPE; tmp = TREE_TYPE (tmp))
    {
         if (TYPE_MAX_VALUE(TYPE_DOMAIN(tmp)))
         {
              len = TREE_INT_CST_LOW(TYPE_MAX_VALUE(TYPE_DOMAIN(tmp)));
              fprintf(fout, "[%d]",len+1);
         }
    }
    
    if (DECL_INITIAL(node))
    {
	fprintf(fout, " = ");    
	unparse_expr(DECL_INITIAL(node), fout);
    }
}

bool is_struct_decl(char *tmpstr)
{
    int i;
    for (i=0;i<struct_name_num;i++)
    {
	if (strcmp(struct_name[i], tmpstr)==0)
	    return true;
    }
    return false;
}

void print_struct_decl(tree node, int level, FILE *fout)
{
    int i;
    
    fprintf(fout, "struct %s {\n", IDENTIFIER_POINTER(TYPE_NAME(node)));
    
    //print decl var
    tree tmp;
    tmp = TYPE_FIELDS (node);
    while (tmp)
    {
	for (i=0;i<4*(level+1);i++) fprintf(fout, " ");
	print_var_tree(tmp, level+1, fout);
	fprintf(fout,";\n");
	tmp = DECL_CHAIN (tmp);
    }
    
    for (i=0;i<4*level;i++) fprintf(fout, " ");
    fprintf(fout, "};\n");
    for (i=0;i<4*level;i++) fprintf(fout, " ");    
}

void print_var_tree(tree node, int level, FILE *fout)
{
    int tmp;
    char* tmpstr;
    //printf("===================\n\n\n");
    switch (TREE_CODE(node))
    {
	case FIELD_DECL:
	case VAR_DECL:
	    if (TREE_CODE(TREE_TYPE(node)) == ARRAY_TYPE) 
    	    {
          	print_array_type(node, level, fout);
    	        break;
	    }
	    print_var_tree(TREE_TYPE(node), level, fout);
	    fprintf(fout, " ");
	    
	    print_var_tree(DECL_NAME(node), level, fout);
	    
	    if (DECL_INITIAL(node))
	    {
		fprintf(fout, " = ");    
		if (TREE_CODE(DECL_INITIAL(node))==REAL_CST || 
		    TREE_CODE(DECL_INITIAL(node))==INTEGER_CST ||
		    TREE_CODE(DECL_INITIAL(node))==CONSTRUCTOR)
		{
		    unparse_expr(DECL_INITIAL(node), fout);
		}
		else
		    unparse_expr(TREE_OPERAND(DECL_INITIAL(node), 0), fout);
	    }
	    break;
	case REAL_TYPE:    
	case INTEGER_TYPE:
	    print_var_tree(DECL_NAME(TYPE_NAME(node)), level, fout);   
	    break;  
	case IDENTIFIER_NODE:
	    fprintf(fout, "%s", IDENTIFIER_POINTER(node));
	    break;
	case RECORD_TYPE:
	    tmpstr = IDENTIFIER_POINTER(TYPE_NAME(node));
	    if (!is_struct_decl(tmpstr))
	    {
		strcpy(struct_name[struct_name_num], tmpstr);
		struct_name_num++;
		print_struct_decl(node, level ,fout);
	    }
	    
	    fprintf(fout, "struct ");    
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
	    //debug_tree(DECL_NAME(TYPE_NAME(tmp_arg)));    
	    print_var_tree(tmp_arg, level, fout);    
	    while (tmp) 
	    {
		fprintf(fout,"*");
		tmp--;
	    }
	    break;
	default:    
	    debug_tree(node);
	    break;
    }      
}

void unparse_global_variables(struct varpool_node *node, FILE *fout)
{
    if (node->next)
    {
	unparse_global_variables(node->next, fout);
    }
    print_var_tree(node->decl, 0, fout);
    //debug_tree(node->decl);
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
    
    if (varpool_nodes)
    	unparse_global_variables(varpool_nodes, fout);
    
    fprintf(fout, "\n");

    if (cgraph_nodes)
    	unparse_functions(cgraph_nodes, fout);
    
    fclose(fout);
}


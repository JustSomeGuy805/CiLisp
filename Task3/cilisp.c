
#include "cilisp.h"

int main(void)
{
    yyparse();
    return 0;
}

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}

// find out which function it is

int resolveFunc(char *func) 
{
    //rearranged to group funcs with one param in the beginning, and funcs with two params at the end   
    char *funcs[] = {"neg", "abs", "exp", "exp2", "sqrt", "cbrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "hypot", ""};

    int i = 0;
    while (funcs[i][0] != '\0') {
        if (!strcmp(funcs[i], func))
            return i;

        i++;
    }
    yyerror("invalid function"); // paranoic -- should never happen
    return -1;
}

// create a node for a number
AST_NODE *number(double value)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(NUMBER_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = NUM_TYPE;
    p->data.number.value = value;

    return p;
}

// create a node for a function
AST_NODE *function(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = FUNC_TYPE;
    p->data.function.name = funcName;
    p->data.function.op1 = op1;
    p->data.function.op2 = op2;

    return p;
}

// free a node
void freeNode(AST_NODE *p)
{
    if (!p)
       return;
       
    if (p->type == FUNC_TYPE)
    {
        free(p->data.function.name);
        freeNode(p->data.function.op1);
        freeNode(p->data.function.op2);
    }
        
    free (p);
}


void translate(AST_NODE *p)
{
    if(p == NULL)
        return;
      
    if (p->type == FUNC_TYPE) 
    {
        int funcID = resolveFunc(p->data.function.name);

        //+ - * /
        if (funcID >= ADD && funcID <= DIV)  //if an arithmetic
        {        
            char op = '?';
            switch (funcID) 
            {
                case ADD:
                    op = '+';
                    break;
                case SUB:
                    op = '-';
                    break;
                case MULT:
                    op = '*';
                    break;
                case DIV:
                    op = '/';
                    break;             
            }

            //-----prints '( left_operand operator right_operand )'-----
            printf("( ");
            translate(p->data.function.op1);    //print left_operand
            printf(" %c ", op );                //print operator
            translate(p->data.function.op2);    //print right_operand
            printf(" )");      
        }
        
        else //non-arithmetic
        {
            //-----prints 'function_name( param1[, param2] )'  -----
            printf("%s( ", p->data.function.name);
            translate(p->data.function.op1);        //print first param
                     
            if(p->data.function.op2 != NULL)        //print second param if any
            {
                printf(", ");
                translate(p->data.function.op2);
            }
         
            printf(" )");           
        }
    } 
    else //node type is NUMBER 
    {
        //simply print the value 
        printf("%f", p->data.number.value);      
    }

}  


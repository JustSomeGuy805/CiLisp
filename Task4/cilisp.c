
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


void eval(AST_NODE *p)
{
    //printf("eval\n");
    if (p == NULL || p->type == NUM_TYPE)
    {
       // printf("null or num type\n");
        return;
    }
    
    AST_NODE *left = p->data.function.op1;
    
    eval(left);

    int funcID = resolveFunc(p->data.function.name);

    //functions with only one parameter
    if(funcID <= 5)
    {     
        switch(funcID)
        {
            case NEG:
                p->data.number.value = left->data.number.value * -1;
                break;
            case ABS:
                p->data.number.value = fabs(left->data.number.value);
                break;
            case EXP:
                 p->data.number.value = exp(left->data.number.value);
                break;
            case EXP2:
                p->data.number.value = exp2(left->data.number.value);
                break;
            case SQRT:
                 p->data.number.value = sqrt(left->data.number.value);
                break;
            case CBRT:
                 p->data.number.value = cbrt(left->data.number.value);
                break;
        }
    }
    else ///functions with two parameters
    {
        AST_NODE *right = p->data.function.op2;

        eval(right);
        
        switch(funcID)
        {
            case ADD:
                p->data.number.value = left->data.number.value + right->data.number.value;
                break;
            case SUB:
                p->data.number.value = left->data.number.value - right->data.number.value;
                break;
            case MULT:
                p->data.number.value = left->data.number.value * right->data.number.value;
                break;
            case DIV:
                p->data.number.value = left->data.number.value / right->data.number.value;
                break;
            case REM:
                 p->data.number.value = fmod(left->data.number.value, right->data.number.value);
                break;
            case LOG:
                 p->data.number.value = log(right->data.number.value) / log(left->data.number.value);
                break;
            case POW:
                p->data.number.value = pow(left->data.number.value, right->data.number.value);
                break;
            case MAX:
                p->data.number.value = (left->data.number.value >= right->data.number.value) ? left->data.number.value : right->data.number.value;
                break;
            case MIN:
                p->data.number.value = (left->data.number.value <= right->data.number.value) ? left->data.number.value : right->data.number.value;
                break;
            case HYPOT:
                p->data.number.value = hypot(left->data.number.value,right->data.number.value);
                break;
        }
    }

  
    p->type = NUM_TYPE;

}  


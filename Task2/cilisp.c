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

int resolveFunc(char *func)
{
   char *funcs[] = { "neg", "abs", "exp", "sqrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "exp2", "cbrt", "hypot",""};
   
   int i = 0;
   while (funcs[i][0] !='\0')
   {
      if (!strcmp(funcs[i], func))
         return i;
         
      i++;
   }
   yyerror("invalid function"); // paranoic -- should never happen
   return -1;
}

double calc(char *func, double op1, double op2)
{

    double val = 0;
    int funcID = resolveFunc(func);
    
    switch(funcID)
    {
        case 0: //neg
            val = op1 * -1;
            break;
        
        case 1: //abs
            val = fabs(op1);
            break;
        
        case 2: //exp
            val = exp(op1);
            break;
        
        case 3: //sqrt
            val = sqrt(op1);
            break;
        
        case 4: //add
            val = op1 + op2;
            break;
        
        case 5: //sub
            val = op1 - op2;
            break;
        
        case 6: //mult
            val = op1 * op2;
            break;
        
        case 7: //div
            val = op1 / op2;
            break; 
        
        case 8: //remain
            val = fmod(op1, op2);
            break;
        
        case 9: //log
            val = log(op2) / log(op1);
            break;
        
        case 10: //pow   
            val = pow(op1, op2);
            break;
        
        case 11: //max
            val = (op1 >= op2) ? op1 : op2;
            break;
        
        case 12: //min
            val = (op1 <= op2) ? op1 : op2;
            break;   
       
        case 13: //exp2       
            val = exp2(op1);
            break;
        
        case 14: //cbrt
            
            val = cbrt(op1);
            break;
        
        case 15: //hypot        
            val = hypot(op1,op2);
            break;
    }

   return val;

}  

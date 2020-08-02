/*
 * Copyright (c) 2012 the authors listed at the following URL, and/or
 * the authors of referenced articles or incorporated external code:
 * http://en.literateprograms.org/Shunting_yard_algorithm_(C)?action=history&offset=20080201043325
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *  
 * Retrieved from: http://en.literateprograms.org/Shunting_yard_algorithm_(C)?oldid=12454
 * 
 * 
 * Copyright (c) 2019 Mohammed Isam [mohammed_isam1984@yahoo.com]
 * 
 * Extensive modifications have been applied to this file to include most of the C
 * language operators and to make this file usable as part of the Layla shell.
 * Please compare with the original file at the above link to see the differences.
 * 
 * UPDATE: the original file doesn't seem to be available anymore, but the archived
 * version can be accessed from here:
 * https://web.archive.org/web/20110718214204/http://en.literateprograms.org/Shunting_yard_algorithm_(C)
 * 
 * For more information, see: https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "shell.h"
#include "symtab/symtab.h"

#define MAXOPSTACK          64
#define MAXNUMSTACK         64
#define MAXBASE             36

struct stack_item_s
{
#define ITEM_LONG_INT       1
#define ITEM_VAR_PTR        2
    int  type;
    union
    {
        long val;
        struct symtab_entry_s *ptr;
    };
};

struct op_s *opstack[MAXOPSTACK];
int    nopstack  = 0;

struct stack_item_s numstack[MAXNUMSTACK];
int    nnumstack = 0;
int    error     = 0;


long long_value(struct stack_item_s *a)
{
    if(a->type == ITEM_LONG_INT)
    {
        return a->val;
    }
    else if(a->type == ITEM_VAR_PTR)
    {
        if(a->ptr->val)
        {
            return atol(a->ptr->val);
        }
    }
    return 0;
}

long eval_uminus (struct stack_item_s *a1, struct stack_item_s *a2 __attribute__ ((unused)) )
{
    return -long_value(a1);
}

long eval_uplus  (struct stack_item_s *a1, struct stack_item_s *a2 __attribute__ ((unused)) )
{
    return  long_value(a1);
}

long eval_lognot(struct stack_item_s *a1, struct stack_item_s *a2 __attribute__ ((unused)) )
{
    return !long_value(a1);
}

long eval_bitnot(struct stack_item_s *a1, struct stack_item_s *a2 __attribute__ ((unused)) )
{
    return ~long_value(a1);
}

long eval_mult(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) * long_value(a2);
}

long eval_add(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) + long_value(a2);
}

long eval_sub(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) - long_value(a2);
}

long eval_lsh(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) << long_value(a2);
}

long eval_rsh(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) >> long_value(a2);
}

long eval_lt(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) < long_value(a2);
}

long eval_le(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) <= long_value(a2);
}

long eval_gt(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) > long_value(a2);
}

long eval_ge(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) >= long_value(a2);
}

long eval_eq(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) == long_value(a2);
}

long eval_ne(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) != long_value(a2);
}

long eval_bitand(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) & long_value(a2);
}

long eval_bitxor(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) ^ long_value(a2);
}

long eval_bitor(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) | long_value(a2);
}

long eval_logand(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) && long_value(a2);
}

long eval_logor(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return long_value(a1) || long_value(a2);
}

long do_eval_exp(long a1, long a2)
{
    return a2 < 0 ? 0 : (a2 == 0 ? 1 : a1 * do_eval_exp(a1, a2-1));
}

long eval_exp(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_exp(long_value(a1), long_value(a2));
}

long eval_div(struct stack_item_s *a1, struct stack_item_s *a2) 
{
    error = 0;
    long n2 = long_value(a2);
    if(!n2)
    {
        fprintf(stderr, "error: Division by zero\n");
        error = 1;
        return 0;
    }
    return long_value(a1) / n2;
}

long eval_mod(struct stack_item_s *a1, struct stack_item_s *a2) 
{
    error = 0;
    long n2 = long_value(a2);
    if(!n2)
    {
        fprintf(stderr, "error: Division by zero\n");
        error = 1;
        return 0;
    }
    return long_value(a1) % n2;
}

long eval_assign(struct stack_item_s *a1, struct stack_item_s *a2)
{
    long val = long_value(a2);
    if(a1->type == ITEM_VAR_PTR)
    {
        char buf[16];
        sprintf(buf, "%ld", val);
        symtab_entry_setval(a1->ptr, buf);
    }
    return val;
}

long do_eval_assign_ext(long (*f)(struct stack_item_s *a1, struct stack_item_s *a2),
            struct stack_item_s *a1, struct stack_item_s *a2)
{
    long val = f(a1, a2);
    if(a1->type == ITEM_VAR_PTR)
    {
        char buf[16];
        sprintf(buf, "%ld", val);
        symtab_entry_setval(a1->ptr, buf);
    }
    return val;
}

long eval_assign_add(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_add, a1, a2);
}

long eval_assign_sub(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_sub, a1, a2);
}

long eval_assign_mult(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_mult, a1, a2);
}

long eval_assign_div(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_div, a1, a2);
}

long eval_assign_mod(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_mod, a1, a2);
}

long eval_assign_lsh(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_lsh, a1, a2);
}

long eval_assign_rsh(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_rsh, a1, a2);
}

long eval_assign_and(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_bitand, a1, a2);
}

long eval_assign_xor(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_bitxor, a1, a2);
}

long eval_assign_or(struct stack_item_s *a1, struct stack_item_s *a2)
{
    return do_eval_assign_ext(eval_bitor, a1, a2);
}

long do_eval_inc_dec(int pre, int add, struct stack_item_s *a1)
{
    long val = long_value(a1);
    char buf[16];
    if(pre)
    {
        if(add)
        {
            val++;
        }
        else
        {
            val--;
        }
        
        if(a1->type == ITEM_VAR_PTR)
        {
            sprintf(buf, "%ld", val);
            symtab_entry_setval(a1->ptr, buf);
        }
    }
    else
    {
        int diff = add ? 1 : -1;
        if(a1->type == ITEM_VAR_PTR)
        {
            sprintf(buf, "%ld", val+diff);
            symtab_entry_setval(a1->ptr, buf);
        }
    }
    return val;
}

long eval_postinc(struct stack_item_s *a1, struct stack_item_s *unused __attribute__((unused)))
{
    return do_eval_inc_dec(0, 1, a1);
}

long eval_postdec(struct stack_item_s *a1, struct stack_item_s *unused __attribute__((unused)))
{
    return do_eval_inc_dec(0, 0, a1);
}

long eval_preinc(struct stack_item_s *a1, struct stack_item_s *unused __attribute__((unused)))
{
    return do_eval_inc_dec(1, 1, a1);
}

long eval_predec(struct stack_item_s *a1, struct stack_item_s *unused __attribute__((unused)))
{
    return do_eval_inc_dec(1, 0, a1);
}


/* extended operator list */
#define CH_GT                2       /* greater than */
#define CH_LT                3       /* lesser than */
#define CH_GE                4       /* greater than or equals */
#define CH_LE                5       /* lesser than or equals */
#define CH_RSH               6       /* shift right */
#define CH_LSH               7       /* shitf left */
#define CH_NE                8       /* not equals */
#define CH_EQ                9       /* equals */
#define CH_ASSIGN            10      /* assignment */
#define CH_PRE_INC           11      /* pre-increment op */
#define CH_POST_INC          12      /* post-increment op */
#define CH_PRE_DEC           13      /* pre-decrement op */
#define CH_POST_DEC          14      /* post-decrement op */
#define CH_B_AND             15      /* bitwise AND */
#define CH_B_OR              16      /* bitwise OR */
#define CH_B_XOR             17      /* bitwise XOR */
#define CH_AND               18      /* logical AND */
#define CH_OR                19      /* logical OR */
#define CH_EXP               20      /* exponent or ** */
#define CH_MINUS             21      /* unary minus */
#define CH_PLUS              22      /* unary plus */
#define CH_ASSIGN_PLUS       23      /* += assignment */
#define CH_ASSIGN_MINUS      24      /* -= assignment */
#define CH_ASSIGN_MULT       25      /* *= assignment */
#define CH_ASSIGN_DIV        26      /* /= assignment */
#define CH_ASSIGN_MOD        27      /* %= assignment */
#define CH_ASSIGN_LSH        28      /* <<= assignment */
#define CH_ASSIGN_RSH        29      /* >>= assignment */
#define CH_ASSIGN_AND        30      /* &= assignment */
#define CH_ASSIGN_XOR        31      /* ^= assignment */
#define CH_ASSIGN_OR         32      /* |= assignment */

enum { ASSOC_NONE = 0, ASSOC_LEFT, ASSOC_RIGHT };

/*
 * see this link for C operator precedence:
 * https://en.cppreference.com/w/c/language/operator_precedence
 */

struct op_s
{
    char op;
    int  prec;
    int  assoc;
    char unary;
    char chars;
    long (*eval)(struct stack_item_s *a1, struct stack_item_s *a2);
} arithm_ops[] =
{
    { CH_POST_INC     , 20, ASSOC_LEFT , 1, 2, eval_postinc       },
    { CH_POST_DEC     , 20, ASSOC_LEFT , 1, 2, eval_postdec       },
    { CH_PRE_INC      , 19, ASSOC_RIGHT, 1, 2, eval_postinc       },
    { CH_PRE_DEC      , 19, ASSOC_RIGHT, 1, 2, eval_postdec       },
    { CH_MINUS        , 19, ASSOC_RIGHT, 1, 1, eval_uminus        },
    { CH_PLUS         , 19, ASSOC_RIGHT, 1, 1, eval_uplus         },
    { '!'             , 19, ASSOC_RIGHT, 1, 1, eval_lognot        },
    { '~'             , 19, ASSOC_RIGHT, 1, 1, eval_bitnot        },
    { CH_EXP          , 18, ASSOC_RIGHT, 0, 2, eval_exp           },
    { '*'             , 17, ASSOC_LEFT , 0, 1, eval_mult          },
    { '/'             , 17, ASSOC_LEFT , 0, 1, eval_div           },
    { '%'             , 17, ASSOC_LEFT , 0, 1, eval_mod           },
    { '+'             , 16, ASSOC_LEFT , 0, 1, eval_add           },
    { '-'             , 16, ASSOC_LEFT , 0, 1, eval_sub           },
    { CH_LSH          , 15, ASSOC_LEFT , 0, 2, eval_lsh           },
    { CH_RSH          , 15, ASSOC_LEFT , 0, 2, eval_rsh           },
    { '<'             , 14, ASSOC_LEFT , 0, 1, eval_lt            },
    { CH_LE           , 14, ASSOC_LEFT , 0, 2, eval_le            },
    { '>'             , 14, ASSOC_LEFT , 0, 1, eval_gt            },
    { CH_GE           , 14, ASSOC_LEFT , 0, 2, eval_ge            },
    { CH_EQ           , 13, ASSOC_LEFT , 0, 2, eval_eq            },
    { CH_NE           , 13, ASSOC_LEFT , 0, 2, eval_ne            },
    { '&'             , 12, ASSOC_LEFT , 0, 1, eval_bitand        },
    { '^'             , 11, ASSOC_LEFT , 0, 1, eval_bitxor        },
    { '|'             , 10, ASSOC_LEFT , 0, 1, eval_bitor         },
    { CH_AND          , 9 , ASSOC_LEFT , 0, 2, eval_logand        },
    { CH_OR           , 8 , ASSOC_LEFT , 0, 2, eval_logor         },
    { CH_ASSIGN       , 7 , ASSOC_RIGHT, 0, 1, eval_assign        },
    { CH_ASSIGN_PLUS  , 7 , ASSOC_RIGHT, 0, 2, eval_assign_add    },
    { CH_ASSIGN_MINUS , 7 , ASSOC_RIGHT, 0, 2, eval_assign_sub    },
    { CH_ASSIGN_MULT  , 7 , ASSOC_RIGHT, 0, 2, eval_assign_mult   },
    { CH_ASSIGN_DIV   , 7 , ASSOC_RIGHT, 0, 2, eval_assign_div    },
    { CH_ASSIGN_MOD   , 7 , ASSOC_RIGHT, 0, 2, eval_assign_mod    },
    { CH_ASSIGN_LSH   , 7 , ASSOC_RIGHT, 0, 3, eval_assign_lsh    },
    { CH_ASSIGN_RSH   , 7 , ASSOC_RIGHT, 0, 3, eval_assign_rsh    },
    { CH_ASSIGN_AND   , 7 , ASSOC_RIGHT, 0, 2, eval_assign_and    },
    { CH_ASSIGN_XOR   , 7 , ASSOC_RIGHT, 0, 2, eval_assign_xor    },
    { CH_ASSIGN_OR    , 7 , ASSOC_RIGHT, 0, 2, eval_assign_or     },

    /*
     * TODO: add the comma ',' and ternary '?:' operators.
     */

    { '('             , 0 , ASSOC_NONE , 0, 1, NULL               },
    { ')'             , 0 , ASSOC_NONE , 0, 1, NULL               }
};

struct op_s *OP_POST_INC     = &arithm_ops[0 ];
struct op_s *OP_POST_DEC     = &arithm_ops[1 ];
struct op_s *OP_PRE_INC      = &arithm_ops[2 ];
struct op_s *OP_PRE_DEC      = &arithm_ops[3 ];
struct op_s *OP_UMINUS       = &arithm_ops[4 ];
struct op_s *OP_UPLUS        = &arithm_ops[5 ];
struct op_s *OP_LOG_NOT      = &arithm_ops[6 ];
struct op_s *OP_BIT_NOT      = &arithm_ops[7 ];
struct op_s *OP_EXP          = &arithm_ops[8 ];
struct op_s *OP_MULT         = &arithm_ops[9 ];
struct op_s *OP_DIV          = &arithm_ops[10];
struct op_s *OP_MOD          = &arithm_ops[11];
struct op_s *OP_ADD          = &arithm_ops[12];
struct op_s *OP_SUB          = &arithm_ops[13];
struct op_s *OP_LSH          = &arithm_ops[14];
struct op_s *OP_RSH          = &arithm_ops[15];
struct op_s *OP_LT           = &arithm_ops[16];
struct op_s *OP_LE           = &arithm_ops[17];
struct op_s *OP_GT           = &arithm_ops[18];
struct op_s *OP_GE           = &arithm_ops[19];
struct op_s *OP_EQ           = &arithm_ops[20];
struct op_s *OP_NE           = &arithm_ops[21];
struct op_s *OP_BIT_AND      = &arithm_ops[22];
struct op_s *OP_BIT_XOR      = &arithm_ops[23];
struct op_s *OP_BIT_OR       = &arithm_ops[24];
struct op_s *OP_LOG_AND      = &arithm_ops[25];
struct op_s *OP_LOG_OR       = &arithm_ops[26];
struct op_s *OP_ASSIGN       = &arithm_ops[27];
struct op_s *OP_ASSIGN_ADD   = &arithm_ops[28];
struct op_s *OP_ASSIGN_SUB   = &arithm_ops[29];
struct op_s *OP_ASSIGN_MULT  = &arithm_ops[30];
struct op_s *OP_ASSIGN_DIV   = &arithm_ops[31];
struct op_s *OP_ASSIGN_MOD   = &arithm_ops[32];
struct op_s *OP_ASSIGN_LSH   = &arithm_ops[33];
struct op_s *OP_ASSIGN_RSH   = &arithm_ops[34];
struct op_s *OP_ASSIGN_AND   = &arithm_ops[35];
struct op_s *OP_ASSIGN_XOR   = &arithm_ops[36];
struct op_s *OP_ASSIGN_OR    = &arithm_ops[37];
struct op_s *OP_LBRACE       = &arithm_ops[38];
struct op_s *OP_RBRACE       = &arithm_ops[39];


/*
 * return 1 if the given char is a valid shell variable name char.
 */
int valid_name_char(char c)
{
    switch(c)
    {
        case '_':
        case '@':
        case '#':
        case '$':
        case '?':
            return 1;

        default:
            if(isalnum(c))
            {
                return 1;
            }
            return 0;
    }
}


/*
 * extract an arithmetic operator from the beginning of expr.
 */
struct op_s *get_op(char *expr)
{
    switch(*expr)
    {
        case '+':
            if(expr[1] == '+')
            {
                return OP_POST_INC;
            }
            else if(expr[1] == '=')
            {
                return OP_ASSIGN_ADD;
            }
            return OP_ADD;

        case '-':
            if(expr[1] == '-')
            {
                return OP_POST_DEC;
            }
            else if(expr[1] == '=')
            {
                return OP_ASSIGN_SUB;
            }
            return OP_SUB;

        case '*':
            if(expr[1] == '*')
            {
                return OP_EXP;
            }
            else if(expr[1] == '=')
            {
                return OP_ASSIGN_MULT;
            }
            return OP_MULT;

        case '<':
            if(expr[1] == '<')
            {
                if(expr[2] == '=')
                {
                    return OP_ASSIGN_LSH;
                }
                return OP_LSH;
            }
            else if(expr[1] == '=')
            {
                return OP_LE;
            }
            return OP_LT;

        case '>':
            if(expr[1] == '>')
            {
                if(expr[2] == '=')
                {
                    return OP_ASSIGN_RSH;
                }
                return OP_RSH;
            }
            else if(expr[1] == '=')
            {
                return OP_GE;
            }
            return OP_GT;

        case '!':
            if(expr[1] == '=')
            {
                return OP_NE;
            }
            return OP_LOG_NOT;

        case '=':
            if(expr[1] == '=')
            {
                return OP_EQ;
            }
            return OP_ASSIGN;

        case '&':
            if(expr[1] == '&')
            {
                return OP_LOG_AND;
            }
            else if(expr[1] == '=')
            {
                return OP_ASSIGN_AND;
            }
            return OP_BIT_AND;

        case '|':
            if(expr[1] == '|')
            {
                return OP_LOG_OR;
            }
            else if(expr[1] == '=')
            {
                return OP_ASSIGN_OR;
            }
            return OP_BIT_OR;

        case '^':
            if(expr[1] == '=')
            {
                return OP_ASSIGN_XOR;
            }
            return OP_BIT_XOR;
        case '/':
            if(expr[1] == '=')
            {
                return OP_ASSIGN_DIV;
            }
            return OP_DIV;
            
        case '%':
            if(expr[1] == '=')
            {
                return OP_ASSIGN_MOD;
            }
            return OP_MOD;

        case '~':
            return OP_BIT_NOT;
            
        case '(':
            return OP_LBRACE ;
            
        case ')':
            return OP_RBRACE ;
    }
    return NULL;
}


/*
 * push an operator on the operator stack.
 */
void push_opstack(struct op_s *op)
{
    if(nopstack>MAXOPSTACK-1)
    {
        fprintf(stderr, "error: Operator stack overflow\n");
        error = 1;
        return;
    }
    opstack[nopstack++]=op;
}


/*
 * pop an operator from the operator stack.
 */
struct op_s *pop_opstack(void)
{
    if(!nopstack)
    {
        fprintf(stderr, "error: Operator stack empty\n");
        error = 1;
        return NULL;
    }
    return opstack[--nopstack];
}


/*
 * push a long numeric operand on the operand stack.
 */
void push_numstackl(long val)
{
    if(nnumstack > MAXNUMSTACK-1)
    {
        fprintf(stderr, "error: Number stack overflow\n");
        error = 1;
        return;
    }

    numstack[nnumstack].type = ITEM_LONG_INT;
    numstack[nnumstack++].val = val;
}


/*
 * push a shell variable operand on the operand stack.
 */
void push_numstackv(struct symtab_entry_s *val)
{
    if(nnumstack > MAXNUMSTACK-1)
    {
        fprintf(stderr, "error: Number stack overflow\n");
        error = 1;
        return;
    }

    numstack[nnumstack].type = ITEM_VAR_PTR;
    numstack[nnumstack++].ptr = val;
}


/*
 * pop an operand from the operand stack.
 */
struct stack_item_s pop_numstack(void)
{
    if(!nnumstack)
    {
        fprintf(stderr, "error: Number stack empty\n");
        error = 1;
        return (struct stack_item_s) { };
    }
    return numstack[--nnumstack];
}


/*
 * perform operator shunting when we have a new operator by popping the operator
 * at the top of the stack and applying it to the operands on the operand stack.
 * we do this if the operator on top of the stack is not a '(' operator and:
 *   - has greater precedence than the new operator, or
 *   - has equal precedence to the new operator, but the top-of-stack one is
 *     left-associative
 * after popping the operator, we push the new operator on the operator stack,
 * and we push the previous top-of-stack operator's result on the operand stack.
 */
void shunt_op(struct op_s *op)
{
    struct op_s *pop;
    error = 0;
    if(op->op == '(')
    {
        push_opstack(op);
        return;
    }
    else if(op->op == ')')
    {
        while(nopstack > 0 && opstack[nopstack-1]->op != '(')
        {
            pop = pop_opstack();
            if(error)
            {
                return;
            }
            struct stack_item_s n1 = pop_numstack();
            if(error)
            {
                return;
            }
            if(pop->unary)
            {
                push_numstackl(pop->eval(&n1, 0));
            }
            else
            {
                struct stack_item_s n2 = pop_numstack();
                if(error)
                {
                    return;
                }
                push_numstackl(pop->eval(&n2, &n1));
                if(error)
                {
                    return;
                }
            }
        }
        if(!(pop = pop_opstack()) || pop->op != '(')
        {
            fprintf(stderr, "error: Stack error. No matching \'(\'\n");
            error = 1;
        }
        return;
    }

    if(op->assoc == ASSOC_RIGHT)
    {
        while(nopstack && op->prec < opstack[nopstack-1]->prec)
        {
            pop = pop_opstack();
            if(error)
            {
                return;
            }
            struct stack_item_s n1 = pop_numstack();
            if(pop->unary)
            {
                push_numstackl(pop->eval(&n1, 0));
            }
            else
            {
                struct stack_item_s n2 = pop_numstack();
                if(error)
                {
                    return;
                }
                push_numstackl(pop->eval(&n2, &n1));
            }
            if(error)
            {
                return;
            }
        }
    }
    else
    {
        while(nopstack && op->prec <= opstack[nopstack-1]->prec)
        {
            pop = pop_opstack();
            if(error)
            {
                return;
            }
            struct stack_item_s n1 = pop_numstack();
            if(pop->unary)
            {
                push_numstackl(pop->eval(&n1, 0));
            }
            else
            {
                struct stack_item_s n2 = pop_numstack();
                if(error)
                {
                    return;
                }
                push_numstackl(pop->eval(&n2, &n1));
            }
            if(error)
            {
                return;
            }
        }
    }
    push_opstack(op);
}


/*
 * check if the given digit falls in the range [0]..[base-1], then return
 * the numeric value of that digit.
 * the base can be any number from 2 to 64, with values higher than 9
 * represented by the letters a-z, then A-Z, then @ and _ (similar to bash).
 * if the base is <= 36, small and capital letters can be used interchangeably.
 * the result is place in the *result field, and 1 is returned.. otherwise
 * zero is returned.
 */
int get_ndigit(char c, int base, int *result)
{
    /* invalid char */
    if(!isalnum(c) && c != '@' && c != '_')
    {
        return 0;
    }
    
    char max, max2;
    /* base 10 or less: only digits 0-9 are acceptable */
    if(base <= 10)
    {
        max = '0'+base-1;
        if(c >= '0' && c <= max)
        {
            (*result) = c-'0';
            return 1;
        }
        goto invalid;
    }

    /* base larger than 10: if the digit is 0-9, return it */
    if(c >= '0' && c <= '9')
    {
        (*result) = c-'0';
        return 1;
    }

    /* bases 11 to 36: small and capital letters can be used interchangeably */
    if(base <= 36)
    {
        max  = 'a'+base-11;     /* max. small letter in this base */
        max2 = max-('a'-'A');   /* max capital letter in this base */
        if(c >= 'a' && c <= max)
        {
            (*result) = c-'a'+10;
            return 1;
        }
        if(c >= 'A' && c <= max2)
        {
            (*result) = c-'A'+10;
            return 1;
        }
    }

    /*
     *  bases 37 to 64: the following characters represent the corresponding digits:
     *     a-z => 10-35
     *     A-Z => 36-61
     *     @   => 62
     *     _   => 63
     */
    else if(base <= 62)
    {
        /* check the small letters first */
        if(c >= 'a' && c <= 'z')
        {
            (*result) = c-'a'+10;
            return 1;
        }
        max2 = 'A'+base-37;          /* max capital letter in this base */
        if(c >= 'A' && c <= max2)
        {
            (*result) = c-'A'+36;
            return 1;
        }
    }
    else if(c == '@')
    {
        (*result) = 62;
        return 1;
    }
    else if(c == '_' && base == 64)
    {
        (*result) = 63;
        return 1;
    }

invalid:
    /* invalid digit */
    fprintf(stderr, "error: digit %c exceeds the value of the base %d\n", c, base);
    error = 1;
    return 0;
}


/*
 * extract a numeric operand from the beginning of the given string.
 * numbers can be hex constants (preceded by 0x or 0X), octal (preceded by 0),
 * binary (preceded by 0b or 0B), or in any base, given in the format: [base#]n.
 * the number of characters used to get the number is stored in *char_count,
 * while the number itself is return as a long int.
 */
long get_num(char *s, int *char_count)
{
    char *s2 = s;
    long num = 0;
    int num2, base = 10;

    /* check if we have a predefined base */
    if(*s2 == '0')
    {
        switch(s2[1])
        {
            case 'x':
            case 'X':
                base = 16;
                s2 += 2;
                break;

            case 'b':
            case 'B':
                base = 2;
                s2 += 2;
                break;
                
            default:
                base = 8;
                s2++;
                break;
        }
    }

    /* get the number according to the given base (use base 10 if none) */
    while(get_ndigit(*s2, base, &num2))
    {
        num = (num*base) + num2;
        s2++;
    }

    /* check we didn't encounter an invalid digit */
    if(error)
    {
        return 0;
    }

    /* if binary, octal or hex base, return the result */
    if(base != 10)
    {
        (*char_count) = s2-s;
        return num;
    }

    /*
     * numbers can be written as base#n, where base is a number
     * between 2 and 64.. this is a non-POSIX extension.. digits higher
     * than 9 are represented by alphabetic characters a..z, A..Z, @ and _.
     * the number of legal letters depends on the selected base.
     */
    if(*s2 == '#')
    {
        base = num;
        num  = 0;
        s2++;
        while(get_ndigit(*s2, base, &num2))
        {
            num = (num*base) + num2;
            s2++;
        }
        /* check we didn't encounter an invalid digit */
        if(error)
        {
            return 0;
        }
    }
    (*char_count) = s2-s;
    return num;
}


/*
 * extract a shell variable name operand from the beginning of chars.
 */
struct symtab_entry_s *get_var(char *s, int *char_count)
{
    char *ss = s;
    if(*ss == '$')
    {
        ss++;        /* var names can begin with '$'. skip it */
    }
    char *s2 = ss;
    while(*s2 && valid_name_char(*s2))
    {
        s2++;
    }
    int len = s2-ss;
    /* empty var name */
    if(len == 0)
    {
        (*char_count) = s2-s;
        return NULL;
    }
    /* copy the name */
    char name[len+1];
    strncpy(name, ss, len);
    name[len] = '\0';
    /* get the symbol table entry for that var */
    struct symtab_entry_s *e = get_symtab_entry(name);
    if(!e)
    {
        e = add_to_symtab(name);
    }
    /* get the real length, including leading '$' if present */
    (*char_count) = s2-s;
    return e;
}


/*
 * Reverse Polish Notation (RPN) calculator.
 * 
 * POSIX note about arithmetic expansion:
 *   The shell shall expand all tokens in the expression for parameter expansion, 
 *   command substitution, and quote removal.
 * 
 * And the rules are:
 *   - Only signed long integer arithmetic is required.
 *   - Only the decimal-constant, octal-constant, and hexadecimal-constant constants 
 *     specified in the ISO C standard, Section 6.4.4.1 are required to be recognized 
 *     as constants.
 *   - The sizeof() operator and the prefix and postfix "++" and "--" operators are not 
 *     required.
 *   - Selection, iteration, and jump statements are not supported.
 * 
 * TODO: we should implement the functionality for math functions.
 *       this, of course, means we will need to compile our shell against
 *       libmath (by passing the -lm option to gcc).
 * 
 * TODO: other operators to implement (not required by POSIX):
 *       - the ternary operator (exprt ? expr : expr)
 *       - the comma operator (expr, expr)
 */

char *arithm_expand(char *orig_expr)
{
    char   *expr;
    char   *tstart       = NULL;
    struct  op_s startop = { 'X', 0, ASSOC_NONE, 0, 0, NULL };    /* Dummy operator to mark start */
    struct  op_s *op     = NULL;
    int     n1, n2;
    struct  op_s *lastop = &startop;
    /*
     * get a copy of orig_expr without the $(( and )), or the $[ and ]
     * if we're given the obsolete arithmetic expansion operator.
     */
    int baseexp_len = strlen(orig_expr);
    char *baseexp = malloc(baseexp_len+1);
    if(!baseexp)
    {
        fprintf(stderr, "error: insufficient memory for arithmetic expansion\n");
        return NULL;
    }
    /* lose the $(( */
    if(orig_expr[0] == '$' && orig_expr[1] == '(' && orig_expr[2] == '(')
    {
        strcpy(baseexp, orig_expr+3);
        baseexp_len -= 3;
        /* and the )) */
        if(baseexp[baseexp_len-1] == ')' && baseexp[baseexp_len-2] == ')')
        {
            baseexp[baseexp_len-2] = '\0';
        }
    }
    else
    {
        strcpy(baseexp, orig_expr);
    }

    /* init our stacks */
    nopstack = 0;
    nnumstack = 0;
    /* clear the error flag */
    error = 0;
    expr = baseexp;
    
    /* and go ... */
    for( ; *expr; )
    {
       if(!tstart)
        {
            if((op = get_op(expr)))
            {
                if(lastop && (lastop == &startop || lastop->op != ')'))
                {
                    /* take care of unary plus and minus */
                    if(op->op == '-')
                    {
                        op = OP_UMINUS;
                    }
                    else if(op->op == '+')
                    {
                        op = OP_UPLUS;
                    }
                    else if(op->op != '(' && !op->unary)
                    {
                        fprintf(stderr, "error: illegal use of binary operator (%c)\n", op->op);
                        goto err;
                    }
                }
                /* fix the pre-post ++/-- dilemma */
                if(op->op == CH_POST_INC || op->op == CH_POST_DEC)
                {
                    /* post ++/-- has higher precedence over pre ++/-- */
                    if(expr < baseexp+2 || !valid_name_char(expr[-2]))
                    {
                        if(op == OP_POST_INC)
                        {
                            op = OP_PRE_INC;
                        }
                        else
                        {
                            op = OP_PRE_DEC;
                        }
                    }
                }
                error = 0;
                shunt_op(op);
                if(error)
                {
                    goto err;
                }
                lastop = op;
                expr += op->chars;
            }
            else if(valid_name_char(*expr))
            {
                tstart = expr;
            }
            else if(isspace(*expr))
            {
                expr++;
            }
            else
            {
                fprintf(stderr, "error: Syntax error near: %s\n", expr);
                goto err;
            }
        }
        else
        {
            if(isspace(*expr))
            {
                expr++;
            }
            else if(isdigit(*expr))
            {
                error = 0;
                n1 = get_num(tstart, &n2);
                if(error)
                {
                    goto err;
                }
                push_numstackl(n1);
                if(error)
                {
                    goto err;
                }
                tstart = NULL;
                lastop = NULL;
                expr += n2;
            }
            else if(valid_name_char(*expr))
            {
                struct symtab_entry_s *n1 = get_var(tstart, &n2);
                if(!n1)
                {
                    fprintf(stderr, "error: Failed to add symbol near: %s\n", tstart);
                    goto err;
                }
                error = 0;
                push_numstackv(n1);
                if(error)
                {
                    goto err;
                }
                tstart = NULL;
                lastop = NULL;
                expr += n2;
            }
            else if((op = get_op(expr)))
            {
                error = 0;
                n1 = get_num(tstart, &n2);
                if(error)
                {
                    goto err;
                }
                push_numstackl(n1);
                if(error)
                {
                    goto err;
                }
                tstart = NULL;

                /* fix the pre-post ++/-- dilemma */
                if(op->op == CH_POST_INC || op->op == CH_POST_DEC)
                {
                    /* post ++/-- has higher precedence over pre ++/-- */
                    if(expr < baseexp+2 || !valid_name_char(expr[-2]))
                    {
                        if(op == OP_POST_INC)
                        {
                            op = OP_PRE_INC;
                        }
                        else
                        {
                            op = OP_PRE_DEC;
                        }
                    }
                }

                shunt_op(op);
                if(error)
                {
                    goto err;
                }
                lastop = op;
                expr += op->chars;
            }
            else
            {
                fprintf(stderr, "error: Syntax error near: %s\n", expr);
                goto err;
            }
        }
    }

    if(tstart)
    {
        error = 0;
        if(isdigit(*tstart))
        {
            n1 = get_num(tstart, &n2);
            if(error)
            {
                goto err;
            }
            push_numstackl(n1);
        }
        else if(valid_name_char(*tstart))
        {
            push_numstackv(get_var(tstart, &n2));
        }
        if(error)
        {
            goto err;
        }
    }

    while(nopstack)
    {
        error = 0;
        op = pop_opstack();
        if(error)
        {
            goto err;
        }
        struct stack_item_s n1 = pop_numstack();
        if(error)
        {
            goto err;
        }
        if(op->unary)
        {
            push_numstackl(op->eval(&n1, 0));
        }
        else
        {
            struct stack_item_s n2 = pop_numstack();
            if(error)
            {
                goto err;
            }
            push_numstackl(op->eval(&n2, &n1));
        }
        if(error)
        {
            goto err;
        }
    }

    /* empty arithmetic expression result */
    if(!nnumstack)
    {
        /*return false as the result */
        free(baseexp);
        return NULL;
    }

    /* we must have only 1 item on the stack now */
    if(nnumstack != 1)
    {
        fprintf(stderr, "error: Number stack has %d elements after evaluation. Should be 1.\n", nnumstack);
        goto err;
    }

    char res[64];
    sprintf(res, "%ld", numstack[0].val);
    char *res2 = malloc(strlen(res)+1);
    if(res2)
    {
        strcpy(res2, res);
    }
    /*
     * invert the exit status for callers who use our value to test for true/false exit status,
     * which is inverted, i.e. non-zero result is true (or zero exit status) and vice versa.
     * this is what bash does with the (( expr )) compound command.
     */
    free(baseexp);
    return res2;

err:
    free(baseexp);
    return NULL;
}

/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: scanner.c
 *    This file is part of the "Let's Build a Linux Shell" tutorial.
 *
 *    This tutorial is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This tutorial is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this tutorial.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "shell.h"
#include "scanner.h"
#include "source.h"

char *tok_buf = NULL;
int   tok_bufsize  = 0;
int   tok_bufindex = -1;

/* special token to indicate end of input */
struct token_s eof_token = 
{
    .text_len = 0,
};


void add_to_buf(char c)
{
    tok_buf[tok_bufindex++] = c;

    if(tok_bufindex >= tok_bufsize)
    {
        char *tmp = realloc(tok_buf, tok_bufsize*2);

        if(!tmp)
        {
            errno = ENOMEM;
            return;
        }

        tok_buf = tmp;
        tok_bufsize *= 2;
    }
}


struct token_s *create_token(char *str)
{
    struct token_s *tok = malloc(sizeof(struct token_s));
    
    if(!tok)
    {
        return NULL;
    }

    memset(tok, 0, sizeof(struct token_s));
    tok->text_len = strlen(str);
    
    char *nstr = malloc(tok->text_len+1);
    
    if(!nstr)
    {
        free(tok);
        return NULL;
    }
    
    strcpy(nstr, str);
    tok->text = nstr;
    
    return tok;
}


void free_token(struct token_s *tok)
{
    if(tok->text)
    {
        free(tok->text);
    }
    free(tok);
}


struct token_s *tokenize(struct source_s *src)
{
    int  endloop = 0;

    if(!src || !src->buffer || !src->bufsize)
    {
        errno = ENODATA;
        return &eof_token;
    }
    
    if(!tok_buf)
    {
        tok_bufsize = 1024;
        tok_buf = malloc(tok_bufsize);
        if(!tok_buf)
        {
            errno = ENOMEM;
            return &eof_token;
        }
    }

    tok_bufindex     = 0;
    tok_buf[0]       = '\0';

    char nc = next_char(src);

    if(nc == ERRCHAR || nc == EOF)
    {
        return &eof_token;
    }

    do
    {
        switch(nc)
        {
            case ' ':
            case '\t':
                if(tok_bufindex > 0)
                {
                    endloop = 1;
                }
                break;
                
            case '\n':
                if(tok_bufindex > 0)
                {
                    unget_char(src);
                }
                else
                {
                    add_to_buf(nc);
                }
                endloop = 1;
                break;
                
            default:
                add_to_buf(nc);
                break;
        }

        if(endloop)
        {
            break;
        }

    } while((nc = next_char(src)) != EOF);

    if(tok_bufindex == 0)
    {
        return &eof_token;
    }
    
    if(tok_bufindex >= tok_bufsize)
    {
        tok_bufindex--;
    }
    tok_buf[tok_bufindex] = '\0';

    struct token_s *tok = create_token(tok_buf);
    if(!tok)
    {
        fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
        return &eof_token;
    }

    tok->src = src;
    return tok;
}

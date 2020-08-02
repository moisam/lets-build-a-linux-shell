/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: source.c
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

#include <errno.h>
#include "shell.h"
#include "source.h"

void unget_char(struct source_s *src)
{
    if(src->curpos < 0)
    {
        return;
    }

    src->curpos--;
}


char next_char(struct source_s *src)
{
    if(!src || !src->buffer)
    {
        errno = ENODATA;
        return ERRCHAR;
    }

    if(src->curpos == INIT_SRC_POS)
    {
        src->curpos  = -1;
    }

    if(++src->curpos >= src->bufsize)
    {
        src->curpos = src->bufsize;
        return EOF;
    }
    
    return src->buffer[src->curpos];
}


char peek_char(struct source_s *src)
{
    if(!src || !src->buffer)
    {
        errno = ENODATA;
        return ERRCHAR;
    }

    long pos = src->curpos;
    
    if(pos == INIT_SRC_POS)
    {
        pos++;
    }
    pos++;

    if(pos >= src->bufsize)
    {
        return EOF;
    }

    return src->buffer[pos];
}


void skip_white_spaces(struct source_s *src)
{
    char c;
    
    if(!src || !src->buffer)
    {
        return;
    }

    while(((c = peek_char(src)) != EOF) && (c == ' ' || c == '\t'))
    {
        next_char(src);
    }
}

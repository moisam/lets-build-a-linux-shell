/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: prompt.c
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

#include <stdio.h>
#include "shell.h"
#include "symtab/symtab.h"


void print_prompt1(void)
{
    struct symtab_entry_s *entry = get_symtab_entry("PS1");

    if(entry && entry->val)
    {
        fprintf(stderr, "%s", entry->val);
    }
    else
    {
        fprintf(stderr, "$ ");
    }
}


void print_prompt2(void)
{
    struct symtab_entry_s *entry = get_symtab_entry("PS2");

    if(entry && entry->val)
    {
        fprintf(stderr, "%s", entry->val);
    }
    else
    {
        fprintf(stderr, "> ");
    }
}


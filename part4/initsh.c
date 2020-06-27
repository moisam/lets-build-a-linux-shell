/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: initsh.c
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

#include <string.h>
#include "shell.h"
#include "symtab/symtab.h"

extern char **environ;

void initsh()
{
    init_symtab();

    struct symtab_entry_s *entry;
    char **p2 = environ;
    
    while(*p2)
    {
        char *eq = strchr(*p2, '=');
        if(eq)
        {
            int len = eq-(*p2);
            char name[len+1];
            
	    strncpy(name, *p2, len);
            name[len] = '\0';
            entry = add_to_symtab(name);
            
	    if(entry)
            {
                symtab_entry_setval(entry, eq+1);
                entry->flags |= FLAG_EXPORT;
            }
        }
        else
        {
            entry = add_to_symtab(*p2);
        }
        p2++;
    }

    
    entry = add_to_symtab("PS1");
    symtab_entry_setval(entry, "$ ");

    entry = add_to_symtab("PS2");
    symtab_entry_setval(entry, "> ");
}

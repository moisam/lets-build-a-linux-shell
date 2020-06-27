/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: symtab.h
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

#ifndef SYMTAB_H
#define SYMTAB_H

#include "../node.h"

/* the type of a symbol table entry's value */
enum symbol_type_e
{
    SYM_STR ,
    SYM_FUNC,
};

/* the symbol table entry structure */
struct symtab_entry_s
{
    char     *name;                   /* key */
    enum      symbol_type_e val_type; /* type of value */
    char     *val;                    /* value */
    unsigned  int flags;              /* flags like readonly, export, ... */
    struct    symtab_entry_s *next;   /* pointer to the next entry */
    struct    node_s *func_body;      /* func's body AST (for funcs) */
};


struct symtab_s
{
    int    level;
    struct symtab_entry_s *first, *last;
};

/* values for the flags field of struct symtab_entry_s */
#define FLAG_EXPORT     (1 << 0)    /* export entry to forked commands */

/* the symbol table stack structure */
#define MAX_SYMTAB	256  /* maximum allowed symbol tables in the stack */

struct symtab_stack_s
{
    int    symtab_count;            /* number of tables in the stack */
    struct symtab_s *symtab_list[MAX_SYMTAB]; /* pointers to the tables */
    struct symtab_s *global_symtab,
		    *local_symtab;  /*
                                     * pointers to the local
                                     * and global symbol tables
                                     */
};

struct symtab_s       *new_symtab(int level);
struct symtab_s       *symtab_stack_push(void);
struct symtab_s       *symtab_stack_pop(void);
int rem_from_symtab(struct symtab_entry_s *entry, struct symtab_s *symtab);
struct symtab_entry_s *add_to_symtab(char *symbol);
struct symtab_entry_s *do_lookup(char *str, struct symtab_s *symtable);
struct symtab_entry_s *get_symtab_entry(char *str);
struct symtab_s       *get_local_symtab(void);
struct symtab_s       *get_global_symtab(void);
struct symtab_stack_s *get_symtab_stack(void);
void                   init_symtab(void);
void                   dump_local_symtab(void);
void                   free_symtab(struct symtab_s *symtab);
void                   symtab_entry_setval(struct symtab_entry_s *entry, char *val);

#endif

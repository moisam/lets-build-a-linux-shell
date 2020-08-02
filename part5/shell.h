/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: shell.h
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

#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>     /* size_t */
#include <glob.h>
#include "source.h"

void print_prompt1(void);
void print_prompt2(void);
char *read_cmd(void);
int  parse_and_execute(struct source_s *src);

void initsh(void);

/* shell builtin utilities */
int dump(int argc, char **argv);

/* struct for builtin utilities */
struct builtin_s
{
    char *name;    /* utility name */
    int (*func)(int argc, char **argv); /* function to call to execute the utility */
};

/* the list of builtin utilities */
extern struct builtin_s builtins[];

/* and their count */
extern int builtins_count;

/* struct to represent the words resulting from word expansion */
struct word_s
{
    char  *data;
    int    len;
    struct word_s *next;
};

/* word expansion functions */
struct  word_s *make_word(char *word);
void    free_all_words(struct word_s *first);

size_t  find_closing_quote(char *data);
size_t  find_closing_brace(char *data);
void    delete_char_at(char *str, size_t index);
char   *substitute_str(char *s1, char *s2, size_t start, size_t end);
char   *wordlist_to_str(struct word_s *word);

struct  word_s *word_expand(char *orig_word);
char   *word_expand_to_str(char *word);
char   *tilde_expand(char *s);
char   *command_substitute(char *__cmd);
char   *var_expand(char *__var_name);
char   *pos_params_expand(char *tmp, int in_double_quotes);
struct  word_s *pathnames_expand(struct word_s *words);
struct  word_s *field_split(char *str);
void    remove_quotes(struct word_s *wordlist);

char   *arithm_expand(char *__expr);

/* some string manipulation functions */
char   *strchr_any(char *string, char *chars);
char   *quote_val(char *val, int add_quotes);
int     check_buffer_bounds(int *count, int *len, char ***buf);
void    free_buffer(int len, char **buf);

/* pattern matching functions */
int     has_glob_chars(char *p, size_t len);
int     match_prefix(char *pattern, char *str, int longest);
int     match_suffix(char *pattern, char *str, int longest);
char  **get_filename_matches(char *pattern, glob_t *matches);

#endif

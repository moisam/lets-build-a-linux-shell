/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: parser.c
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

#include <unistd.h>
#include "shell.h"
#include "parser.h"
#include "scanner.h"
#include "node.h"
#include "source.h"


struct node_s *parse_simple_command(struct token_s *tok)
{
    if(!tok)
    {
        return NULL;
    }
    
    struct node_s *cmd = new_node(NODE_COMMAND);
    if(!cmd)
    {
        free_token(tok);
        return NULL;
    }
    
    struct source_s *src = tok->src;
    
    do
    {
        if(tok->text[0] == '\n')
        {
            free_token(tok);
            break;
        }

        struct node_s *word = new_node(NODE_VAR);
        if(!word)
        {
            free_node_tree(cmd);
            free_token(tok);
            return NULL;
        }
        set_node_val_str(word, tok->text);
        add_child_node(cmd, word);

        free_token(tok);

    } while((tok = tokenize(src)) != &eof_token);

    return cmd;
}

/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: node.c
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
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "shell.h"
#include "node.h"
#include "parser.h"


struct node_s *new_node(enum node_type_e type)
{
    struct node_s *node = malloc(sizeof(struct node_s));

    if(!node)
    {
        return NULL;
    }
    
    memset(node, 0, sizeof(struct node_s));
    node->type = type;
    
    return node;
}


void add_child_node(struct node_s *parent, struct node_s *child)
{
    if(!parent || !child)
    {
        return;
    }

    if(!parent->first_child)
    {
        parent->first_child = child;
    }
    else
    {
        struct node_s *sibling = parent->first_child;
    
    	while(sibling->next_sibling)
        {
            sibling = sibling->next_sibling;
        }
    
    	sibling->next_sibling = child;
        child->prev_sibling = sibling;
    }
    parent->children++;
}


void set_node_val_str(struct node_s *node, char *val)
{
    node->val_type = VAL_STR;

    if(!val)
    {
        node->val.str = NULL;
    }
    else
    {
        char *val2 = malloc(strlen(val)+1);
    
    	if(!val2)
        {
            node->val.str = NULL;
        }
        else
        {
            strcpy(val2, val);
            node->val.str = val2;
        }
    }
}


void free_node_tree(struct node_s *node)
{
    if(!node)
    {
        return;
    }

    struct node_s *child = node->first_child;
    
    while(child)
    {
        struct node_s *next = child->next_sibling;
        free_node_tree(child);
        child = next;
    }
    
    if(node->val_type == VAL_STR)
    {
        if(node->val.str)
        {
            free(node->val.str);
        }
    }
    free(node);
}


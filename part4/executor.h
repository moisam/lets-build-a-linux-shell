/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: executor.h
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

#ifndef BACKEND_H
#define BACKEND_H

#include "node.h"

char *search_path(char *file);
int do_exec_cmd(int argc, char **argv);
int do_simple_command(struct node_s *node);

#endif

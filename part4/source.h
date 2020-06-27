/* 
 *    Programmed By: Mohammed Isam [mohammed_isam1984@yahoo.com]
 *    Copyright 2020 (c)
 * 
 *    file: source.h
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

#ifndef SOURCE_H
#define SOURCE_H

#define EOF             (-1)
#define ERRCHAR         ( 0)

#define INIT_SRC_POS    (-2)

struct source_s
{
    char *buffer;       /* the input text */
    long bufsize;       /* size of the input text */
    long  curpos;       /* absolute char position in source */
};

char next_char(struct source_s *src);
void unget_char(struct source_s *src);
char peek_char(struct source_s *src);
void skip_white_spaces(struct source_s *src);

#endif



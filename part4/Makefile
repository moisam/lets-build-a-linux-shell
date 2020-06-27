# 
#    Copyright 2020 (c)
#    Mohammed Isam [mohammed_isam1984@yahoo.com]
# 
#    file: Makefile
#    This file is part of the "Let's Build a Linux Shell" tutorial.
#
#    This tutorial is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This tutorial is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this tutorial.  If not, see <http://www.gnu.org/licenses/>.
#    


# directory definitions
SRCDIR=.
BUILTINS_SRCDIR=$(SRCDIR)/builtins
SYMTAB_SRCDIR=$(SRCDIR)/symtab
BUILD_DIR=$(SRCDIR)/build

# compiler name and flags
CC=gcc
LIBS=
CFLAGS=-Wall -Wextra -g -I$(SRCDIR)
LDFLAGS=-g

# generate the lists of source and object files
SRCS_BUILTINS=$(shell find $(SRCDIR)/builtins -name "*.c")

SRCS_SYMTAB=$(SRCDIR)/symtab/symtab.c

SRCS=main.c prompt.c node.c parser.c scanner.c source.c executor.c initsh.c  \
     $(SRCS_BUILTINS) $(SRCS_SYMTAB)

OBJS=$(SRCS:%.c=$(BUILD_DIR)/%.o)

# output file name
TARGET=shell

# default target (when we call make with no arguments)
.PHONY: all
all: prep-build $(TARGET)

prep-build:
	mkdir -p $(BUILD_DIR)/builtins
	mkdir -p $(BUILD_DIR)/symtab

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# target to auto-generate header file dependencies for source files
depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

#compile C source files
$(BUILD_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean target
.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET) core .depend
	$(RM) -r $(BUILD_DIR)


AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = main
main_SOURCES = main.c arena.c ast.c hashtbl.c lexer.c pair.c parser.c token.c utils.c vector.c

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/test

SUBDIRS = test

AM_CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g -DDEBUG -Wno-gnu-zero-variadic-macro-arguments

.PHONY: test
test:
	$(MAKE) -C test test_suite
	./test/test_suite

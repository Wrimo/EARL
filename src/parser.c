// MIT License

// Copyright (c) 2023 malloc-nbytes

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// File: parser.c
// Description:
//   The implementation of a set number
//   of parsers that will parse different
//   types of statements and expressions.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "notify.h"
#include "token.h"
#include "ast.h"
#include "parser.h"

/********** HELPERS **********/

// Name: expect
//
// Description:
//   Given `lexer` and some expected token type `exp`,
//   will check if the type of the head token in the
//   lexer matches the type `exp`. If they do not
//   match, panic.
struct token *
expect(struct lexer *lexer, enum token_type exp)
{
  struct token *hd = lexer_next(lexer);
  if (hd->type != exp) {
    NOTIFY_ERRARGS(NOTIFY_ERR_SYNTAX, "expect: expected %d but got %d", exp, hd->type);
  }
  return hd;
}

// Name: expect_keyword
//
// Description:
//   Given `lexer` and some expected keyword `keyword`,
//   will check if the type of the head token in the
//   lexer matches the type of keyword AND checks if
//   `keyword` matches lexer->hd->lexeme. If they do not
//   match, panic.
struct token *
expect_keyword(struct lexer *lexer, const char *keyword)
{
  struct token *hd = lexer_next(lexer);
  if (hd->type != TOKENTYPE_KEYWORD || utils_streq(hd->lexeme, keyword)) {
    NOTIFY_ERRARGS(NOTIFY_ERR_SYNTAX, "expect_keyword: expected keyword %s but got %s", keyword, hd->lexeme);
  }
  return hd;
}

struct token *
expect_type(struct lexer *lexer)
{
  NOOP(lexer);
  UNIMPLEMENTED("expect_type", NULL);
}

/********** PARSERS **********/

// Name: parse_def_stmt_args
//
// Description:
//   Given the syntax of (k1: ty1, k2: ty2,...,kn: tyn)
//   will parse and return a vector of pairs of the id
//   and the associated type i.e.
//     vec[(k1, ty1), (k2, ty2),...,(kn, tyn)].
//
// NOTE: Expects to have the LPAREN ('(') and RPAREN (')')
//   and will consume those.
struct vector(struct pair(struct token *id, struct token *type))
parse_def_stmt_args(struct lexer *lexer)
{
  (void)expect(lexer, TOKENTYPE_LPAREN);
  assert(0 && "parse_def_stmt_args: unimplemented");
  (void)expect(lexer, TOKENTYPE_RPAREN);
}

struct stmt_block *
parse_stmt_block(struct lexer *lexer)
{
  NOOP(lexer);
  UNIMPLEMENTED("parse_stmt_block", NULL);
}

struct stmt_def *
parse_stmt_def(struct lexer *lexer)
{
  // def
  lexer_discard(lexer);

  // identifier
  struct token *id = expect(lexer, TOKENTYPE_IDENT);

  // (...)
  struct vector(struct pair(struct token *id, struct token *type)) args
    = parse_def_stmt_args(lexer);

  // ->
  (void)expect(lexer, TOKENTYPE_MINUS);
  (void)expect(lexer, TOKENTYPE_GREATERTHAN);

  // type
  struct token *rettype = expect_type(lexer);

  // { ... }
  struct stmt_block *block = parse_stmt_block(lexer);

  return stmt_def_alloc(id, args, rettype, block);
}

struct stmt *
parse_stmt(struct lexer *lexer)
{
  // NOTE: cannot switch on lexer_next as this
  // will make parsing expression very difficult.
  switch (lexer->hd->type) {
  case TOKENTYPE_KEYWORD: {
    if (utils_streq(lexer->hd->lexeme, "def")) {
      ;
    }
    else if (utils_streq(lexer->hd->lexeme, "let")) {
      ;
    }
  } break;

  case TOKENTYPE_IDENT: {
  } break;

  default:
    NOTIFY_ERRARGS(ERR_FATAL, "parse_stmt found an unkown statement of type ID (%d).", lexer->hd->type);
  }
}

struct vector(struct stmt *)
parse_stmts(struct lexer *lexer)
{
  struct vector stmts = vector_create(struct stmt *);

  struct token *curtok = NULL;
  while ((curtok = lexer_next(lexer)) != NULL) {
    switch (curtok->type) {
      case TOKENTYPE_KEYWORD: {
        if (utils_streq(curtok->lexeme, "let")) {
          ;
        }
        else if (utils_streq(curtok->lexeme, "def")) {
          ;
        }
        break;
      }
      case TOKENTYPE_IDENT: {
        break;
      }
      case TOKENTYPE_EOF:
        break;
      default: {
        NOTIFY_ERRARGS(ERR_FATAL, "parse_stmts found an unkown statement of type ID (%d).", curtok->type);
      }
    }
  }

  return stmts;
}

struct program
parse(struct lexer *lexer)
{
  return (struct program) {
    .stmts = parse_stmts(lexer),
  };
}

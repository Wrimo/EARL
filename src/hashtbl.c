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

#include <stdlib.h>
#include <string.h>

#include "hashtbl.h"
#include "utils.h"

struct hashtbl_node {
  uint8_t *key;
  uint8_t *value;
  struct hashtbl_node *next;
};

struct hashtbl {
  // The table of nodes
  struct hashtbl_node **tbl;

  // Hash function needed
  unsigned (*hashfunc)(void *key, size_t bytes);

  // Function for comparing keys.
  // TODO: is this really needed? or
  // would memcmp be enough?
  int (*keycompar)(void *x, void *y);

  // The size of each key in bytes.
  size_t key_stride;

  // The size of each value in bytes.
  size_t value_stride;

  // The number of nodes in the tbl;
  size_t len;

  // The capacity of the tbl;
  size_t cap;
};

struct hashtbl_node *
hashtbl_node_alloc(struct hashtbl *ht, void *key, void *value, struct hashtbl_node *next)
{
  struct hashtbl_node *n = utils_safe_malloc(sizeof(struct hashtbl_node));
  n->key = key;
  n->value = value;
  n->next = next;

  (void)memcpy(n->key, (uint8_t *)key, ht->key_stride);
  (void)memcpy(n->value, (uint8_t *)value, ht->value_stride);

  return n;
}

struct hashtbl
hashtbl_create(size_t key_stride, size_t value_stride,
               unsigned (*hashfunc)(void *key, size_t bytes),
               int (*keycompar)(void *x, void *y))
{
  return (struct hashtbl) {
    .tbl = NULL,
    .hashfunc = hashfunc,
    .keycompar = keycompar,
    .key_stride = key_stride,
    .value_stride = value_stride,
    .len = 0,
    .cap = 0,
  };
}

struct hashtbl_node *
find(struct hashtbl *ht, struct hashtbl_node *lst, void *key)
{
  struct hashtbl_node *it = lst;
  while (it && !ht->keycompar(key, it->value)) {
    it = it->next;
  }
  return it;
}

static void
try_resize(struct hashtbl(any, any) *ht)
{
  if ((float)ht->len / (float)ht->cap > 0.5f) {
    size_t new_cap = ht->cap*2;
    struct hashtbl_node **new_tbl = utils_safe_malloc(sizeof(struct hashtbl_node *)*new_cap);

    for (size_t i = 0; i < ht->cap; ++i) {
      struct hashtbl_node *tmp = ht->tbl[i];
      while (tmp) {
        struct hashtbl_node *next = tmp->next;
        unsigned newidx = ht->hashfunc(tmp->key, ht->key_stride)%new_cap;
        new_tbl[newidx] = tmp;
        tmp = next;
      }
    }

    free(ht->tbl);
    ht->tbl = new_tbl;
    ht->cap = new_cap;
  }
}

void
hashtbl_insert(struct hashtbl *ht, void *key, void *value, size_t bytes)
{
  unsigned idx = ht->hashfunc(key, bytes)%ht->cap;

  struct hashtbl_node *p = find(ht, ht->tbl[idx], key);

  if (!p) {
    ht->tbl[idx] = hashtbl_node_alloc(ht, ht->tbl[idx], key, value);
    ht->len++;
  }
  else {
    (void)memcpy(p->value, value, ht->key_stride);
  }

  try_resize(ht);
}

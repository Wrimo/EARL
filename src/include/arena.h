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

// File: arena.h
// Description:
//   Provides an API for an arena allocator. This is
//   helpful because we have a "pool" of memory that
//   we can use and then free it all at once. If
//   unfamiliar with arena allocators,
//   https://en.wikipedia.org/wiki/Region-based_memory_management

#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

struct arena;

// Create a new arena allocator with capacity as `cap`.
struct arena *arena_create(size_t cap);

// Allocate `bytes` number of bytes in an arena.
uint8_t *arena_alloc(struct arena *arena, size_t bytes);

// Free the memory inside of `arena` AS WELL AS
// THE ARENA ITSELF.
void arena_free(struct arena *arena);

#endif // ARENA_H

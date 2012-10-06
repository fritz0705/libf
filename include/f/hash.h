/* Copyright (c) 2012 Fritz Grimpen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include <f/fnv.h>

#ifdef LIBF_INTERNAL
struct hash_node
{
	fnv_t hash;
	void *value;
};

struct hash
{
	list_t data;
	unsigned int bits;
};
#endif

typedef struct hash *hash_t;

hash_t hash_new();

void *hash_set(hash_t h, void *k, unsigned int ksize, void *v);
void *hash_get(hash_t h, void *k, unsigned int ksize);
void *hash_delete(hash_t h, void *k, unsigned int ksize);

void *hash_set_r(hash_t h, fnv_t fnv, void *value);
void *hash_get_r(hash_t h, fnv_t fnv);
void *hash_delete_r(hash_t h, fnv_t fnv);

unsigned int hash_size(hash_t h);

void hash_clean(hash_t h);
void hash_destroy(hash_t h);


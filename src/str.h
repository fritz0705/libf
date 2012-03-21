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

#include <f/_.h>
#include <f/list.h>
#include <f/alloc.h>

struct str_chunk
{
	unsigned int refs;
	unsigned int length;
	char *data;
};

struct str
{
	list_t chunks;
	_Bool frozen;
};

static inline struct str_chunk *newchunk(char *d, int len)
{
	struct str_chunk *chunk = alloc(sizeof(struct str_chunk));
	if (chunk == NULL)
		return NULL;

	chunk->length = len;
	chunk->refs = 1;
	chunk->data = alloc(len);
	if (chunk->data == NULL)
	{
		unalloc(chunk);
		return NULL;
	}

	for (int off = 0; off < len; ++off)
		chunk->data[off] = d[off];
	return chunk;
}

static inline void usechunk(struct str_chunk *c)
{
	++c->refs;
}

static inline void unallocchunk(struct str_chunk *c)
{
	--c->refs;
	if (c->refs == 0)
	{
		unalloc(c->data);
		unalloc(c);
	}
}

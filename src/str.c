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

#include <f/str.h>
#include <f/list.h>

#include <stdlib.h>
#include <string.h>

struct str_chunk
{
	unsigned int refs;
	unsigned int length;
	char *data;
};

struct str
{
	list_t chunks;
};

static inline struct str_chunk *newchunk(char *d, int len)
{
	struct str_chunk *chunk = malloc(sizeof(struct str_chunk));
	if (chunk == NULL)
		return NULL;

	chunk->length = len;
	chunk->refs = 1;
	chunk->data = malloc(len);
	if (chunk->data == NULL)
	{
		free(chunk);
		return NULL;
	}

	memcpy(chunk->data, d, len);
	return chunk;
}

static inline void usechunk(struct str_chunk *c)
{
	++c->refs;
}

static inline void freechunk(struct str_chunk *c)
{
	--c->refs;
	if (c->refs == 0)
	{
		free(c->data);
		free(c);
	}
}

str_t str_new()
{
	str_t str = malloc(sizeof(struct str));
	if (str == NULL)
		return NULL;

	str->chunks = list_new();

	return str;
}

/* Create from C String */
str_t str_create_cs(char *cs)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;
	struct str_chunk *chunk = newchunk(cs, strlen(cs));
	if (chunk == NULL)
	{
		str_destroy(str);
		return NULL;
	}

	list_append(str->chunks, chunk);
	return str;
}

/* Create from char */
str_t str_create_c(char c)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;
	struct str_chunk *chunk = newchunk(&c, 1);
	if (chunk == NULL)
	{
		str_destroy(str);
		return NULL;
	}

	list_append(str->chunks, chunk);
	return str;
}

str_t str_join(str_t left, str_t right)
{
	str_t newstr = str_new();
	if (newstr == NULL)
		return NULL;

	int chunks, i;

	chunks = list_length(left->chunks);
	for (i = 0; i < chunks; ++i)
	{
		struct str_chunk *c = list_get(left->chunks, i);
		usechunk(c);
		list_append(newstr->chunks, c);
	}

	chunks = list_length(right->chunks);
	for (i = 0; i < chunks; ++i)
	{
		struct str_chunk *c = list_get(right->chunks, i);
		usechunk(c);
		list_append(newstr->chunks, c);
	}

	return newstr;
}

/* TODO Implement this :p */
str_t str_sub(str_t left, int offset, unsigned int length)
{
	str_t newstr = str_new();
	if (newstr == NULL)
		return NULL;

	return newstr;
}

char *str_dump(str_t str)
{
	char *dump = malloc(str_length(str));
	if (dump == NULL)
		return NULL;

	char *offset = dump;
	int chunks = list_length(str->chunks);
	for (int i = 0; i < chunks; ++i)
	{
		struct str_chunk *c = list_get(str->chunks, i);
		memcpy(offset, c->data, c->length);
		offset += c->length;
	}

	return dump;
}

unsigned int str_length(str_t str)
{
	int chunks = list_length(str->chunks);
	unsigned int length = 0;
	for (int i = 0; i < chunks; ++i)
		length += ((struct str_chunk *)list_get(str->chunks, i))->length;

	return length;
}

void str_destroy(str_t str)
{
	int chunks = list_length(str->chunks);
	if (chunks == 0)
		goto free_string;

	for (int i = 0; i < chunks; ++i)
		freechunk(list_get(str->chunks, i));

free_string:
	list_destroy(str->chunks);
	free(str);
}

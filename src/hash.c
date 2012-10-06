/* Copyright (c) 2012 Fritz Grimpen
 *
 * Permission is hereby granted, unalloc of charge, to any person obtaining a copy of
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

#define LIBF_INTERNAL
#include <f/_.h>
#include <f/hash.h>
#include <f/list.h>
#include <f/alloc.h>

hash_t hash_new()
{
	hash_t h = alloc(sizeof(struct hash));
	if (h == NULL)
		return NULL;

	h->data = list_new();
	if (h->data == NULL)
	{
		unalloc(h);
		return NULL;
	}
	h->bits = 64;

	return h;
}

void *hash_set_r(hash_t h, fnv_t fnv, void *val)
{
	struct hash_node *node = alloc(sizeof(struct hash_node));
	if (node == NULL)
		return NULL;

	node->hash = fnv;
	node->value = val;

	if (list_append(h->data, node) == NULL)
	{
		unalloc(node);
		return NULL;
	}

	return val;
}

void *hash_get_r(hash_t h, fnv_t fnv)
{
	list_iterator_t i = list_iterate(h->data);
	if (i == NULL)
		return NULL;

	struct hash_node *cur = NULL;
	while (1)
	{
		cur = list_iterate_next(i);
		if (cur == NULL || cur->hash == fnv)
			break;
	}

	list_iterate_end(i);
	if (cur == NULL)
		return cur;
	return cur->value;
}

void *hash_delete_r(hash_t h, fnv_t fnv)
{
	list_iterator_t i = list_iterate(h->data);
	if (i == NULL)
		return NULL;

	struct hash_node *cur = NULL;
	unsigned int offset = 0;
	while (1)
	{
		cur = list_iterate_next(i);
		if (cur == NULL || cur->hash == fnv)
			break;
		++offset;
	}

	list_iterate_end(i);
	if (cur == NULL)
		return NULL;

	void *ret = cur->value;
	list_delete(h->data, offset);
	unalloc(cur);
	return ret;
}

void *hash_set(hash_t h, void *k, unsigned int ksize, void *v)
{
	return hash_set_r(h, fnv_calc(h->bits, k, ksize), v);
}

void *hash_get(hash_t h, void *k, unsigned int ksize)
{
	return hash_get_r(h, fnv_calc(h->bits, k, ksize));
}

void *hash_delete(hash_t h, void *k, unsigned int ksize)
{
	return hash_delete_r(h, fnv_calc(h->bits, k, ksize));
}

void hash_clean(hash_t h)
{
	list_iterator_t i = list_iterate(h->data);
	if (i == NULL)
		goto clean;

	void *cur = NULL;
	while (1)
	{
		cur = list_iterate_next(i);
		if (cur == NULL)
			break;
		unalloc(cur);
	}

	list_iterate_end(i);
clean:
	list_clean(h->data);
}

unsigned int hash_size(hash_t h)
{
	return list_length(h->data);
}

void hash_destroy(hash_t h)
{
	hash_clean(h);
	list_destroy(h->data);
	unalloc(h);
}


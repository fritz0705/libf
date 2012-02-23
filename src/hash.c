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

#include <f/hash.h>
#include <f/list.h>
#include <f/fnv.h>

#include <stdint.h>
#include <stdlib.h>

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

hash_t hash_new()
{
	hash_t h = malloc(sizeof(struct hash));
	if (h == NULL)
		return NULL;

	h->data = list_new();
	if (h->data == NULL)
	{
		free(h);
		return NULL;
	}
	h->bits = 64;

	return h;
}

void *hash_set(hash_t h, void *k, unsigned int ksize, void *v)
{
	struct hash_node *node = malloc(sizeof(struct hash_node));
	if (node == NULL)
		return NULL;

	node->hash = fnv_calc(h->bits, k, ksize);
	node->value = v;

	if (list_append(h->data, node) == NULL)
	{
		free(node);
		return NULL;
	}

	return v;
}

void *hash_get(hash_t h, void *k, unsigned int ksize)
{
	int length = list_length(h->data);
	if (length == 0)
		return NULL;

	fnv_t hash = fnv_calc(h->bits, k, ksize);

	for (int i = 0; i < length; ++i)
	{
		struct hash_node *node = (struct hash_node *)list_get(h->data, i);
		if (node == NULL)
			continue;

		if (node->hash == hash)
			return node->value;
	}

	return NULL;
}

void *hash_delete(hash_t h, void *k, unsigned int ksize)
{
	int length = list_length(h->data);
	if (length == 0)
		return NULL;

	fnv_t hash = fnv_calc(h->bits, k, ksize);
	void *ret = NULL;

	for (int i = 0; i < length; ++i)
	{
		struct hash_node *node = (struct hash_node *)list_get(h->data, i);

		if (node->hash == hash)
		{
			ret = node->value;
			free(node);
			list_delete(h->data, i);
			break;
		}
	}

	return ret;
}

void _clean_iterator(unsigned int i, void *hnode_r, void *unused)
{
	free(hnode_r);
}

void hash_clean(hash_t h)
{
	list_iterate(h->data, _clean_iterator, NULL);
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
	free(h);
}


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

#include <stdint.h>
#include <stdlib.h>

struct hash_node
{
	uint32_t hash;
	char *key;
	size_t key_size;
	void *value;
};

struct hash
{
	struct list *data;
};

static uint32_t calc_fnv1a_32(char *k, size_t ksize)
{
	uint32_t hash = 2166136261U;
	for (size_t i = 0; i < ksize; ++i)
	{
		hash = hash ^ k[i];
		hash = hash * 16777619U;
	}

	return hash;
}

struct hash *hash_new()
{
	struct hash *h = malloc(sizeof(struct hash));
	if (h == NULL)
		return NULL;

	h->data = list_new();
	if (h->data == NULL)
	{
		free(h);
		return NULL;
	}

	return h;
}

void *hash_set(struct hash *h, char *k, size_t ksize, void *v)
{
	struct hash_node *node = malloc(sizeof(struct hash_node));
	if (node == NULL)
		return NULL;

	node->hash = calc_fnv1a_32(k, ksize);
	node->key = k;
	node->key_size = ksize;
	node->value = v;

	if (list_append(h->data, node) == NULL)
	{
		free(node);
		return NULL;
	}

	return v;
}

void *hash_get(struct hash *h, char *k, size_t ksize)
{
	int length = list_length(h->data);
	if (length == 0)
		return NULL;

	uint32_t hash = calc_fnv1a_32(k, ksize);

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

void *hash_delete(struct hash *h, char *k, size_t ksize)
{
	int length = list_length(h->data);
	if (length == 0)
		return NULL;

	uint32_t hash = calc_fnv1a_32(k, ksize);
	void *ret = NULL;

	for (int i = 0; i < length; ++i)
	{
		struct hash_node *node = (struct hash_node *)list_get(h->data, i);

		if (node->hash == hash)
		{
			ret = node->value;
			list_delete(h->data, i);
			break;
		}
	}

	return ret;
}

void hash_clean(struct hash *h)
{
	int length = list_length(h->data);

	for (int i = 0; i < length; ++i)
	{
		struct hash_node *node = (struct hash_node *)list_get(h->data, i);
		free(node);
		list_delete(h->data, i);
	}
}

void hash_destroy(struct hash *h)
{
	hash_clean(h);
	list_destroy(h->data);
}


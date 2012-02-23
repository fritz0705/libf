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

#include <f/list.h>
#include <stdlib.h>

struct list
{
	struct list_node *first_node;
	struct list_node *last_node;
};

struct list_node
{
	struct list *list;
	struct list_node *next;
	struct list_node *prev;
	void *data;
};

static inline struct list_node *get_node(list_t l, int offset)
{
	struct list_node *node = l->first_node;
	for (int i = 0; node != NULL && i < offset; ++i)
		node = node->next;

	return node;
}

static inline void recalculate_list(list_t l)
{
	struct list_node *cur_node = l->first_node;
	int i;
	for (i = 0; cur_node != NULL; ++i)
		cur_node = cur_node->next;

	l->last_node = cur_node;
}

static list_t list_init(list_t l)
{
	l->first_node = NULL;
	l->last_node = NULL;

	return l;
}

list_t list_new()
{
	list_t list = malloc(sizeof(struct list));
	if (list == NULL)
		return NULL;

	return list_init(list);
}

void *list_append(list_t l, void *data)
{
	struct list_node *node = malloc(sizeof(struct list_node));

	node->data = data;
	node->list = l;
	node->next = NULL;
	node->prev = NULL;

	if (l->first_node == NULL)
	{
		l->first_node = node;
		l->last_node = node;

		return data;
	}

	node->prev = l->last_node;
	l->last_node->next = node;
	l->last_node = node;

	return data;
}

void *list_prepend(list_t l, void *data)
{
	struct list_node *node = malloc(sizeof(struct list_node));

	node->data = data;
	node->list = l;
	node->next = NULL;
	node->prev = NULL;

	if (l->last_node == NULL)
	{
		l->first_node = node;
		l->last_node = node;

		return data;
	}

	node->next = l->first_node;
	l->first_node->prev = node;
	l->first_node = node;

	return data;
}

void *list_insert(list_t l, void *data, int offset)
{
	struct list_node *next_node = get_node(l, offset);
	if (next_node == NULL)
		return NULL;

	struct list_node *prev_node = next_node->prev;

	struct list_node *node = malloc(sizeof(struct list_node));

	node->data = data;
	node->list = l;

	node->next = next_node;
	node->prev = prev_node;

	next_node->prev = node;
	if (prev_node == NULL)
		l->first_node = node;

	return data;
}

void *list_get(list_t l, int offset)
{
	return get_node(l, offset)->data;
}

int list_find(list_t l, void *data)
{
	struct list_node *node = l->first_node;
	int i;
	for (i = 0; node != NULL && node->data != data; ++i)
		node = node->next;

	if (node == NULL)
		return -1;

	return i;
}

void *list_delete(list_t l, int offset)
{
	struct list_node *node = get_node(l, offset);
	if (node == NULL)
		return NULL;

	if (node->prev != NULL)
		node->prev->next = node->next;
	else
		l->first_node = node->next;

	if (node->next != NULL)
		node->next->prev = node->prev;
	else
		l->last_node = node->prev;

	void *ret = node->data;
	free(node);

	return ret;
}

void list_clean(list_t l)
{
	struct list_node *node = l->first_node;
	for (; node != NULL; node = node->next)
		free(node);

	l->first_node = NULL;
	l->last_node = NULL;
}

void list_destroy(list_t l)
{
	list_clean(l);
	free(l);
}

void list_rebuild(list_t l)
{
	recalculate_list(l);
}

int list_length(list_t l)
{
	struct list_node *node = l->first_node;
	unsigned int length = 0;
	while (node != NULL)
	{
		++length;
		node = node->next;
	}

	return length;
}

void list_iterate(list_t l, list_iterator_t iter, void *data)
{
	struct list_node *node = l->first_node;
	for (unsigned int i = 0; node != NULL; node = node->next, ++i)
		iter(i, node->data, data);
}

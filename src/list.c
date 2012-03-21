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

#include <f/_.h>
#include <f/list.h>
#include <f/alloc.h>

#if __STDC_HOSTED__ == 1
#include <stdarg.h>
#endif

#include "list.h"

static inline int absoffset(list_t l, int offset)
{
	if (offset < 0)
		return list_length(l) + offset;
	return offset;
}

static inline struct list_node *get_node(list_t l, int offset)
{
	struct list_node *node = l->first_node;
	for (int i = 0; node != NULL && i < offset; ++i)
		node = node->next;

	return node;
}

list_t list_new()
{
	list_t list = alloc(sizeof(struct list));
	if (list == NULL)
		return NULL;

	list->first_node = NULL;
	list->last_node = NULL;

	return list;
}

void *list_append(list_t l, void *data)
{
	struct list_node *node = alloc(sizeof(struct list_node));

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
	struct list_node *node = alloc(sizeof(struct list_node));

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
	offset = absoffset(l, offset);

	struct list_node *next_node = get_node(l, offset);
	if (next_node == NULL)
		return NULL;

	struct list_node *prev_node = next_node->prev;

	struct list_node *node = alloc(sizeof(struct list_node));

	node->data = data;
	node->list = l;

	node->next = next_node;
	node->prev = prev_node;

	next_node->prev = node;
	if (prev_node == NULL)
		l->first_node = node;
	else
		prev_node->next = node;

	return data;
}

void *list_get(list_t l, int offset)
{
	offset = absoffset(l, offset);

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
	offset = absoffset(l, offset);

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
	unalloc(node);

	return ret;
}

void list_clean(list_t l)
{
	struct list_node *node = l->first_node;
	while (node != NULL)
	{
		struct list_node *next = node->next;
		unalloc(node);
		node = next;
	}

	l->first_node = NULL;
	l->last_node = NULL;
}

void list_destroy(list_t l)
{
	list_clean(l);
	unalloc(l);
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

#if __STDC_HOSTED__ == 1
list_t list_build(void *v, ...)
{
	list_t list = list_new();
	if (list == NULL)
		return NULL;

	list_append(list, v);
	va_list ap;
	va_start(ap, v);

	while (1)
	{
		void *e = va_arg(ap, void *);
		if (e == NULL)
			break;

		list_append(list, e);
	}

	va_end(ap);

	return list;
}
#endif

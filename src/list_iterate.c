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

#include "list.h"

struct list_iterator
{
	struct list *list;
	struct list_node *current;
};

list_iterator_t list_iterate(list_t l)
{
	list_iterator_t iter = malloc(sizeof(struct list_iterator));
	if (iter == NULL)
		return NULL;

	iter->list = l;
	iter->current = l->first_node;

	return iter;
}

void *list_iterate_next(list_iterator_t i)
{
	if (i->current == NULL)
		return NULL;

	void *val = i->current->data;
	i->current = i->current->next;

	return val;
}

void list_iterate_end(list_iterator_t i)
{
	free(i);
}

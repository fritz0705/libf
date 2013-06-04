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

#define F_LIST_STRUCTS

#include <f/list.h>

F_list_t F_list_create()
{
	F_list_t l = malloc(sizeof *l);
	if (!l)
		return NULL;

	l->stub = (struct F_list_node){
		.next = &l->stub,
		.prev = &l->stub,
		.lstub = &l->stub,
		.stub = true,
		.data = (uintptr_t)l
	};

	return l;
}

F_list_node_t F_list_head(F_list_t l)
{
	return &l->stub;
}

F_list_node_t F_list_tail(F_list_t l)
{
	return &l->stub;
}

F_list_node_t F_list_next(F_list_node_t l)
{
	return l->next;
}

F_list_node_t F_list_prev(F_list_node_t l)
{
	return l->prev;
}

F_list_node_t F_list_insert_after(F_list_node_t n, uintptr_t data)
{
	/* Allocate new list node */
	F_list_node_t new_n = malloc(sizeof *new_n);
	/* Fill new list node with parameters */
	*new_n = (struct F_list_node){
		.next = n->next,
		.prev = n,
		.lstub = n->lstub,
		.stub = false,
		.data = data
	};

	n->next->prev = new_n;
	n->next = new_n;

	return new_n;
}

F_list_node_t list_insert_before(F_list_node_t n, uintptr_t data)
{
	/* Allocate new list node */
	F_list_node_t new_n = malloc(sizeof *new_n);
	/* Fill new list node with parameters */
	*new_n = (struct F_list_node){
		.next = n,
		.prev = n->prev,
		.lstub = n->lstub,
		.stub = false,
		.data = data
	};

	n->prev->next = new_n;
	n->prev = new_n;

	return new_n;
}

F_list_node_t F_list_replace(F_list_node_t n, uintptr_t data)
{
	n->data = data;
	return n;
}

const uintptr_t *F_list_data(F_list_node_t n)
{
	return &n->data;
}

bool F_list_remove(F_list_node_t n)
{
	if (n->stub)
		return false;

	n->next->prev = n->prev;
	n->prev->next = n->next;

	free(n);

	return true;
}

bool F_list_is_stub(F_list_node_t n)
{
	return n->stub;
}

void F_list_destroy(F_list_t l)
{
	(void)l;
}

F_list_t F_list_get_list(F_list_node_t n)
{
	return (F_list_t)n->lstub->data;
}

F_list_node_t F_list_append(F_list_t l, uintptr_t data)
{
	return list_insert_before(&l->stub, data);
}

F_list_node_t F_list_prepend(F_list_t l, uintptr_t data)
{
	return F_list_insert_after(&l->stub, data);
}


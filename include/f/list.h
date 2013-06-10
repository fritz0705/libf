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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef F_LIST_H_
#define F_LIST_H_

typedef struct F_list *F_list_t;
typedef struct F_list_node *F_list_node_t;

struct F_list_node
{
	uintptr_t data;
	F_list_node_t next;
	F_list_node_t prev;
	F_list_node_t lstub;
	bool stub:1;
} __attribute__((packed));

struct F_list
{
	struct F_list_node stub;
};

F_list_t F_list_create();

static inline F_list_node_t F_list_head(F_list_t l)
{
	return &l->stub;
}

static F_list_node_t (*F_list_tail)(F_list_t) = F_list_head;

static inline F_list_node_t F_list_next(F_list_node_t l)
{
	return l->next;
}
static inline F_list_node_t F_list_prev(F_list_node_t l)
{
	return l->prev;
}

F_list_node_t F_list_insert_after(F_list_node_t n, uintptr_t data);
F_list_node_t F_list_insert_before(F_list_node_t n, uintptr_t data);

F_list_node_t F_list_replace(F_list_node_t n, uintptr_t data);

F_list_node_t F_list_append(F_list_t l, uintptr_t data);
F_list_node_t F_list_prepend(F_list_t l, uintptr_t data);

F_list_t F_list_get_list(F_list_node_t n);

static inline uintptr_t F_list_data(F_list_node_t n)
{
	return n->data;
}

bool F_list_remove(F_list_node_t n);

static inline bool F_list_is_stub(F_list_node_t n)
{
	return n->stub;
}

void F_list_destroy(F_list_t l);

#define F_LIST_FOR_EACH(l, n) \
	for (F_list_node_t n = F_list_next(F_list_head(l));\
			!F_list_is_stub(n); n = F_list_next(n))

#define F_LIST_FOR_EACH_R(l, n) \
	for (F_list_node_t n = F_list_prev(F_list_head(l));\
			!F_list_is_stub(n); n = F_list_prev(n))

#define F_list_push(l, v) F_list_append(l, v)
#define F_list_dequeue(l) F_list_pop(l)
#define F_list_enqueue(l, v) F_list_prepend(l, v)

#define F_list_value(t, n) ((t)(F_list_data(n)))

static inline uintptr_t F_list_pop(F_list_t l)
{
	F_list_node_t last = F_list_prev(F_list_tail(l));
	if (F_list_is_stub(last))
		return 0;
	uintptr_t data = F_list_data(last);
	F_list_remove(last);
	return data;
}

#endif


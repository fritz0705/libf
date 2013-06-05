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

#include "../list.h"

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


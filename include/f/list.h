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

typedef struct list *list_t;
typedef void (*list_iterator_t)(unsigned int, void *, void *);

list_t list_new ();

void *list_append (list_t l, void *data);
void *list_prepend (list_t l, void *data);
void *list_insert (list_t l, void *data, int offset);

void *list_get (list_t l, int offset);
int list_find (list_t l, void *data);

void *list_delete (list_t l, int offset);

/* The difference between list_destroy and list_clean is simple:
 * list_clean will only remove the list nodes, while list_destroy
 * will also destroy the internal list structure. So you should only
 * use list_destroy if you will never use the list again.
 */
void list_destroy (list_t l);
void list_clean (list_t l);

int list_length (list_t l);
void list_rebuild (list_t l);

void list_iterate(list_t l, list_iterator_t iter, void *data);

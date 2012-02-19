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

struct list *list_new ();
struct list *list_init (struct list *l);
struct list *list_alloc (void *(*allocator) (int len));

void *list_append (struct list *l, void *data);
void *list_prepend (struct list *l, void *data);
void *list_insert (struct list *l, void *data, int offset);

void *list_get (struct list *l, int offset);
int list_find (struct list *l, void *data);

void *list_delete (struct list *l, int offset);

/* The difference between list_destroy and list_clean is simple:
 * list_clean will only remove the list nodes, while list_destroy
 * will also destroy the internal list structure. So you should only
 * use list_destroy if you will never use the list again.
 */
void list_destroy (struct list *l);
void list_clean (struct list *l);

int list_length (struct list *l);
void list_rebuild (struct list *l);


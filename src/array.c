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

#include <f/array.h>
#include <f/list.h>
#include <f/alloc.h>

struct array {
	size_t length;
	list_t elements;
};

array_t array_new(size_t len)
{
	array_t ary = alloc(sizeof(struct array));
	if (ary == NULL)
		return NULL;
	ary->length = len;
	ary->elements = list_new();
	if (ary->elements == NULL)
	{
		unalloc(ary);
		return NULL;
	}
	return ary;
}

void *array_add(array_t ary, void *element)
{
	if (ary == NULL || element == NULL)
		return NULL;
	void *data = alloc(ary->length);
	if (data == NULL)
		return NULL;

	libf_memcpy(data, element, ary->length);
	list_append(ary->elements, data);

	return element;
}

void array_delete(array_t ary, int offset)
{
	if (ary == NULL)
		return;
	void *data = list_get(ary->elements, offset);
	if (data != NULL)
		unalloc(data);
}

void *array_get(array_t ary, int offset)
{
	if (ary == NULL)
		return NULL;
	return list_get(ary->elements, offset);
}

int array_find(array_t ary, void *element)
{
	return -1;
}

void array_clean(array_t ary)
{
	if (ary == NULL)
		return;
	list_iterator_t i = list_iterate(ary->elements);

	while (1)
	{
		void *elem = list_iterate_next(i);
		if (elem == NULL)
			break;
		unalloc(elem);
	}
	
	list_iterate_end(i);
	list_clean(ary->elements);
}

void array_destroy(array_t ary)
{
	if (ary == NULL)
		return;
	array_clean(ary);
	list_destroy(ary->elements);
	unalloc(ary);
}


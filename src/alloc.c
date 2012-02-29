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

#include <f/_.h>
#include <f/alloc.h>

#if __STDC_HOSTED__ == 1
#include <stdlib.h>
#endif

#ifndef ALLOC_CB
#if __STDC_HOSTED__ == 1
#define ALLOC_CB malloc
#else
#define ALLOC_CB NULL
#endif
#endif

#ifndef UNALLOC_CB
#if __STDC_HOSTED__ == 1
#define UNALLOC_CB free
#else
#define UNALLOC_CB NULL
#endif
#endif

static void *(*alloc_cb)(size_t) = ALLOC_CB;
static void (*unalloc_cb)(void *) = UNALLOC_CB;

void *alloc(size_t length)
{
	if (alloc_cb == NULL)
		return NULL;
	return alloc_cb(length);
}

void unalloc(void *a)
{
	if (unalloc_cb == NULL)
		return;
	unalloc_cb(a);
}

void alloc_setup(void *(*callback)(size_t))
{
	alloc_cb = callback;
}

void unalloc_setup(void (*callback)(void *))
{
	unalloc_cb = callback;
}


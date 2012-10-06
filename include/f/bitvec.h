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
#include <f/_.h>

#ifdef LIBF_INTERNAL
struct bitvec
{
	unsigned int fields;
	char *data;
};
#endif

typedef struct bitvec *bitvec_t;

/* Initialize new bitvector with size bits */
bitvec_t bitvec_new(unsigned int size);

_Bool bitvec_get(bitvec_t bitvec, unsigned int off);

void bitvec_set(bitvec_t bitvec, unsigned int off);
void bitvec_unset(bitvec_t bitvec, unsigned int off);
void bitvec_swap(bitvec_t bitvec, unsigned int off);

bitvec_t bitvec_resize(bitvec_t bitvec, unsigned int size);

void bitvec_clear(bitvec_t bitvec);
void bitvec_destroy(bitvec_t bitvec);


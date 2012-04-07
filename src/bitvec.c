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
#include <f/bitvec.h>

struct bitvec
{
	unsigned int fields;
	char *data;
};

bitvec_t bitvec_new(unsigned int size)
{
	bitvec_t bitvec = alloc(sizeof(struct bitvec));
	if (bitvec == NULL)
		return NULL;

	if (size % 8 != 0)
		size = size + 8 - (size % 8);

	bitvec->fields = size;
	bitvec->data = alloc(size / 8);
	if (bitvec->data == NULL)
	{
		unalloc(bitvec);
		return NULL;
	}
	libf_memset(bitvec->data, 0, size / 8);

	return bitvec;
}

_Bool bitvec_get(bitvec_t bitvec, unsigned int off)
{
	if (bitvec->fields < off)
		return 0;
	unsigned int byte = off - (off % 8);
	unsigned int bit = off - byte;

	char mask = 1 << bit;
	char v = bitvec->data[off];

	return (v & mask) > 0;
}

void bitvec_set(bitvec_t bitvec, unsigned int off)
{
	if (bitvec->fields < off)
		return;

	unsigned int byte = off - (off % 8);
	unsigned int bit = off - byte;

	char mask = 1 << bit;
	char v = bitvec->data[off];

	v = v | mask;
	bitvec->data[off] = v;
}

void bitvec_unset(bitvec_t bitvec, unsigned int off)
{
	if (bitvec->fields < off)
		return;

	unsigned int byte = off - (off % 8);
	unsigned int bit = off - byte;

	char mask = ~(1 << bit);
	char v = bitvec->data[off];

	v = v & mask;
	bitvec->data[off] = v;
}

void bitvec_swap(bitvec_t bitvec, unsigned int off)
{
	if (bitvec->fields < off)
		return;

	unsigned int byte = off - (off % 8);
	unsigned int bit = off - byte;

	char mask = 1 << bit;
	char v = bitvec->data[off];

	v = v ^ mask;
	bitvec->data[off] = v;
}

void bitvec_clear(bitvec_t bitvec)
{
	libf_memset(bitvec->data, 0, bitvec->fields / 8);
}

bitvec_t bitvec_resize(bitvec_t bitvec, unsigned int size)
{
	if (size % 8 != 0)
		size = size + 8 - (size % 8);

	if (size == 0)
	{
		bitvec_destroy(bitvec);
		return NULL;
	}

	char *new_mem = alloc(size / 8);
	if (new_mem == NULL)
		return NULL;
	libf_memset(new_mem, 0, size / 8);

	unsigned int copysize = ((size / 8) > (bitvec->fields / 8)) ? (bitvec->fields / 8) : (size / 8);
	libf_memcpy(new_mem, bitvec->data, copysize);
	unalloc(bitvec->data);
	bitvec->data = new_mem;
	bitvec->fields = size;

	return bitvec;
}

void bitvec_destroy(bitvec_t bitvec)
{
	unalloc(bitvec->data);
	unalloc(bitvec);
}

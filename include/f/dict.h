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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "list.h"

#ifndef F_DICT_H_
#define F_DICT_H_

#ifdef F_DICT_STRUCTS
#ifndef F_DICT_SLOTS_COUNT
#define F_DICT_SLOTS_COUNT 8
#endif

struct F_dict
{
	uint_least16_t buckets_cnt;
	struct F_dict_bucket *buckets;
};

struct F_dict_entry
{
	uint32_t hash;
	uintptr_t data;
};

struct F_dict_bucket
{
	struct F_dict_entry slots[F_DICT_SLOTS_COUNT];
	uint_fast8_t smask;
	F_list_t burst;
};
#endif

static inline uint32_t F_dict_fnv32(unsigned char *data, size_t len)
{
	uint32_t hash = 2166136261U;
	for (size_t o = 0; o < len; ++o)
	{
		hash *= 16777619U;
		hash ^= data[o];
	}
	return hash;
}

typedef struct F_dict *F_dict_t;
typedef struct F_dict_entry *F_dict_entry_t;

#define F_DICT_1 3
#define F_DICT_2 7
#define F_DICT_3 31
#define F_DICT_4 127
#define F_DICT_5 8191
#define F_DICT_6 16381
#define F_DICT_7 32749
#define F_DICT_8 65521

F_dict_t F_dict_create(uint_least16_t buckets);

F_dict_t F_dict_setup(F_dict_t d, uint_least16_t buckets);
F_dict_t F_dict_resize(F_dict_t d, uint_least16_t buckets);

void F_dict_destroy(F_dict_t d);

const uintptr_t *F_dict_lookup(F_dict_t d, uint32_t hash);
const uintptr_t *F_dict_set(F_dict_t d, uint32_t hash, uintptr_t val);

size_t F_dict_keys(F_dict_t d, uint32_t *dst, size_t dstlen);
size_t F_dict_values(F_dict_t d, uintptr_t *dst, size_t dstlen);
size_t F_dict_entries(F_dict_t d, F_dict_entry_t dst, size_t dstlen);

size_t F_dict_length(F_dict_t d);

bool F_dict_delete(F_dict_t d, uint32_t hash);

#define F_dict_set_v(d, o, s, v) \
	(F_dict_set(d, F_dict_fnv32((unsigned char *)(o), s), v))
#define F_dict_lookup_v(d, o, s) \
	(F_dict_lookup(d, F_dict_fnv32((unsigned char *)(o), s)))
#define F_dict_delete_v(d, o, s) \
	(F_dict_delete(d, F_dict_fnv32((unsigned char *)(o), s)))

#define F_dict_set_s(d, s, v) \
	(F_dict_set(d, F_dict_fnv32((unsigned char *)(s), strlen(s)), v))
#define F_dict_lookup_s(d, s) \
	(F_dict_lookup(d, F_dict_fnv32((unsigned char *)(s), strlen(s))))

#define F_dict_contains(d, h) (F_dict_lookup(d, h) != NULL)

#ifdef F_DICT_INLINE
#undef F_DICT_INLINE
#include "dict.c"
#endif

#endif


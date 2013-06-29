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

#ifdef F_DICT_STRUCTS
#define F_LIST_STRUCTS
#endif

#include "list.h"

#ifndef F_DICT_H_
#define F_DICT_H_

#ifdef F_DICT_STRUCTS
#ifndef F_DICT_SLOTS_COUNT
#define F_DICT_SLOTS_COUNT 8
#endif

#if F_DICT_SLOTS_COUNT <= 8
#define SMASK_TYPE uint_fast8_t
#elif F_DICT_SLOTS_COUNT <= 16
#define SMASK_TYPE uint_fast16_t
#elif F_DICT_SLOTS_COUNT <= 32
#define SMASK_TYPE uint_fast32_t
#elif F_DICT_SLOTS_COUNT <= 64
#define SMASK_TYPE uint_fast64_t
#endif

struct F_dict
{
	uintptr_t buckets_cnt;
	struct F_dict_bucket *buckets;
};

struct F_dict_entry
{
	uintptr_t hash;
	uintptr_t data;
};

struct F_dict_bucket
{
	struct F_dict_entry slots[F_DICT_SLOTS_COUNT];
	SMASK_TYPE smask;
	F_list_t burst;
};

#undef SMASK_TYPE
#endif

static inline uint32_t F_dict_fnv32(const unsigned char *restrict data, size_t len)
	__attribute__((always_inline));
static inline uint32_t F_dict_fnv32(const unsigned char *restrict data, size_t len)
{
	register uint32_t hash = 2166136261U;
	for (register size_t o = 0; o < len; ++o)
	{
		hash ^= data[o];
		hash *= 16777619U;
	}
	return hash;
}

static inline uint32_t F_dict_fnv32_s(const char *restrict str)
	__attribute__((always_inline));
static inline uint32_t F_dict_fnv32_s(const char *restrict str)
{
	register uint32_t hash = 2166136261U;
	while (*str)
	{
		hash ^= *str++;
		hash *= 16777619U;
	}
	return hash;
}

static inline uint64_t F_dict_fnv64(const unsigned char *restrict data, size_t len)
	__attribute__((always_inline));
static inline uint64_t F_dict_fnv64(const unsigned char *restrict data, size_t len)
{
	register uint64_t hash = 14695981039346656037ULL;
	for (register size_t o = 0; o < len; ++o)
	{
		hash ^= data[o];
		hash *= 1099511628211ULL;
	}
	return hash;
}

static inline uint64_t F_dict_fnv64_s(const char *restrict str)
	__attribute__((always_inline));
static inline uint64_t F_dict_fnv64_s(const char *restrict str)
{
	register uint64_t hash = 14695981039346656037ULL;
	while (*str)
	{
		hash ^= *str++;
		hash *= 1099511628211ULL;
	}
	return hash;
}

#ifdef __SIZEOF_POINTER__
#if __SIZEOF_POINTER__ <= 4
#define F_DICT_FNV F_dict_fnv32
#define F_DICT_FNV_S F_dict_fnv32_s
#elif __SIZEOF_POINTER__ <= 8
#define F_DICT_FNV F_dict_fnv64
#define F_DICT_FNV_S F_dict_fnv64_s
#endif
#else
#define F_DICT_FNV F_dict_fnv32
#define F_DICT_FNV_S F_dict_fnv32_s
#endif

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

#define F_BUCKETS(n) (((n) + 1) + ((n) & 1))

F_dict_t F_dict_create(uintptr_t buckets);

F_dict_t F_dict_setup(F_dict_t restrict d, uintptr_t buckets);
F_dict_t F_dict_resize(F_dict_t restrict d, uintptr_t buckets);

void F_dict_destroy(F_dict_t restrict d);

const uintptr_t *F_dict_lookup(const F_dict_t restrict d, uintptr_t hash);
const uintptr_t *F_dict_set(const F_dict_t restrict d, uintptr_t hash, uintptr_t val);

size_t F_dict_keys(const F_dict_t restrict d, uintptr_t *restrict dst, size_t dstlen);
size_t F_dict_values(const F_dict_t restrict d, uintptr_t *restrict dst, size_t dstlen);
size_t F_dict_entries(const F_dict_t restrict d, F_dict_entry_t *restrict dst, size_t dstlen);

size_t F_dict_length(const F_dict_t restrict d);

bool F_dict_delete(const F_dict_t restrict d, uintptr_t hash);

double F_dict_loadfactor(const F_dict_t restrict d);

#define F_dict_set_v(d, o, s, v) \
	(F_dict_set(d, (uintptr_t)F_DICT_FNV((unsigned char *)(o), s), v))
#define F_dict_lookup_v(d, o, s) \
	(F_dict_lookup(d, (uintptr_t)F_DICT_FNV((unsigned char *)(o), s)))
#define F_dict_delete_v(d, o, s) \
	(F_dict_delete(d, (uintptr_t)F_DICT_FNV((unsigned char *)(o), s)))

#define F_dict_set_s(d, s, v) \
	(F_dict_set(d, (uintptr_t)F_DICT_FNV_S(s), v))
#define F_dict_lookup_s(d, s) \
	(F_dict_lookup(d, (uintptr_t)F_DICT_FNV_S(s)))
#define F_dict_delete_s(d, s) \
	(F_dict_delete(d, (uintptr_t)F_DICT_FNV_S(s)))

#define F_dict_contains(d, h) (F_dict_lookup(d, h) != NULL)

#ifdef F_DICT_INLINE
#undef F_DICT_INLINE
#include "dict.c"
#endif

#endif


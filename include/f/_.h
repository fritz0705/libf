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

#if __STDC_HOSTED__ == 0
#ifndef NULL
#define NULL ((void*)0)
#endif

typedef __UINT64_TYPE__ uint64_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT8_TYPE__ uint8_t;

typedef __INT64_TYPE__ int64_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT8_TYPE__ int8_t;

typedef __SIZE_TYPE__ size_t;

typedef _Bool bool;
#else
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#endif

char *libf_version();
_Bool libf_freestanding();

static inline void libf_memset(char *p, char val, unsigned int size)
{
#	if __STDC_HOSTED__ == 0
	for (unsigned int i = 0; i < size; ++i)
		p[i] = val;
#	else
	memset(p, val, size);
#	endif
}

static inline void libf_memcpy(char *d, char *s, unsigned int size)
{
#	if __STDC_HOSTED__ == 0
	for (unsigned int i = 0; i < size; ++i)
		d[i] = s[i];
#	else
	memcpy(d, s, size);
#	endif
}

static inline _Bool libf_memcmp(char *l, char *r, unsigned int len)
{
#	if __STDC_HOSTED__ == 0
	for (unsigned int i = 0; i < len; ++i)
		if (l[i] != r[i])
			return 0;
	return 1;
#	else
	return memcmp(l, r, len);
#	endif
}

static inline unsigned int libf_strlen(char *s)
{
#	if __STDC_HOSTED__ == 0
	unsigned int len = 0;
	while (*s != '\0') {
		++len;
		++s;
	}
	return len;
#	else
	return strlen(s);
#	endif
}

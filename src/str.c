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

#include <f/_.h>
#include <f/str.h>
#include <f/list.h>
#include <f/alloc.h>

#include "str.h"

static inline unsigned int __strlen(char *s)
{
	unsigned int len = 0;
	while (*s != '\0') {
		++len;
		++s;
	}
	return len;
}

static inline unsigned int __memcmp(char *l, char *r, int len)
{
	for (int i = 0; i < len; ++i)
		if (l[i] != r[i])
			return 0;
	return 1;
}

static unsigned int getabsoffset(str_t str, int offset)
{
	if (offset < 0)
		return str_length(str) + offset;

	return offset;
}

str_t str_new()
{
	str_t str = alloc(sizeof(struct str));
	if (str == NULL)
		return NULL;

	str->chunks = list_new();
	str->frozen = 0;

	return str;
}

str_t str_create(str_t oldstr)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	if (str_append(str, oldstr) == NULL)
	{
		str_destroy(str);
		return NULL;
	}

	return str;
}

/* Create from C String */
str_t str_create_cs(char *cs)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	if (str_append_cs(str, cs) == NULL)
	{
		str_destroy(str);
		return NULL;
	}

	return str;
}

/* Create from char */
str_t str_create_c(char c)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	if (str_append_c(str, c) == NULL)
	{
		str_destroy(str);
		return NULL;
	}

	return str;
}

str_t str_create_r(void	*d, unsigned int length)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	if (str_append_r(str, d, length) == NULL)
	{
		str_destroy(str);
		return NULL;
	}
	
	return str;
}

str_t str_join(str_t left, str_t right)
{
	str_t newstr = str_new();
	if (newstr == NULL)
		return NULL;

	int chunks, i;

	chunks = list_length(left->chunks);
	for (i = 0; i < chunks; ++i)
	{
		struct str_chunk *c = list_get(left->chunks, i);
		usechunk(c);
		list_append(newstr->chunks, c);
	}

	chunks = list_length(right->chunks);
	for (i = 0; i < chunks; ++i)
	{
		struct str_chunk *c = list_get(right->chunks, i);
		usechunk(c);
		list_append(newstr->chunks, c);
	}

	return newstr;
}

/* Build a new string with only one chunk */
str_t str_normalize(str_t str)
{
	char *tmp = str_dump(str);
	if (tmp == NULL)
		return NULL;

	str_t newstr = str_new();
	if (newstr == NULL)
	{
		unalloc(tmp);
		return NULL;
	}

	struct str_chunk *c = newchunk(tmp, str_length(str));
	unalloc(tmp);
	if (c == NULL)
	{
		str_destroy(newstr);
		return NULL;
	}

	list_append(newstr->chunks, c);

	return newstr;
}

str_t str_sub(str_t str, int offset, unsigned int length)
{
	char *tmp = str_dump(str);
	if (tmp == NULL)
		return NULL;

	str_t newstr = str_new();
	if (newstr == NULL)
	{
		unalloc(tmp);
		return NULL;
	}

	unsigned int absoffset = getabsoffset(str, offset);

	struct str_chunk *newc = newchunk(tmp + absoffset, length);
	if (newc == NULL)
	{
		unalloc(tmp);
		unalloc(newstr);
		return NULL;
	}

	list_append(newstr->chunks, newc);

	unalloc(tmp);
	return newstr;
}

void str_trim(str_t str, int offset)
{
	unsigned int absoffset = getabsoffset(str, offset);
	
	unsigned int data_length = str_length(str);
	char *data = str_dump(str);
	if (data == NULL)
		return;
	
	str_clean(str);

	if (data_length > absoffset)
		str_append_r(str, data + absoffset, data_length - absoffset);
	unalloc(data);
}

char *str_dump(str_t str)
{
	unsigned int length = str_length(str);
	char *dump = (char *)alloc(length + 1);
	if (dump == NULL)
		return NULL;
	dump[length] = 0;

	char *offset = dump;
	list_iterator_t i = list_iterate(str->chunks);
	if (i == NULL)
	{
		unalloc(dump);
		return NULL;
	}

	while (1)
	{
		struct str_chunk *c = list_iterate_next(i);
		if (c == NULL)
			break;
		for (unsigned int off = 0; off < c->length; ++off)
			offset[off] = c->data[off];
		offset += c->length;
	}

	list_iterate_end(i);

	return dump;
}

unsigned int str_length(str_t str)
{
	unsigned int length = 0;
	list_iterator_t i = list_iterate(str->chunks);
	if (i == NULL)
		return 0;

	while (1)
	{
		struct str_chunk *c = list_iterate_next(i);
		if (c == NULL)
			break;
		length += c->length;
	}

	list_iterate_end(i);
	return length;
}

void str_clean(str_t str)
{
	list_iterator_t i = list_iterate(str->chunks);
	if (i == NULL)
		goto clean;

	while (1)
	{
		struct str_chunk *c = list_iterate_next(i);
		if (c == NULL)
			break;
		unallocchunk(c);
	}

	list_iterate_end(i);
clean:
	list_clean(str->chunks);
}

void str_destroy(str_t str)
{
	str_clean(str);
	list_destroy(str->chunks);
	unalloc(str);
}

char str_get(str_t str, int offset)
{
	unsigned int absoffset = getabsoffset(str, offset);

	char *cstr = str_dump(str);
	if (cstr == NULL)
		return 0;

	char retval = cstr[absoffset];

	unalloc(cstr);
	return retval;
}

str_t str_append_r(str_t str, void *d, unsigned int length)
{
	if (str_frozen(str))
		return NULL;

	struct str_chunk *chunk = newchunk(d, length);
	if (chunk == NULL)
		return NULL;

	list_append(str->chunks, chunk);
	return str;
}

str_t str_append_c(str_t str, char c)
{
	if (str_frozen(str))
		return NULL;

	struct str_chunk *chunk = newchunk(&c, 1);
	if (chunk == NULL)
		return NULL;

	list_append(str->chunks, chunk);
	return str;
}

str_t str_append_cs(str_t str, char *cs)
{
	if (str_frozen(str))
		return NULL;

	struct str_chunk *chunk = newchunk(cs, __strlen(cs));
	if (chunk == NULL)
		return NULL;

	list_append(str->chunks, chunk);
	return str;
}

str_t str_append(str_t str, str_t right)
{
	if (str_frozen(str))
		return NULL;

	unsigned int chunks = list_length(right->chunks);
	for (unsigned int i = 0; i < chunks; ++i)
	{
		struct str_chunk *c = list_get(right->chunks, i);
		usechunk(c);
		list_append(str->chunks, c);
	}

	return str;
}

int str_freeze(str_t str)
{
	str->frozen = 1;
	return 1;
}

int str_frozen(str_t str)
{
	return str->frozen;
}

static inline char _to_digit(char n)
{
	return (n < 10 ? '0' : 'a' - 10) + n;
}

str_t str_append_i(str_t str, int num, unsigned int base)
{
	if (str_frozen(str))
		return NULL;
	if (num == 0)
		return str_append_c(str, '0');

	_Bool negative = 0;
	if (num < 0)
	{
		negative = 1;
		num = -num;
	}
	char buffer[__SIZEOF_INT__ * 8 + 1];
	char *res = buffer + __SIZEOF_INT__ * 8;

	*res = 0;
	--res;

	while ((num > 0) && res >= buffer)
	{
		*res = _to_digit(num % base);
		--res;
		num /= base;
	}

	if (negative)
		str_append_c(str, '-');

	return str_append_cs(str, res + 1);
}

str_t str_append_ui(str_t str, unsigned int num, unsigned int base)
{
	if (str_frozen(str))
		return NULL;
	if (num == 0)
		return str_append_c(str, '0');

	char buffer[__SIZEOF_INT__ * 8 + 1];
	char *res = buffer + __SIZEOF_INT__ * 8;

	*res = 0;
	--res;

	while ((num > 0) && res >= buffer)
	{
		*res = _to_digit(num % base);
		--res;
		num /= base;
	}

	return str_append_cs(str, res + 1);
}

str_t str_create_i(int num, unsigned int base)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	if (str_append_i(str, num, base) == NULL)
	{
		unalloc(str);
		return NULL;
	}

	return str;
}

str_t str_create_ui(unsigned int num, unsigned int base)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	if (str_append_ui(str, num, base) == NULL)
	{
		str_destroy(str);
		return NULL;
	}

	return str;
}

int str_offset(str_t str, char c)
{
	char *dump = str_dump(str);
	if (dump == NULL)
		return -1;

	unsigned int len = str_length(str);
	for (unsigned int i = 0; i < len; ++i)
	{
		if (dump[i] == c)
		{
			unalloc(dump);
			return i;
		}
	}

	unalloc(dump);
	return -1;
}

int str_find(str_t str, char *c)
{
	char *dump = str_dump(str);
	if (dump == NULL)
		return -1;

	int offset = 0;
	unsigned int len = __strlen(c);
	int limit = str_length(str) - len + 1;

	if (len > str_length(str))
	{
		unalloc(dump);
		return -1;
	}
	for (; offset < limit; ++offset)
	{
		if (__memcmp(dump	+ offset, c, len))
		{
			unalloc(dump);
			return offset;
		}
	}

	unalloc(dump);
	return -1;
}


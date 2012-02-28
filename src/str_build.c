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

#include <f/str.h>
#include "str.h"

#include <stdlib.h>
#include <stdarg.h>

str_t str_build(str_t str, ...)
{
	str_t newstr = str_new();
	if (newstr == NULL)
		return NULL;

	str_append(newstr, str);

	va_list ap;
	va_start(ap, str);

	while (1)
	{
		str_t cstr = va_arg(ap, str_t);
		if (cstr == NULL)
			break;
		str_append(newstr, cstr);
	}

	va_end(ap);

	return newstr;
}

str_t str_build_cs(char *cs, ...)
{
	str_t newstr = str_new();
	if (newstr == NULL)
		return NULL;

	str_append_cs(newstr, cs);

	va_list ap;
	va_start(ap, cs);

	while (1)
	{
		char *ccs = va_arg(ap, char *);
		if (ccs == NULL)
			break;
		str_append_cs(newstr, ccs);
	}

	va_end(ap);

	return newstr;
}

str_t str_build_r(void *d, unsigned int length, ...)
{
	str_t newstr = str_new();
	if (newstr == NULL)
		return NULL;

	str_append_r(newstr, d, length);

	va_list ap;
	va_start(ap, length);

	while (1)
	{
		d = va_arg(ap, void *);
		if (d == NULL)
			break;
		length = va_arg(ap, unsigned int);

		str_append_r(newstr, d, length);
	}

	va_end(ap);

	return newstr;
}

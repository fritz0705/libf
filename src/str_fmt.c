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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

str_t str_create_fmt(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	return str_create_vfmt(fmt, ap);
}

str_t str_create_vfmt(const char *fmt, va_list ap)
{
	int buflen = vsnprintf(NULL, 0, fmt, ap);
	if (buflen < 0)
		return NULL;

	char *buf = malloc(buflen);
	if (buf == NULL)
		return NULL;
	if (vsnprintf(buf, buflen, fmt, ap) < 0)
	{
		free(buf);
		return NULL;
	}

	str_t str = str_create_r(buf, buflen);
	free(buf);

	return str;
}

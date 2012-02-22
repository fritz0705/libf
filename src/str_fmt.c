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

str_t str_append_fmt(str_t str, const char *fmt, ...)
{
	if (str_frozen(str))
		return NULL;

	va_list ap;
	va_start(ap, fmt);

	str_t retstr = str_append_vfmt(str, fmt, ap);

	va_end(ap);

	return retstr;
}

str_t str_append_vfmt(str_t str, const char *fmt, va_list ap)
{
	if (str_frozen(str))
		return NULL;

	va_list aq;
	va_copy(aq, ap);
	int buflen = vsnprintf(NULL, 0, fmt, aq);
	if (buflen < 0)
		return NULL;
	va_end(aq);

	buflen += 1;
	char *buf = malloc(buflen);
	if (buf == NULL)
		return NULL;
	if (vsnprintf(buf, buflen, fmt, ap) < 0)
	{
		free(buf);
		return NULL;
	}

	str_t retstr = str_append_r(str, buf, buflen);
	free(buf);

	return retstr;
}

str_t str_create_fmt(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	str_t str = str_new();
	if (str_append_vfmt(str, fmt, ap) == NULL)
	{
		str_destroy(str);
		str = NULL;
	}

	va_end(ap);

	return str;
}

str_t str_create_vfmt(const char *fmt, va_list ap)
{
	str_t str = str_new();

	if (str_append_vfmt(str, fmt, ap) == NULL)
	{
		str_destroy(str);
		str = NULL;
	}

	return str;
}

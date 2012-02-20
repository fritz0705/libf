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

#include <unistd.h>
#include <stdlib.h>

str_t str_io_readline(int fd)
{
	str_t str = str_new();
	if (str == NULL)
		return NULL;

	char c = 0;
	unsigned int i = 0;
	while (1)
	{
		if (read(fd, &c, 1) == -1)
		{
			str_destroy(str);
			return NULL;
		}

		if (c == '\n' || c == '\r')
		{
			if (i != 0)
				break;
			else
				continue;
		}

		str_t cstr = str_create_c(c);
		if (cstr == NULL)
		{
			str_destroy(str);
			return NULL;
		}
		str_t newstr = str_join(str, str_create_c(c));
		if (newstr == NULL)
		{
			str_destroy(cstr);
			str_destroy(newstr);
			return NULL;
		}
		str_destroy(str);
		str_destroy(cstr);

		str = newstr;

		++i;
	}
	
	return str;
}

str_t str_io_read(int fd, unsigned int octets)
{
	char *buf = malloc(octets);
	if (buf == NULL)
		return NULL;

	int bytes = read(fd, buf, octets);
	if (bytes < 0)
	{
		free(buf);
		return NULL;
	}

	str_t str = str_create_r(buf, bytes);
	
	free(buf);
	return str;
}

int str_io_write(int fd, str_t str)
{
	char *tmp = str_dump(str);
	if (tmp == NULL)
		return -1;

	int retval = write(fd, tmp, str_length(str));
	free(tmp);

	return retval;
}

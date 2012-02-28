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
#include <f/alloc.h>
#include "str.h"

/* Here we will compare the two strings `left` and `right` */
_Bool str_cmp(str_t left, str_t right)
{
	/* If the two strings have different lengths then they are not equal */
	if (str_length(left) != str_length(right))
		return 0;

	/* Dump the two strings */
	char *left_dump = str_dump(left);
	if (left_dump == NULL) return 0;
	char *right_dump = str_dump(right);
	if (right_dump == NULL) { unalloc(left_dump); return 0; }

	int result = 1;
	for (unsigned int i = 0; i < str_length(right); ++i)
		if (left_dump[i] != right_dump[i])
		{
			result = 0;
			break;
		}

	unalloc(left_dump);
	unalloc(right_dump);

	return result;
}

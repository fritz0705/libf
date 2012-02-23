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

#include <string.h>
#include <stdlib.h>

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
	if (right_dump == NULL) { free(left_dump); return 0; }

	int result = memcmp(left_dump, right_dump, str_length(right));

	free(left_dump);
	free(right_dump);
	
	if (result == 0)
		return 1;

	return 0;
}

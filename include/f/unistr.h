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

#include <f/_.h>

typedef struct unistr *unistr_t;
typedef uint32_t unistr_point_t;

enum unistr_enc {
	UNISTR_DEFAULT;
	UNISTR_UTF8;
	UNISTR_UTF16;
	UNISTR_UTF16_LE;
	UNISTR_UTF16_BE;
	UNISTR_UTF32;
	UNISTR_UTF32_LE;
	UNISTR_UTF32_BE;
};

unistr_t unistr_new();

unistr_t unistr_create(unistr_t oldstr);
unistr_t unistr_append(unistr_t str, unistr_t right);

unistr_t unistr_create_s(str_t str, enum unistr_enc enc);
unistr_t unistr_append_s(unistr_t str, str_t right, enum unistr_enc enc);

unistr_t unistr_create_cs(char *cs, enum unistr_enc enc);
unistr_t unistr_append_cs(unistr_t str, char *cs, enum unistr_enc enc);

unistr_t unistr_create_cp(unistr_point_t point);
unistr_t unistr_append_cp(unistr_t str, unistr_point_t point);

unistr_t unistr_create_i(int num, unsigned int base);
unistr_t unistr_append_i(unistr_t str, int num, unsigned int base);

unistr_t unistr_create_ui(unsigned int num, unsigned int base);
unistr_t unistr_append_ui(unistr_t str, unsigned int num, unsigned int base);

unistr_t unistr_replace(unistr_t str, unistr_point_t pat, unistr_point_t rep);

unistr_point_t unistr_get(unistr_t str, int offset);
int unistr_offset(unistr_t str, unistr_point_t point);
int unistr_offset_nth(unistr_t str, unistr_point_t point, int occ);

str_t unistr_dump(unistr_t str, enum unistr_enc enc);
char *unistr_dump_cs(unistr_t str, enum unistr_enc enc);

unsigned int unistr_length(unistr_t str);

_Bool unistr_cmp(unistr_t left, unistr_t right);

int unistr_freeze(unistr_t str);
int unistr_frozen(unistr_t str);

void unistr_clean(unistr_t str);
void unistr_destroy(unistr_t str);


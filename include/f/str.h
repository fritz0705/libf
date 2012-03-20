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

/* You will only handle pointers to string objects, therefore you should use str_t
 * instead of struct str *
 */
typedef struct str *str_t;

/* Initialize empty string object */
str_t str_new();

str_t str_create(str_t oldstr);
/* Create string object from C string cs */
str_t str_create_cs(char *cs);
/* Create string object from char c */
str_t str_create_c(char c);
/* Create string object from raw data at position d with length */
str_t str_create_r(void *d, unsigned int length);

str_t str_create_i(int num, unsigned int base);
str_t str_create_ui(unsigned int num, unsigned int base);

/* Build new string object consisting of left and right */
str_t str_join(str_t left, str_t right);
str_t str_sub(str_t str, int offset, unsigned int length);
void str_trim(str_t str, int offset);

str_t str_append(str_t str, str_t right);
str_t str_append_cs(str_t str, char *cs);
str_t str_append_c(str_t str, char c);
str_t str_append_r(str_t str, void *d, unsigned int length);

str_t str_append_i(str_t str, int num, unsigned int base);
str_t str_append_ui(str_t str, unsigned int num, unsigned int base);

char str_get(str_t str, int offset);
int str_offset(str_t str, char c);

char *str_dump(str_t str);
str_t str_normalize(str_t str);
unsigned int str_length(str_t str);

_Bool str_cmp(str_t left, str_t right);

int str_freeze(str_t str);
int str_frozen(str_t str);

void str_clean(str_t str);
void str_destroy(str_t str);

#if __STDC_HOSTED__ == 1
#include <stdarg.h>

/* Create string object using vsnprintf(3). Please consider, that this method is
 * not a good style!
 */
str_t str_create_fmt(const char *fmt, ...);
str_t str_create_vfmt(const char *fmt, va_list ap);

str_t str_append_fmt(str_t str, const char *fmt, ...);
str_t str_append_vfmt(str_t str, const char *fmt, va_list ap);

str_t str_build(str_t str, ...);
str_t str_build_cs(char *cs, ...);
str_t str_build_r(void *d, unsigned int length, ...);

int str_io_write(int fd, str_t str);
int str_io_writeline(int fd, str_t str);

str_t str_io_readline(int fd);
str_t str_io_read(int fd, unsigned int octets);
int str_io_readin(int fd, unsigned int octets, str_t str);

/* Scatter/gather functionality; you have to terminate the argument list with NULL */
int str_io_writev(int fd, str_t str, ...);

#endif

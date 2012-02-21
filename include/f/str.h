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

/* You will only handle pointers to string objects, therefore you should use str_t
 * instead of struct str *
 */
typedef struct str *str_t;

/* Initialize empty string object */
str_t str_new();

/* Create string object from C string cs */
str_t str_create_cs(char *cs);
/* Create string object from char c */
str_t str_create_c(char c);
/* Create string object from raw data at position d with length */
str_t str_create_r(char *d, unsigned int length);

str_t str_create_i(int num, unsigned int base);
str_t str_create_ui(unsigned int num, unsigned int base);

/* Create string object using vsnprintf(3). Please consider, that this method is
 * not a good style!
 */
str_t str_create_fmt(const char *fmt, ...);

/* Build new string object consisting of left and right */
str_t str_join(str_t left, str_t right);
str_t str_sub(str_t str, int offset, unsigned int length);

char str_get(str_t str, int offset);

char *str_dump(str_t str);
str_t str_normalize(str_t str);
unsigned int str_length(str_t str);

str_t str_io_readline(int fd);
str_t str_io_read(int fd, unsigned int octets);
int str_io_write(int fd, str_t str);

void str_destroy(str_t str);

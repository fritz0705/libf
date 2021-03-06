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

#include "../dict.h"

size_t F_dict_values(F_dict_t d, uintptr_t *dst, size_t dstlen)
{
	size_t max = dstlen / sizeof *dst;
	size_t written = 0;

	if (max == 0)
		return 0;

	for (size_t bucket_off = 0; bucket_off < d->buckets_cnt; ++bucket_off)
	{
		struct F_dict_bucket *bucket = &d->buckets[bucket_off];

		for (size_t slot_off = 0; slot_off < F_DICT_SLOTS_COUNT; ++slot_off)
			if (bucket->smask & (1 << slot_off))
			{
				if (written == max)
					goto no_space;
				dst[written++] = bucket->slots[slot_off].data;
			}

		if (!bucket->burst)
			continue;
		F_LIST_FOR_EACH(bucket->burst, node)
		{
			struct F_dict_entry *e = F_list_value(struct F_dict_entry *, node);
			if (written == max)
				goto no_space;
			dst[written++] = e->data;
		}
	}

no_space:
	return written;
}


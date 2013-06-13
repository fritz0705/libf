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

const uintptr_t *F_dict_set(F_dict_t d, uintptr_t hash, uintptr_t val)
{
	struct F_dict_bucket *bucket = &d->buckets[BUCKET(d, hash)];
	uint8_t slot = SLOT(hash);

	if (!(bucket->smask & (1 << slot)))
	{
		bucket->smask |= (1 << slot);
		bucket->slots[slot] = (struct F_dict_entry){
			.hash = hash,
			.data = val
		};
		return &bucket->slots[slot].data;
	}

	if (bucket->slots[slot].hash == hash)
	{
		bucket->slots[slot].data = val;
		return &bucket->slots[slot].data;
	}

	struct F_dict_entry *entry = NULL;

	if (!bucket->burst)
		bucket->burst = F_list_create();
	if (!bucket->burst)
		return NULL;
	F_LIST_FOR_EACH(bucket->burst, e)
	{
		if (F_list_value(struct F_dict_entry *, e)->hash == hash)
			entry = F_list_value(struct F_dict_entry *, e);
	}

	if (entry)
	{
		entry->data = val;
		return &entry->data;
	}

	entry = malloc(sizeof *entry);
	*entry = (struct F_dict_entry){
		.hash = hash,
		.data = val
	};

	F_list_append(bucket->burst, (uintptr_t)entry);
	return &entry->data;
}

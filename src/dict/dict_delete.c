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

bool F_dict_delete(F_dict_t d, uintptr_t hash)
{
	struct F_dict_bucket *bucket = &d->buckets[BUCKET(d, hash)];
	uint8_t slot = SLOT(hash);

	if (!(bucket->smask & (1 << slot)))
		return false;

	if (bucket->slots[slot].hash == hash)
	{
		F_list_node_t node = NULL;
		if (bucket->burst)
			F_LIST_FOR_EACH(bucket->burst, e)
				if (F_list_value(struct F_dict_entry *, e)->hash == hash)
					node = e;

		if (node)
		{
			bucket->slots[slot] = *F_list_value(struct F_dict_entry *, node);
			free(F_list_value(struct F_dict_entry *, node));
			F_list_remove(node);
		}
		else
			bucket->smask &= ~(1 << slot);

		return true;
	}

	if (!bucket->burst)
		return false;

	F_list_node_t node = NULL;
	F_LIST_FOR_EACH(bucket->burst, e)
	{
		if (F_list_value(struct F_dict_entry *, e)->hash == hash)
			node = e;
	}

	if (!node)
		return false;

	bucket->slots[slot] = *F_list_value(struct F_dict_entry *, node);
	free(F_list_value(struct F_dict_entry *, node));
	F_list_remove(node);

	return true;
}

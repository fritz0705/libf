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

#define F_DICT_STRUCTS

#include <f/dict.h>

#define SLOT(h) (((h) * (F_DICT_SLOTS_COUNT - 1)) % F_DICT_SLOTS_COUNT)
#define BUCKET(d, h) ((h) % (d)->buckets_cnt)

F_dict_t F_dict_create(uint_least16_t buckets)
{
	F_dict_t d = malloc(sizeof *d);
	if (!d)
		return NULL;

	return F_dict_setup(d, buckets);
}

F_dict_t F_dict_setup(F_dict_t d, uint_least16_t buckets)
{
	d->buckets_cnt = buckets;
	d->buckets = malloc(sizeof *d->buckets * buckets);

	if (!d->buckets)
	{
		free(d);
		return NULL;
	}

	for (size_t o = 0; o < buckets; ++o)
	{
		d->buckets[o] = (struct F_dict_bucket){
			.smask = 0,
			.burst = NULL
		};
	}

	return d;
}

const uintptr_t *F_dict_lookup(F_dict_t d, uint32_t hash)
{
	struct F_dict_bucket *bucket = &d->buckets[BUCKET(d, hash)];
	uint8_t slot = SLOT(hash);

	struct F_dict_entry *entry = NULL;
	if (bucket->smask & (1 << slot))
		entry = &bucket->slots[slot];
	else
		return NULL;

	if (entry->hash != hash)
	{
		if (!bucket->burst)
			return NULL;
		entry = NULL;
		F_LIST_FOR_EACH(bucket->burst, node)
		{
			F_dict_entry_t e = F_list_value(F_dict_entry_t, node);
			if (e->hash == hash)
				entry = e;
		}
	}

	if (entry == NULL)
		return NULL;

	return &entry->data;
}

const uintptr_t *F_dict_set(F_dict_t d, uint32_t hash, uintptr_t val)
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

bool F_dict_delete(F_dict_t d, uint32_t hash)
{
	struct F_dict_bucket *bucket = &d->buckets[BUCKET(d, hash)];
	uint8_t slot = SLOT(hash);

	if (bucket->smask & (1 << slot))
		return false;

	if (bucket->slots[slot].hash == hash)
	{
		F_list_node_t node = NULL;
		F_LIST_FOR_EACH(bucket->burst, e)
		{
			if (F_list_value(struct F_dict_entry *, e)->hash == hash)
				node = e;
		}

		if (node)
		{
			bucket->slots[slot] = *F_list_value(struct F_dict_entry *, node);
			free(F_list_value(struct F_dict_entry *, node));
			F_list_remove(node);
		}
		else
			bucket->smask -= (1 << slot);

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

size_t F_dict_keys(F_dict_t d, uint32_t *dst, size_t dstlen)
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
				dst[written++] = bucket->slots[slot_off].hash;
			}

		if (!bucket->burst)
			continue;
		F_LIST_FOR_EACH(bucket->burst, node)
		{
			struct F_dict_entry *e = F_list_value(struct F_dict_entry *, node);
			if (written == max)
				goto no_space;
			dst[written++] = e->hash;
		}
	}

no_space:
	return written;
}

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

size_t F_dict_entries(F_dict_t d, F_dict_entry_t *dst, size_t dstlen)
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
				dst[written++] = &bucket->slots[slot_off];
			}

		if (!bucket->burst)
			continue;
		F_LIST_FOR_EACH(bucket->burst, node)
		{
			struct F_dict_entry *e = F_list_value(struct F_dict_entry *, node);
			if (written == max)
				goto no_space;
			dst[written++] = e;
		}
	}

no_space:
	return written;
}

size_t F_dict_length(F_dict_t d)
{
	size_t len = 0;

	for (size_t bucket_off = 0; bucket_off < d->buckets_cnt; ++bucket_off)
	{
		struct F_dict_bucket *bucket = &d->buckets[bucket_off];

		for (size_t slot_off = 0; slot_off < F_DICT_SLOTS_COUNT; ++slot_off)
			if (bucket->smask & (1 << slot_off))
				++len;

		if (!bucket->burst)
			continue;
		F_LIST_FOR_EACH(bucket->burst, node)
		{
			++len;
		}
	}

	return len;
}


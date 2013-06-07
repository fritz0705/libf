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

F_dict_t F_dict_resize(F_dict_t dict, uint_least16_t buckets)
{
	/* dict == NULL || dict->buckets_cnt == buckets */
	if (!dict || dict->buckets_cnt == buckets)
		return dict;

	if (buckets > dict->buckets_cnt)
	{
		/* Grow dictionary */
		dict->buckets = realloc(dict->buckets, buckets * sizeof *dict->buckets);
		for (size_t i = dict->buckets_cnt; i < buckets; ++i)
			dict->buckets[i] = (struct F_dict_bucket){
				.smask = 0,
				.burst = NULL
			};

		F_list_t reinsert = F_list_create();

		for (size_t bucket_off = 0; bucket_off < dict->buckets_cnt; ++bucket_off)
		{
			struct F_dict_bucket *bucket = &dict->buckets[bucket_off];

			for (size_t slot = 0; slot < F_DICT_SLOTS_COUNT; ++slot)
			{
				if (!(bucket->smask & (1 << slot)))
					continue;
				struct F_dict_entry *entry = &bucket->slots[slot];

				uint32_t new_bucket_off = entry->hash % buckets;

				if (new_bucket_off < dict->buckets_cnt && new_bucket_off >= bucket_off)
				{
					struct F_dict_entry *tmp_entry = malloc(sizeof *tmp_entry);
					*tmp_entry = *entry;
					F_list_append(reinsert, (uintptr_t)tmp_entry);
				}
				else
				{
					/* In right split */

					struct F_dict_bucket *new_bucket = &dict->buckets[new_bucket_off];
					if (new_bucket->smask & (1 << slot))
					{
						/* Push in burst */
						if (!new_bucket->burst)
							new_bucket->burst = F_list_create();

						struct F_dict_entry *burst_entry = malloc(sizeof *burst_entry);
						*burst_entry = *entry;

						F_list_append(new_bucket->burst, (uintptr_t)burst_entry);
					}
					else
					{
						/* Put in slot and set mask */
						new_bucket->smask |= (1 << slot);
						new_bucket->slots[slot] = *entry;
					}
				}
			}

			if (bucket->burst)
			{
				F_LIST_FOR_EACH(bucket->burst, node)
				{
					struct F_dict_entry *entry = F_list_value(struct F_dict_entry *, node);

					uint32_t new_bucket_off = entry->hash % buckets;
					size_t slot = SLOT(entry->hash);

					if (new_bucket_off < dict->buckets_cnt)
						F_list_append(reinsert, (uintptr_t)entry);
					else
					{
						struct F_dict_bucket *new_bucket = &dict->buckets[new_bucket_off];
						if (new_bucket->smask & (1 << slot))
						{
							/* Push in burst */
							if (!new_bucket->burst)
								new_bucket->burst = F_list_create();

							F_list_append(new_bucket->burst, (uintptr_t)entry);
						}
						else
						{
							new_bucket->smask |= (1 << slot);
							new_bucket->slots[slot] = *entry;
							free(entry);
						}
					}
				}

				F_list_destroy(bucket->burst);
				bucket->burst = NULL;
			}

			bucket->smask = 0;
		}

		F_LIST_FOR_EACH(reinsert, node)
		{
			struct F_dict_entry *entry = F_list_value(struct F_dict_entry *, node);

			uint32_t new_bucket_off = entry->hash % buckets;
			size_t slot = SLOT(entry->hash);
			struct F_dict_bucket *new_bucket = &dict->buckets[new_bucket_off];

			if (new_bucket->smask & (1 << slot))
			{
				if (!new_bucket->burst)
					new_bucket->burst = F_list_create();

				F_list_append(new_bucket->burst, (uintptr_t)entry);
			}
			else
			{
				new_bucket->smask |= (1 << slot);
				new_bucket->slots[slot] = *entry;
				free(entry);
			}
		}

		F_list_destroy(reinsert);
		reinsert = NULL;

		dict->buckets_cnt = buckets;
	}
	else
	{
		/* Shrink dictionary */
		F_dict_t new_dict = F_dict_create(buckets);

		for (size_t bucket_off = 0; bucket_off < dict->buckets_cnt; ++bucket_off)
		{
			struct F_dict_bucket *bucket = &dict->buckets[bucket_off];

			for (size_t slot = 0; slot < F_DICT_SLOTS_COUNT; ++slot)
			{
				if (!(bucket->smask & (1 << slot)))
					continue;

				struct F_dict_entry *entry = &bucket->slots[slot];
				F_dict_set(new_dict, entry->hash, entry->data);
			}

			if (bucket->burst)
			{
				F_LIST_FOR_EACH(bucket->burst, node)
				{
					struct F_dict_entry *entry = F_list_value(struct F_dict_entry *, node);
					F_dict_set(new_dict, entry->hash, entry->data);
				}
			}
		}

		F_dict_destroy(dict);
		dict = new_dict;
	}

	return dict;
}

#define _GNU_SOURCE
#define F_DICT_STRUCTS

#include <f/dict.h>
#include <stdio.h>
#include <time.h>

struct test
{
	const char *description;
	const char *funcname;
	char *errmsg;
	int (*func)(char **);
	struct timespec start;
	struct timespec stop;

	int retval;
	bool executed:1;
};

#define REGISTER_TEST(_d, _descr, _f) {\
		struct test *__test = malloc(sizeof *__test);\
		*__test = (struct test){\
			.description = _descr,\
			.funcname = #_f,\
			.errmsg = NULL,\
			.func = _f,\
			.start = {0, 0},\
			.stop = {0, 0},\
			.retval = 0,\
			.executed = 0,\
		};\
		F_dict_set_s(_d, #_f, (uintptr_t)__test);\
	}

static int test_dict_1(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_4);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set(d, i * (F_DICT_4 - 1), i);

	F_dict_destroy(d);

	return 0;
}

static int test_dict_2(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_2);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set(d, i * (F_DICT_2 - 1), i);

	F_dict_destroy(d);

	return 0;
}

static int test_dict_3(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_4);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set(d, i * (F_DICT_4 - 1), i);

	for (size_t i = 0; i < 1024; ++i)
		if (F_dict_lookup(d, i * (F_DICT_4 - 1)) == NULL)
		{
			asprintf(errmsg, "Lookup at %d(%d) failed: Got %d", i * (F_DICT_4 - 1), i,
				F_dict_lookup(d, i));
			F_dict_destroy(d);
			return 1;
		}

	F_dict_destroy(d);

	return 0;
}

static int test_dict_4(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_2);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set(d, i * (F_DICT_2 - 1), i);

	for (size_t i = 0; i < 1024; ++i)
		if (F_dict_lookup(d, i * (F_DICT_2 - 1)) == NULL)
		{
			asprintf(errmsg, "Lookup at %d(%d) failed: Got %d", i * (F_DICT_2 - 1), i,
				F_dict_lookup(d, i));
			F_dict_destroy(d);
			return 1;
		}

	F_dict_destroy(d);

	return 0;
}

static int test_dict_5(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_6);
	for (size_t i = 0; i < 65536; ++i)
		F_dict_set(d, i * (F_DICT_6 - 1), i);

	F_dict_destroy(d);

	return 0;
}

static int test_dict_6(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_2);
	for (size_t i = 0; i < 65536; ++i)
		F_dict_set(d, i * (F_DICT_2 - 1), i);

	F_dict_destroy(d);

	return 0;
}

static int test_dict_7(char **errmsg)
{
	F_dict_t d = F_dict_create(F_DICT_6);
	for (size_t i = 0; i < 65536; ++i)
		F_dict_set(d, i * (F_DICT_6 - 1), i);

	for (size_t i = 0; i < 65536; ++i)
		if (F_dict_lookup(d, i * (F_DICT_6 - 1)) == NULL)
		{
			asprintf(errmsg, "Lookup at %d(%d) failed: Got %d", i * (F_DICT_6 - 1), i,
				F_dict_lookup(d, i));
			return 1;
		}

	F_dict_destroy(d);

	return 0;
}

static int test_dict_8(char **errmsg)
{
	F_dict_t d = F_dict_create(1024);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set_v(d, &i, sizeof i, i);

	F_dict_destroy(d);

	return 0;
}

static int test_dict_9(char **errmsg)
{
	F_dict_t d = F_dict_create(16);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set_v(d, &i, sizeof i, i);

	for (size_t i = 0; i < 1024; ++i)
		if (!F_dict_lookup_v(d, &i, sizeof i))
		{
			asprintf(errmsg, "Lookup for %d failed", i);
			return 1;
		}

	F_dict_destroy(d);

	return 0;
}

static int test_dict_10(char **errmsg)
{
	F_dict_t d = F_dict_create(8);
	for (size_t i = 0; i < 1024; ++i)
		F_dict_set_v(d, &i, sizeof i, i);

	for (size_t i = 0; i < 1024; ++i)
		if (!F_dict_lookup_v(d, &i, sizeof i))
		{
			asprintf(errmsg, "Lookup for %d failed", i);
			return 1;
		}

	F_dict_destroy(d);

	return 0;
}

static int test_dict_11(char **errmsg)
{
	F_dict_t d = F_dict_create(8);
	for (size_t i = 0; i < 8; ++i)
		F_dict_set(d, i, i);

	if (F_dict_length(d) == 8)
	{
		F_dict_destroy(d);
		return 0;
	}

	F_dict_destroy(d);
	
	return 1;
}

static void run_test(struct test *t)
{
	t->executed = 1;
	clock_gettime(CLOCK_MONOTONIC, &t->start);
	t->retval = t->func(&t->errmsg);
	clock_gettime(CLOCK_MONOTONIC, &t->stop);
}

int main(int argc, char **argv)
{
	(void)argc, (void)argv;

	F_dict_t tests = F_dict_create(F_DICT_3);
	F_list_t ran_tests = F_list_create();

	REGISTER_TEST(tests, "dict: Insert 1024 elements in dict 4", test_dict_1);
	REGISTER_TEST(tests, "dict: Insert 1024 elements in dict 2", test_dict_2);
	REGISTER_TEST(tests, "dict: Insert 1024 elements in dict 4 and retrieve each", test_dict_3);
	REGISTER_TEST(tests, "dict: Insert 1024 elements in dict 2 and retrieve each", test_dict_4);
	REGISTER_TEST(tests, "dict: Insert 65536 elements in dict 6", test_dict_5);
	REGISTER_TEST(tests, "dict: Insert 65536 elements in dict 2", test_dict_6);
	REGISTER_TEST(tests, "dict: Insert 65536 elements in dict 6 and retrieve each", test_dict_7);
	REGISTER_TEST(tests, "dict: Insert 1024 elements to 1024 buckets", test_dict_8);
	REGISTER_TEST(tests, "dict: Insert 1024 elements to 16 buckets and retrieve each", test_dict_9);
	REGISTER_TEST(tests, "dict: Insert 1024 elements to 8 buckets and retrieve each", test_dict_10);
	REGISTER_TEST(tests, "dict: Insert 8 elements and check length", test_dict_11);

	for (int o = 1; o < argc; ++o)
	{
		if (!strcmp(argv[o], "any"))
		{
			F_dict_entry_t entries[F_dict_length(tests)];
			size_t entries_cnt = F_dict_entries(tests, entries, sizeof entries);

			for (size_t o = 0; o < entries_cnt; ++o)
			{
				struct test *t = (struct test *)entries[o]->data;
				if (!t->executed)
				{
					run_test(t);
					F_list_append(ran_tests, (uintptr_t)t);
				}
			}
			continue;
		}

		struct test **tp = (struct test **)F_dict_lookup_s(tests, argv[o]);
		if (tp && !(*tp)->executed)
		{
			struct test *t = *tp;
			run_test(t);
			F_list_append(ran_tests, (uintptr_t)t);
		}
		else if (!tp)
			fprintf(stderr, "No such test: %s\n", argv[o]);
	}

	int passed = 0;
	int failed = 0;
	int ran = 0;
	double time = 0.0;

	F_LIST_FOR_EACH(ran_tests, node)
	{
		struct test *t = F_list_value(struct test *, node);
		double rtime = t->stop.tv_sec - t->start.tv_sec;
		rtime += (t->stop.tv_nsec - t->start.tv_nsec) / 1000000000.;

		fprintf(stdout, "[%s] %s [%s] %Fs%s%s\n",
			t->funcname,
			t->description,
			(t->retval ? "fail" : "pass"),
			rtime,
			(t->errmsg ? "\n\t" : ""),
			(t->errmsg ? t->errmsg : ""));

		ran += 1;
		if (t->retval)
			failed += 1;
		else
			passed += 1;

		time += rtime;
	}

	if (ran)
		fprintf(stdout, "%d / %d / %d >> %d%% in %Fs\n",
			passed,
			failed,
			ran,
			passed * 100 / ran,
			time);
}


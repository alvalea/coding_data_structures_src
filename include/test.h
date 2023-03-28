#ifndef __TEST__
#define __TEST__

typedef int (*test_fn)();

void test(const char* test_name, test_fn fn);

void test_main();

#endif

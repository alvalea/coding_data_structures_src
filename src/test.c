#include <test.h>

#include <stdio.h>
#include <algo/container_test.h>

void test(const char* test_name, test_fn fn) {
  printf("[%s]\t", test_name);
  if(fn()) {
    printf("\033[1;31m FAIL");
  } else {
    printf("\033[1;32m SUCCESS");
  }
  printf("\033[0m \n");
}

void test_main() {
  test("test_Array_add", test_Array_add);
  test("test_Array_get", test_Array_get);
  test("test_Array_len", test_Array_len);
  test("test_Array_clear", test_Array_clear);

  test("test_List_add", test_List_add);
  test("test_List_remove", test_List_remove);
  test("test_List_head", test_List_head);
  test("test_List_tail", test_List_tail);
  test("test_List_find", test_List_find);
}

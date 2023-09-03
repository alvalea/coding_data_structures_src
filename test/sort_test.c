#include <algo/sort_test.h>

#include <stdio.h>
#include <algo/sort.h>

int test_quicksort() {
  int array[] = { 5, 4, 3, 2, 1, 0 };
  quicksort(array, 0, sizeof(array)/sizeof(int) - 1);
  for (int i=0; i<sizeof(array)/sizeof(int); ++i) {
    if (array[i] != i) {
      return -1;
    }
  }
  return 0;
}

#include <algo/sort.h>

static
void swap(int* i, int* j) {
  int tmp = *i;
  *i = *j;
  *j = tmp;
}

static
int partition(int array[], int start, int end) {
  int pivot = array[end];
  int i = start - 1;
  for (int j=start; j<end; ++j) {
    if (array[j] <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i+1], &array[end]);

  return i+1;
}

void quicksort(int array[], int start, int end) {
  if (start < end) {
    int pivot = partition(array, start, end);
    quicksort(array, start, pivot - 1);
    quicksort(array, pivot + 1, end);
  }
}

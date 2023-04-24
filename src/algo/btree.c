#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 3
#define MIN 1

#define STR 100
typedef char string[STR];

typedef struct BTreeNode {
  struct BTreeNode* link[MAX + 1];
  int item[MAX + 1];
  int count;
} BTreeNode;

static
BTreeNode* new_BTreeNode(BTreeNode* parent, BTreeNode* child, int item){
  BTreeNode* n = calloc(1, sizeof(BTreeNode));
  n->link[0] = parent;
  n->link[1] = child;
  n->item[1] = item;
  n->count = 1;
  return n;
}

static
void delete_BTreeNode(BTreeNode* n) {
  for (int i = 0; i <= n->count; ++i) {
    if (n->link[i] != NULL) {
      delete_BTreeNode(n->link[i]);
    }
  }
  free(n);
}

static
void BTreeNode_print_page(BTreeNode* n, int child, char* str, BTreePrintFn print) {
  strcat_s(str, STR, "     ");
  for (int j = 1; j <= child; ++j) {
    strcat_s(str, STR, "| ");
  }
  printf(str);

  printf(" `---.");
  for (int i = 0; i < n->count; ++i) {
    print((void*)&n->item[i + 1]);
  }
  printf("\n");
}

static
void BTreeNode_print(BTreeNode* n, int child, char* str, BTreePrintFn print) {
  if (n) {
    string s = "";
    strcpy_s(s, STR, str);
    BTreeNode_print_page(n, child, s, print);
    for (int i = n->count; i >= 0; --i) {
      BTreeNode_print(n->link[i], i, s, print);
    }
  }
}

static
int BTreeNode_find_index(BTreeNode* n, int item) {
  int index = 0;

  if (item >= n->item[1]) {
      for (index = n->count;
          (item < n->item[index] && index > 1); --index);
  }
  return index;
}

static
bool BTreeNode_full(BTreeNode* n) {
  return n->count == MAX;
}

static
void BTreeNode_insert_value(BTreeNode* n, BTreeNode* child, int index, int item) {
    int i = n->count;
    while (i > index) {
        n->item[i + 1] = n->item[i];
        n->link[i + 1] = n->link[i];
        i--;
    }
    n->item[i + 1] = item;
    n->link[i + 1] = child;
    n->count++;
}

static
void BTreeNode_copy_half(BTreeNode* n, BTreeNode* nn, int median) {
  int i = median + 1;
  while (i <= MAX) {
    nn->item[i - median] = n->item[i];
    nn->link[i - median] = n->link[i];
    i++;
  }
  n->count = median;
  nn->count = MAX - median;
}

static
int find_median(int index) {
  int median = MIN;
  if (index > MIN) {
    median = MIN + 1;
  }
  return median;
}

static
void BTreeNode_split(BTreeNode* n, BTreeNode* child, BTreeNode** new_node,
  int index, int* pval, int item) {

  int median = find_median(index);

  BTreeNode* nn = calloc(1, sizeof(BTreeNode));
  BTreeNode_copy_half(n, nn, median);

  if (index <= MIN) {
    BTreeNode_insert_value(n, child, index, item);
  }
  else {
    BTreeNode_insert_value(nn, child, index - median, item);
  }
  *pval = n->item[n->count];
  nn->link[0] = n->link[n->count];
  n->count--;

  *new_node = nn;
}

static
bool BTreeNode_insert(BTreeNode* n, BTreeNode** child, int item, int* pval) {
  if (n == NULL) {
    *pval = item;
    *child = NULL;
    return true;
  }

  int index = BTreeNode_find_index(n, item);
  if (item == n->item[index]) {
    printf("Duplicates not allowed\n");
    return false;
  }

  if (BTreeNode_insert(n->link[index], child, item, pval)) {
    if (BTreeNode_full(n)) {
      BTreeNode_split(n, *child, child, index, pval, *pval);
      return true;
    }
    else {
      BTreeNode_insert_value(n, *child, index, *pval);
    }
  }
  return false;
}

struct BTree {
  BTreeNode* root;
};

BTree* new_BTree() {
  BTree* t = calloc(1, sizeof(BTree));
  return t;
}

void delete_BTree(BTree* t) {
  delete_BTreeNode(t->root);
  free(t);
}

void BTree_insert(BTree* t, int value) {
  int val;
  BTreeNode* child;
  if (BTreeNode_insert(t->root, &child, value, &val)) {
    t->root = new_BTreeNode(t->root, child, val);
  }
}

void BTree_print(BTree* h, BTreePrintFn print) {
  string s = "";
  BTreeNode_print(h->root, 0, s, print);
}

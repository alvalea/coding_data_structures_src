#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define STR 100
typedef char string[STR];

typedef struct BTreeNode {
  bool leaf;
  int min;	 // Minimum degree (defines the range for number of items)
  int count;	 // Current number of items
  size_t key_size;
  size_t value_size;
  BTreeCompareFn compare;
  void** items; // An array of items
  struct BTreeNode** c; // An array of child pointers
} BTreeNode;

static
BTreeNode* new_BTreeNode(bool leaf, int min, size_t key_size, size_t value_size,
    BTreeCompareFn compare) {
  BTreeNode* n = calloc(1, sizeof(BTreeNode));
  n->leaf = leaf;
  n->min  = min;
  n->key_size = key_size;
  n->value_size = value_size;
  n->compare = compare;
  n->items = calloc(2 * min - 1, sizeof(void*));
  n->c    = calloc(2 * min, sizeof(BTreeNode*));
  return n;
}

static
void BTreeNode_free(BTreeNode* n) {
  free(n->items);
  free(n->c);
  free(n);
}

static
void delete_BTreeNode(BTreeNode* n) {
  if (!n->leaf) {
    for (int i=0; i<=n->count; ++i) {
      delete_BTreeNode(n->c[i]);
    }
  }
  for (int i=0; i<n->count; ++i) {
    if (n->items[i] != NULL) {
      free(n->items[i]);
    }
  }
  BTreeNode_free(n);
}

static
bool BTreeNode_empty(BTreeNode* n) {
  return n->count == 0;
}

static
bool BTreeNode_full(BTreeNode* n) {
  return n->count == 2 * n->min - 1;
}

static
int BTreeNode_compare(BTreeNode* n, void* key1, void* key2) {
  if (n->compare != NULL) {
    return n->compare(key1, key2);
  }
  return memcmp(key1, key2, n->key_size) == 0;
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
static
void BTreeNode_split_child(BTreeNode* n, int i, BTreeNode* y)
{
  // Create a new node which is going to store (min-1) items
  // of y
  BTreeNode* z = new_BTreeNode(y->leaf, y->min, y->key_size, y->value_size,
      y->compare);
  z->count = n->min - 1;

  // Copy the last (min-1) min of y to z
  memmove(&z->items[0], &y->items[n->min], sizeof(void*) * (n->min - 1));

  // Copy the last min children of y to z
  if (!y->leaf)
  {
    memmove(&z->c[0], &y->c[n->min], sizeof(BTreeNode*) * n->min);
  }

  // Reduce the number of items in y
  y->count = n->min - 1;

  // Since this node is going to have a new child,
  // create space of new child
  memmove(&n->c[i + 2], &n->c[i + 1], sizeof(BTreeNode*) * (n->count - i));

  // Link the new child to this node
  n->c[i + 1] = z;

  // A key of y will move to this node. Find location of
  // new key and move all greater keys one space ahead
  memmove(&n->items[i+1], &n->items[i], sizeof(void*) * (n->count - i));

  // Copy the middle key of y to this node
  n->items[i] = y->items[n->min - 1];

  // Increment count of keys in this node
  n->count++;
}

static
void BTreeNode_insert_value(BTreeNode* n, int index, void* key, void* value) {
  if (n->items[index] == NULL) {
    n->items[index] = malloc(n->key_size + n->value_size);
  }
  char* item = (char*)n->items[index];
  memcpy(item, key, n->key_size);
  memcpy(item + n->key_size, value, n->value_size);
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
static
void BTreeNode_insert_non_full(BTreeNode* n, void* key, void* value) {
  // Initialize index as index of rightmost element
  int i = n->count - 1;

  // If this is a leaf node
  if (n->leaf) {
    // The following loop does two things
    // a) Finds the location of new key to be inserted
    // b) Moves all greater keys to one place ahead
    while (i >= 0 && BTreeNode_compare(n, n->items[i], key) > 0) {
      n->items[i + 1] = n->items[i];
      i--;
    }

    // Insert the new key at found location
    BTreeNode_insert_value(n, i + 1, key, value);
    n->count++;
  }
  else // If this node is not leaf
  {
    // Find the child which is going to have the new key
    while (i >= 0 && BTreeNode_compare(n, n->items[i], key) > 0)
      i--;

    // See if the found child is full
    if (BTreeNode_full(n->c[i + 1])) {
      // If the child is full, then split it
      BTreeNode_split_child(n, i + 1, n->c[i + 1]);

      // After split, the middle key of c[i] goes up and
      // c[i] is splitted into two. See which of the two
      // is going to have the new key
      if (BTreeNode_compare(n, n->items[i + 1], key) < 0)
        i++;
    }
    BTreeNode_insert_non_full(n->c[i + 1], key, value);
  }
}

// A utility function that returns the index of the first item that is
// greater than or equal to value
static
int BTreeNode_find_index(BTreeNode* n, void* key) {
  int idx = 0;
  while (idx < n->count && BTreeNode_compare(n, n->items[idx], key) < 0)
    ++idx;
  return idx;
}

// A function to remove the idx-th key from this node - which is a leaf node
static
void BTreeNode_remove_from_leaf(BTreeNode* n, int idx, bool free_item) {
  if (free_item) {
    free(n->items[idx]);
  }

  // Move all the keys after the idx-th pos one place backward
  memmove(&n->items[idx], &n->items[idx+1], sizeof(void*) * (n->count - (idx + 1)));

  // Reduce the count of keys
  n->count--;
}

// A function to find predecessor of items[idx]
static
void* BTreeNode_find_pred(BTreeNode* n, int idx) {
  // Keep moving to the right most node until we reach a leaf
  BTreeNode* cur = n->c[idx];
  while (!cur->leaf)
    cur = cur->c[cur->count];

  // Return the last key of the leaf
  return cur->items[cur->count - 1];
}

// A function to find successor of items[idx]
static
void* BTreeNode_find_succ(BTreeNode* n, int idx) {
  // Keep moving the left most node starting from c[idx+1] until we reach a leaf
  BTreeNode* cur = n->c[idx + 1];
  while (!cur->leaf)
    cur = cur->c[0];

  // Return the first item of the leaf
  return cur->items[0];
}

// A function to merge c[idx] with c[idx+1]
// c[idx+1] is freed after merging
void BTreeNode_merge(BTreeNode* n, int idx) {

  BTreeNode* child = n->c[idx];
  BTreeNode* sibling = n->c[idx + 1];

  // Pulling an item from the current node and inserting it into (min-1)th
  // position of n->c[idx]
  child->items[n->min - 1] = n->items[idx];

  // Copying the items from c[idx+1] to c[idx] at the end
  memmove(&child->items[n->min], &sibling->items[0], sizeof(void*) * (sibling->count));

  // Copying the child pointers from c[idx+1] to c[idx]
  if (!child->leaf)
  {
    memmove(&child->c[n->min], &sibling->c[0], sizeof(BTreeNode*) * (sibling->count + 1));
  }

  // Moving all items after idx in the current node one step before -
  // to fill the gap created by moving items[idx] to c[idx]
  memmove(&n->items[idx], &n->items[idx+1], sizeof(void*) * (n->count - (idx + 1)));

  // Moving the child pointers after (idx+1) in the current node one
  // step before
  memmove(&n->c[idx + 1], &n->c[idx + 2], sizeof(BTreeNode*) * ((n->count + 1) - (idx + 2)));

  // Updating the key count of child and the current node
  child->count += sibling->count + 1;
  n->count--;

  // Freeing the memory occupied by sibling
  BTreeNode_free(sibling);
}

// Forward declaration
static
void BTreeNode_remove(BTreeNode* n, void* key, bool free_item);

static
void BTreeNode_replace(BTreeNode* n, int idx, void* item) {
  free(n->items[idx]);
  n->items[idx] = item;
}

// A function to remove the idx-th item from this node - which is a non-leaf node
static
void BTreeNode_remove_from_non_leaf(BTreeNode* n, int idx, bool free_item) {
  void* k = n->items[idx];

  // If the child that precedes k (c[idx]) has at least min items,
  // find the predecessor 'pred' of k in the subtree rooted at
  // c[idx]. Replace k by pred. Recursively delete pred
  // in c[idx]
  if (n->c[idx]->count >= n->min)
  {
    void* pred = BTreeNode_find_pred(n, idx);
    BTreeNode_replace(n, idx, pred);
    BTreeNode_remove(n->c[idx], pred, false);
  }

  // If the child c[idx] has less that min items, examine c[idx+1].
  // If c[idx+1] has at least min items, find the successor 'succ' of k in
  // the subtree rooted at c[idx+1]
  // Replace k by succ
  // Recursively delete succ in c[idx+1]
  else if (n->c[idx + 1]->count >= n->min)
  {
    void* succ = BTreeNode_find_succ(n, idx);
    BTreeNode_replace(n, idx, succ);
    BTreeNode_remove(n->c[idx + 1], succ, false);
  }

  // If both c[idx] and c[idx+1] has less that min items,merge k and all of c[idx+1]
  // into c[idx]
  // Now c[idx] contains 2*min-1 items
  // Free c[idx+1] and recursively delete k from c[idx]
  else
  {
    BTreeNode_merge(n, idx);
    BTreeNode_remove(n->c[idx], k, free_item);
  }
}

// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
static
void BTreeNode_borrow_from_prev(BTreeNode* n, int idx)
{
  BTreeNode* child = n->c[idx];
  BTreeNode* sibling = n->c[idx - 1];

  // The last key from c[idx-1] goes up to the parent and items[idx-1]
  // from parent is inserted as the first item in c[idx]. Thus, the loses
  // sibling one item and child gains one item

  // Moving all items in c[idx] one step ahead
  memmove(&child->items[1], &child->items[0], sizeof(void*) * child->count);

  // If c[idx] is not a leaf, move all its child pointers one step ahead
  if (!child->leaf)
  {
    memmove(&child->c[1], &child->c[0], sizeof(BTreeNode*) * (child->count + 1));
  }

  // Setting child's first item equal to items[idx-1] from the current node
  child->items[0] = n->items[idx - 1];

  // Moving sibling's last child as C[idx]'s first child
  if (!child->leaf)
    child->c[0] = sibling->c[sibling->count];

  // Moving the item from the sibling to the parent
  // This reduces the number of items in the sibling
  n->items[idx - 1] = sibling->items[sibling->count - 1];

  child->count++;
  sibling->count--;
}

// A function to borrow an item from the c[idx+1] and place
// it in c[idx]
static
void BTreeNode_borrow_from_next(BTreeNode* n, int idx)
{
  BTreeNode* child = n->c[idx];
  BTreeNode* sibling = n->c[idx + 1];

  // items[idx] is inserted as the last item in c[idx]
  child->items[(child->count)] = n->items[idx];

  // Sibling's first child is inserted as the last child
  // into c[idx]
  if (!(child->leaf))
    child->c[(child->count) + 1] = sibling->c[0];

  //The first item from sibling is inserted into items[idx]
  n->items[idx] = sibling->items[0];

  // Moving all items in sibling one step behind
  memmove(&sibling->items[0], &sibling->items[1], sizeof(void*) * (sibling->count - 1));

  // Moving the child pointers one step behind
  if (!sibling->leaf)
  {
    memmove(&sibling->c[0], &sibling->c[1], sizeof(BTreeNode*) * sibling->count);
  }

  // Increasing and decreasing the item count of c[idx] and c[idx+1]
  // respectively
  child->count++;
  sibling->count--;
}

// A function to fill child c[idx] which has less than min-1 items
static
void BTreeNode_fill(BTreeNode* n, int idx)
{
  // If the previous child(c[idx-1]) has more than min-1 items, borrow an item
  // from that child
  if (idx != 0 && n->c[idx - 1]->count >= n->min)
    BTreeNode_borrow_from_prev(n, idx);

  // If the next child(c[idx+1]) has more than min-1 items, borrow an item
  // from that child
  else if (idx != n->count && n->c[idx + 1]->count >= n->min)
    BTreeNode_borrow_from_next(n, idx);

  // Merge c[idx] with its sibling
  // If c[idx] is the last child, merge it with its previous sibling
  // Otherwise merge it with its next sibling
  else
  {
    if (idx != n->count)
      BTreeNode_merge(n, idx);
    else
      BTreeNode_merge(n, idx - 1);
  }
}

// A function to remove the item value from the sub-tree rooted with this node
static
void BTreeNode_remove(BTreeNode* n, void* key, bool free_item) {
  int idx = BTreeNode_find_index(n, key);

  // The key to be removed is present in this node
  if (idx < n->count && BTreeNode_compare(n, n->items[idx], key) == 0) {

    // If the node is a leaf node - removeFromLeaf is called
    // Otherwise, removeFromNonLeaf function is called
    if (n->leaf)
      BTreeNode_remove_from_leaf(n, idx, free_item);
    else
      BTreeNode_remove_from_non_leaf(n, idx, free_item);
  }
  else
  {
    // If this node is a leaf node, then the key is not present in tree
    if (n->leaf) {
      printf("Key not found\n");
      return;
    }

    // The key to be removed is present in the sub-tree rooted with this node
    // The flag indicates whether the key is present in the sub-tree rooted
    // with the last child of this node
    bool flag = (idx == n->count);

    // If the child where the key is supposed to exist has less that min keys,
    // we fill that child
    if (n->c[idx]->count < n->min)
      BTreeNode_fill(n, idx);

    // If the last child has been merged, it must have merged with the previous
    // child and so we recurse on the (idx-1)th child. Else, we recurse on the
    // (idx)th child which now has at least min keys
    if (flag && idx > n->count)
      BTreeNode_remove(n->c[idx - 1], key, free_item);
    else
      BTreeNode_remove(n->c[idx], key, free_item);
  }
}

static
void* BTreeNode_find(BTreeNode* n, void* key) {
  // Find the first key greater than or equal to value
  int i = 0;
  while (i < n->count && BTreeNode_compare(n, n->items[i], key) < 0)
    i++;

  // If the found item is equal to value, return this node
  if (BTreeNode_compare(n, n->items[i], key) == 0)
    return (char*)n->items[i] + n->key_size;

  // If item is not found here and this is a leaf node
  if (n->leaf) {
    printf("Key not found\n");
    return NULL;
  }

  // Go to the appropriate child
  return BTreeNode_find(n->c[i], key);
}

static
void BTreeNode_print_page(BTreeNode* n, int child, string str, BTreePrintFn print) {
  strcat(str, "     ");
  for (int j = 1; j <= child; ++j) {
    strcat(str, "| ");
  }
  printf("%s", str);

  printf(" `---.");
  for (int i = 0; i < n->count; ++i) {
    print(n->items[i]);
  }
  printf("\n");
}

static
void BTreeNode_print_tree(BTreeNode* n, int child, string str, BTreePrintFn print) {
  if (n != NULL) {
    string s = "";
    strcpy(s, str);
    BTreeNode_print_page(n, child, s, print);
    for (int i = n->count; i >= 0; --i) {
      if (!n->leaf) {
        BTreeNode_print_tree(n->c[i], i, s, print);
      }
    }
  }
}

struct BTree {
  int min; // Minimum degree
  size_t key_size;
  size_t value_size;
  BTreeCompareFn compare;
  BTreeNode* root; // Pointer to root node
};

BTree* new_BTree(int min_degree, size_t key_size, size_t value_size,
    BTreeCompareFn compare) {
  BTree* t = calloc(1, sizeof(BTree));
  t->min = min_degree;
  t->key_size = key_size;
  t->value_size = value_size;
  t->compare = compare;
  return t;
}

void delete_BTree(BTree* t) {
  if (t->root != NULL) {
    delete_BTreeNode(t->root);
  }
  free(t);
}

void BTree_insert(BTree* t, void* key, void* value) {
  // If tree is empty
  if (t->root == NULL) {
    // Allocate memory for root
    BTreeNode* n = new_BTreeNode(true, t->min, t->key_size, t->value_size,
        t->compare);
    BTreeNode_insert_value(n, 0, key, value);
    n->count = 1; // Update number of keys in root
    t->root = n;
  }
  else // If tree is not empty
  {
    // If root is full, then tree grows in height
    if (BTreeNode_full(t->root)) {
      // Allocate memory for new root
      BTreeNode* n = new_BTreeNode(false, t->min, t->key_size, t->value_size,
          t->compare);

      // Make old root as child of new root
      n->c[0] = t->root;

      // Split the old root and move 1 key to the new root
      BTreeNode_split_child(n, 0, t->root);

      // New root has two children now. Decide which of the
      // two children is going to have new key
      int i = 0;
      if (BTreeNode_compare(n, n->items[0], key) < 0)
        i++;
      BTreeNode_insert_non_full(n->c[i], key, value);

      // Change root
      t->root = n;
    }
    else // If root is not full, call insertNonFull for root
    {
      BTreeNode_insert_non_full(t->root, key, value);
    }
  }
}

void BTree_remove(BTree* t, void* key) {
  if (t->root == NULL) {
    printf("The tree is empty\n");
    return;
  }

  // Call the remove function for root
  BTreeNode_remove(t->root, key, true);

  // If the root node has 0 keys, make its first child as the new root
  // if it has a child, otherwise set root as NULL
  if (BTreeNode_empty(t->root)) {
    BTreeNode* tmp = t->root;
    if (t->root->leaf)
      t->root = NULL;
    else
      t->root = t->root->c[0];

    // Free the old root
    BTreeNode_free(tmp);
  }
}

void* BTree_find(BTree* t, void* key) {
  if (t->root == NULL) {
    printf("Key not found\n");
    return NULL;
  }
  return BTreeNode_find(t->root, key);
}

void BTree_print(BTree* t, BTreePrintFn print) {
  if (t->root != NULL) {
    string s = "";
    BTreeNode_print_tree(t->root, 0, s, print);
  }
}

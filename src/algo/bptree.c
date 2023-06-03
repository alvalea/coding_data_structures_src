#include <algo/container.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define STR 100
typedef char string[STR];

typedef struct record {
  int value;
} record;

typedef struct BpTreeNode {
  bool leaf;
  int min;	 // Minimum degree (defines the range for number of items)
  int count;	 // Current number of items
  int* keys;
  void** c;
  struct BpTreeNode* parent;
} BpTreeNode;

typedef struct BpTreeNodeEntry {
  BpTreeNode* n;
  record* r;
} BpTreeNodeEntry;

static
BpTreeNode* new_BpTreeNode(bool leaf, int min) {
  BpTreeNode* n = calloc(1, sizeof(BpTreeNode));
  n->leaf = leaf;
  n->min = min;
  n->keys = calloc(min - 1, sizeof(int));
  n->c    = calloc(min, sizeof(void*));
  return n;
}

static
void BpTreeNode_free(BpTreeNode* n) {
  free(n->keys);
  free(n->c);
  free(n);
}

static
void delete_BpTreeNode(BpTreeNode* n) {
  if (!n->leaf) {
    for (int i=0; i<=n->count; ++i) {
      delete_BpTreeNode(n->c[i]);
    }
  } else {
    for (int i = 0; i < n->count; ++i) {
      free(n->c[i]);
    }
  }
  BpTreeNode_free(n);
}

static
bool BpTreeNode_full(BpTreeNode* n) {
  return n->count == n->min - 1;
}

static
void BpTreeNode_leaf_insert(BpTreeNode* n, int key, record* r) {
  // TODO
  // Replace with algorithm from BTreeNode_insert_non_full
  int insertion_point = 0;
  while (insertion_point < n->count && n->keys[insertion_point] < key) {
    insertion_point++;
  }

  for (int i = n->count; i > insertion_point; i--) {
    n->keys[i] = n->keys[i - 1];
    n->c[i] = n->c[i - 1];
  }
  n->keys[insertion_point] = key;
  n->c[insertion_point] = r;
  n->count++;
}

static
void BpTreeNode_node_insert(BpTreeNode* n, int left_index, int key, BpTreeNode* right) {
  for (int i = n->count; i > left_index; i--) {
    n->c[i + 1] = n->c[i];
    n->keys[i] = n->keys[i - 1];
  }
  n->c[left_index + 1] = right;
  n->keys[left_index] = key;
  n->count++;
}

static
int BpTreeNode_left_index(BpTreeNode* n, BpTreeNode* left) {
  int left_index = 0;
  while (left_index <= n->count && n->c[left_index] != left) {
    left_index++;
  }
  return left_index;
}

static
void BpTreeNode_print_page(BpTreeNode* n, int child, string str) {
  strcat(str, "     ");
  for (int j = 1; j <= child; ++j) {
    strcat(str, "| ");
  }
  printf("%s", str);

  printf(" `---.");
  for (int i = 0; i < n->count; ++i) {
    printf("%d ", n->keys[i]);
    if (n->leaf) {
      record* r = (record*)n->c[i];
      printf("[%d] ", r->value);
    }
  }

  printf("\n");
}

static
void BpTreeNode_print_tree(BpTreeNode* n, int child, string str) {
  if (n != NULL) {
    string s = "";
    strcpy(s, str);
    BpTreeNode_print_page(n, child, s);
    for (int i = n->count; i >= 0; --i) {
      if (!n->leaf) {
        BpTreeNode_print_tree(n->c[i], i, s);
      }
    }
  }
}

static
void BpTreeNode_remove(BpTreeNode* n, int key, BpTreeNode* entry) {
  // Remove the key and shift other keys accordingly.
  int i = 0;
  while (n->keys[i] != key) {
    i++;
  }
  for (++i; i < n->count; i++) {
    n->keys[i - 1] = n->keys[i];
  }

  // Remove the pointer and shift other c accordingly.
  // First determine number of c.
  int num_c = n->leaf ? n->count : n->count + 1;
  i = 0;
  while (n->c[i] != entry) {
    i++;
  }
  for (++i; i < num_c; i++) {
    n->c[i - 1] = n->c[i];
  }


  // One key fewer.
  n->count--;

  // Set the other c to NULL for tidiness.
  // A leaf uses the last pointer to point to the next leaf.
  if (n->leaf) {
    for (i = n->count; i < n->min - 1; i++) {
      n->c[i] = NULL;
    }
  } else {
    for (i = n->count + 1; i < n->min; i++) {
      n->c[i] = NULL;
    }
  }
}

/* Utility function for deletion.  Retrieves
 * the index of a node's nearest neighbor (sibling)
 * to the left if one exists.  If not (the node
 * is the leftmost child), returns -1 to signify
 * this special case.
 */
static
int BpTreeNode_neighbor_index(BpTreeNode* n) {
  /* Return the index of the key to the left
   * of the pointer in the parent pointing
   * to n.  
   * If n is the leftmost child, this means
   * return -1.
   */
  for (int i = 0; i <= n->parent->count; i++) {
    if (n->parent->c[i] == n) {
      return i - 1;
    }
  }
  return -1;
}

//===========================================================================

struct BpTree {
  int min;
  BpTreeNode* root;
};

BpTree* new_BpTree(int min_degree) {
  if (min_degree < 3) {
    printf("\n[ERROR] Degree: %d < Minimum Degree: 3\n", min_degree);
    exit(1);
  }

  BpTree* t = calloc(1, sizeof(BpTree));
  t->min = min_degree;
  return t;
}

void delete_BpTree(BpTree* t) {
  if (t->root != NULL) {
    delete_BpTreeNode(t->root);
  }
  free(t);
}
static
BpTreeNode* BpTree_find_leaf(BpTree* t, int key) {
  int i = 0;
  BpTreeNode* n = t->root;
  while (!n->leaf) {
    i = 0;
    while (i < n->count && key >= n->keys[i]) {
      i++;
    }
    n = (BpTreeNode*)n->c[i];
  }
  return n;
}

static
BpTreeNodeEntry BpTree_find_entry(BpTree* t, int key) {
  if (t->root != NULL) {
    BpTreeNode* leaf = BpTree_find_leaf(t, key);

    /* If root != NULL, leaf must have a value, even
     * if it does not contain the desired key.
     * (The leaf holds the range of keys that would
     * include the desired key.) 
     */
    int i = 0;
    while (i < leaf->count && leaf->keys[i] != key) {
      ++i;
    }
    if (i != leaf->count) {
      return (BpTreeNodeEntry){
        .n = leaf,
          .r = (record*)leaf->c[i]
      };
    }
  }
  return (BpTreeNodeEntry){0};
}

static
void BpTree_create_root(BpTree* t, int key, record* r) {
  BpTreeNode* n = new_BpTreeNode(true, t->min);
  n->keys[0] = key;
  n->c[0] = r;
  n->count++;
  t->root = n;
}

static
int find_split(int min) {
  if (min % 2 == 0) {
    return min/2;
  } else {
    return min/2 + 1;
  }
}

static
void BpTree_insert_into_new_root(BpTree* t, BpTreeNode* left, int key, BpTreeNode* right) {
  BpTreeNode* root = new_BpTreeNode(false, t->min);
  root->keys[0] = key;
  root->c[0] = left;
  root->c[1] = right;
  root->count++;
  left->parent = root;
  right->parent = root;
  t->root = root;
}

static
void BpTree_parent_insert(BpTree* t, BpTreeNode* left, int key, BpTreeNode* right);

static
void BpTree_node_split(BpTree* t, BpTreeNode* old_node, int left_index, int key,
    BpTreeNode* right) {
  /* First create a temporary set of keys and c
   * to hold everything in order, including
   * the new key and pointer, inserted in their
   * correct places. 
   * Then create a new node and copy half of the 
   * keys and c to the old node and
   * the other half to the new.
   */
  int k_prime = 0;
  BpTreeNode* new_node;
  BpTreeNode** temp_c = calloc(t->min + 1, sizeof(BpTreeNode*));
  int* temp_keys = calloc(t->min, sizeof(int));
  {
    for (int i = 0, j = 0; i < old_node->count + 1; i++, j++) {
      if (j == left_index + 1) j++;
      temp_c[j] = old_node->c[i];
    }

    for (int i = 0, j = 0; i < old_node->count; i++, j++) {
      if (j == left_index) j++;
      temp_keys[j] = old_node->keys[i];
    }

    temp_c[left_index + 1] = right;
    temp_keys[left_index] = key;

    /* Create the new node and copy
     * half the keys and c to the
     * old and half to the new.
     */  
    int split = find_split(t->min);
    new_node = new_BpTreeNode(false, t->min);
    old_node->count = 0;
    int i = 0;
    for (i = 0; i < split - 1; i++) {
      old_node->c[i] = temp_c[i];
      old_node->keys[i] = temp_keys[i];
      old_node->count++;
    }
    old_node->c[i] = temp_c[i];
    k_prime = temp_keys[split - 1];
    int j = 0;
    for (++i; i < t->min; i++, j++) {
      new_node->c[j] = temp_c[i];
      new_node->keys[j] = temp_keys[i];
      new_node->count++;
    }
    new_node->c[j] = temp_c[i];

    new_node->parent = old_node->parent;
    for (int i = 0; i <= new_node->count; i++) {
      BpTreeNode* child = new_node->c[i];
      child->parent = new_node;
    }
  }
  free(temp_c);
  free(temp_keys);

  /* Insert a new key into the parent of the two
   * nodes resulting from the split, with
   * the old node to the left and the new to the right.
   */
  BpTree_parent_insert(t, old_node, k_prime, new_node);
}

static
void BpTree_parent_insert(BpTree* t, BpTreeNode* left, int key, BpTreeNode* right) {
  BpTreeNode* parent = left->parent;

  /* Case: new root. */
  if (parent == NULL) {
    BpTree_insert_into_new_root(t, left, key, right);
    return;
  }

  /* Case: leaf or node. (Remainder of
   * function body.)  
   */

  /* Find the parent's pointer to the left 
   * node.
   */

  int left_index = BpTreeNode_left_index(parent, left);


  // Simple case: the new key fits into the node. 
  if (!BpTreeNode_full(parent)) {
    BpTreeNode_node_insert(parent, left_index, key, right);
    return;
  }

  /*
   * Harder case:  split a node in order 
   * to preserve the B+ tree properties.
   */
  BpTree_node_split(t, parent, left_index, key, right);
}

static
void BpTree_leaf_split(BpTree* t, BpTreeNode* leaf, int key, record* r) {
  // TODO: use memmove

  BpTreeNode* new_leaf = new_BpTreeNode(true, t->min);

  int* temp_keys = calloc(t->min, sizeof(int));
  void** temp_c = calloc(t->min, sizeof(void *));
  {
    int insertion_index = 0;
    while (insertion_index < t->min - 1 && leaf->keys[insertion_index] < key) {
      insertion_index++;
    }

    for (int i = 0, j = 0; i < leaf->count; i++, j++) {
      if (j == insertion_index) j++;
      temp_keys[j] = leaf->keys[i];
      temp_c[j] = leaf->c[i];
    }

    temp_keys[insertion_index] = key;
    temp_c[insertion_index] = r;

    leaf->count = 0;

    int split = find_split(t->min - 1);

    for (int i = 0; i < split; i++) {
      leaf->c[i] = temp_c[i];
      leaf->keys[i] = temp_keys[i];
      leaf->count++;
    }

    for (int i = split, j = 0; i < t->min; i++, j++) {
      new_leaf->c[j] = temp_c[i];
      new_leaf->keys[j] = temp_keys[i];
      new_leaf->count++;
    }
  }
  free(temp_c);
  free(temp_keys);

  new_leaf->c[t->min - 1] = leaf->c[t->min - 1];
  leaf->c[t->min - 1] = new_leaf;

  for (int i = leaf->count; i < t->min - 1; i++) {
    leaf->c[i] = NULL;
  }
  for (int i = new_leaf->count; i < t->min - 1; i++) {
    new_leaf->c[i] = NULL;
  }

  new_leaf->parent = leaf->parent;
  int new_key = new_leaf->keys[0];

  BpTree_parent_insert(t, leaf, new_key, new_leaf);
}

void BpTree_insert(BpTree* t, int key, int value) {
  // The current implementation ignores duplicates
  BpTreeNodeEntry entry = BpTree_find_entry(t, key);
  if (entry.r != NULL) {
    // If the key already exists in this tree, update the value
    entry.r->value = value;
    return;
  }

  // Create a new record for the value
  record* r = calloc(1, sizeof(record));
  r->value = value;

  // Case: the tree does not exist yet. Start a new tree
  if (t->root == NULL) {
    BpTree_create_root(t, key, r);
    return;
  }

  // Case: the tree already exists. (Rest of function body.)
  BpTreeNode* leaf = BpTree_find_leaf(t, key);

  // Case: leaf has room for key and record_pointer
  if (!BpTreeNode_full(leaf)) {
    BpTreeNode_leaf_insert(leaf, key, r);
    return;
  }

  // Case:  leaf must be split
  BpTree_leaf_split(t, leaf, key, r);
}

static
void BpTree_adjust(BpTree* t) {
  /* Case: nonempty root.
   * Key and pointer have already been deleted,
   * so nothing to be done.
   */
  if (t->root->count > 0) {
    return;
  }

  /* Case: empty root. 
  */

  // If it has a child, promote 
  // the first (only) child
  // as the new root.
  BpTreeNode* new_root = NULL;
  if (!t->root->leaf) {
    new_root = t->root->c[0];
    new_root->parent = NULL;
  }

  BpTreeNode_free(t->root);
  t->root = new_root;
}

static
void BpTree_remove_entry(BpTree* t, BpTreeNode* n, int key, BpTreeNode* entry);

/* Merges a node that has become
 * too small after deletion
 * with a neighboring node that
 * can accept the additional entries
 * without exceeding the maximum.
 */
static
void BpTree_merge(BpTree* t, BpTreeNode* n, BpTreeNode* neighbor, int neighbor_index,
    int k_prime) {

  BpTreeNode* tmp;

  /* Swap neighbor with node if node is on the
   * extreme left and neighbor is to its right.
   */
  if (neighbor_index == -1) {
    tmp = n;
    n = neighbor;
    neighbor = tmp;
  }

  /* Starting point in the neighbor for copying
   * keys and pointers from n.
   * Recall that n and neighbor have swapped places
   * in the special case of n being a leftmost child.
   */
  int neighbor_insertion_index = neighbor->count;

  /* Case:  nonleaf node.
   * Append k_prime and the following pointer.
   * Append all pointers and keys from the neighbor.
   */
  if (!n->leaf) {

    /* Append k_prime.
    */
    neighbor->keys[neighbor_insertion_index] = k_prime;
    neighbor->count++;


    int n_end = n->count;
    int i = 0;
    int j = 0;
    for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
      neighbor->keys[i] = n->keys[j];
      neighbor->c[i] = n->c[j];
      neighbor->count++;
      n->count--;
    }

    /* The number of pointers is always
     * one more than the number of keys.
     */
    neighbor->c[i] = n->c[j];

    /* All children must now point up to the same parent.
    */
    for (i = 0; i < neighbor->count + 1; i++) {
      tmp = (BpTreeNode*)neighbor->c[i];
      tmp->parent = neighbor;
    }
  }

  /* In a leaf, append the keys and pointers of
   * n to the neighbor.
   * Set the neighbor's last pointer to point to
   * what had been n's right neighbor.
   */
  else {
    for (int i = neighbor_insertion_index, j = 0; j < n->count; i++, j++) {
      neighbor->keys[i] = n->keys[j];
      neighbor->c[i] = n->c[j];
      neighbor->count++;
    }
    neighbor->c[t->min - 1] = n->c[t->min - 1];
  }

  BpTree_remove_entry(t, n->parent, k_prime, n);
  BpTreeNode_free(n);
}

/* Redistributes entries between two nodes when
 * one has become too small after deletion
 * but its neighbor is too big to append the
 * small node's entries without exceeding the
 * maximum
 */
static
void BpTree_redistribute(BpTree* t, BpTreeNode* n, BpTreeNode* neighbor,
    int neighbor_index, int k_prime_index, int k_prime) {
  int i;
  BpTreeNode* tmp;

  /* Case: n has a neighbor to the left. 
   * Pull the neighbor's last key-pointer pair over
   * from the neighbor's right end to n's left end.
   */
  if (neighbor_index != -1) {
    if (!n->leaf)
      n->c[n->count + 1] = n->c[n->count];
    for (i = n->count; i > 0; i--) {
      n->keys[i] = n->keys[i - 1];
      n->c[i] = n->c[i - 1];
    }
    if (!n->leaf) {
      n->c[0] = neighbor->c[neighbor->count];
      tmp = (BpTreeNode*)n->c[0];
      tmp->parent = n;
      neighbor->c[neighbor->count] = NULL;
      n->keys[0] = k_prime;
      n->parent->keys[k_prime_index] = neighbor->keys[neighbor->count - 1];
    }
    else {
      n->c[0] = neighbor->c[neighbor->count - 1];
      neighbor->c[neighbor->count - 1] = NULL;
      n->keys[0] = neighbor->keys[neighbor->count - 1];
      n->parent->keys[k_prime_index] = n->keys[0];
    }
  }

  /* Case: n is the leftmost child.
   * Take a key-pointer pair from the neighbor to the right.
   * Move the neighbor's leftmost key-pointer pair
   * to n's rightmost position.
   */
  else {  
    if (n->leaf) {
      n->keys[n->count] = neighbor->keys[0];
      n->c[n->count] = neighbor->c[0];
      n->parent->keys[k_prime_index] = neighbor->keys[1];
    }
    else {
      n->keys[n->count] = k_prime;
      n->c[n->count + 1] = neighbor->c[0];
      tmp = (BpTreeNode*)n->c[n->count + 1];
      tmp->parent = n;
      n->parent->keys[k_prime_index] = neighbor->keys[0];
    }
    for (i = 0; i < neighbor->count - 1; i++) {
      neighbor->keys[i] = neighbor->keys[i + 1];
      neighbor->c[i] = neighbor->c[i + 1];
    }
    if (!n->leaf)
      neighbor->c[i] = neighbor->c[i + 1];
  }

  /* n now has one more key and one more pointer;
   * the neighbor has one fewer of each.
   */
  n->count++;
  neighbor->count--;
}

static
void BpTree_remove_entry(BpTree* t, BpTreeNode* n, int key, BpTreeNode* entry) {
  BpTreeNode_remove(n, key, entry);

  if (n == t->root) {
    BpTree_adjust(t);
    return;
  }

  /* Case:  deletion from a node below the root.
   * (Rest of function body.)
   */

  /* Determine minimum allowable size of node,
   * to be preserved after deletion.
   */
  int min_keys = n->leaf ? find_split(t->min - 1) : find_split(t->min) - 1;

  /* Case:  node stays at or above minimum.
   * (The simple case.)
   */
  if (n->count >= min_keys) {
    return;
  }

  /* Case:  node falls below minimum.
   * Either merge or redistribution
   * is needed.
   */

  /* Find the appropriate neighbor node with which
   * to merge.
   * Also find the key (k_prime) in the parent
   * between the pointer to node n and the pointer
   * to the neighbor.
   */
  int neighbor_index = BpTreeNode_neighbor_index(n);
  int k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
  int k_prime = n->parent->keys[k_prime_index];
  BpTreeNode* neighbor = neighbor_index == -1 ? n->parent->c[1] : 
    n->parent->c[neighbor_index];
  int capacity = n->leaf ? t->min : t->min - 1;

  if (neighbor->count + n->count < capacity) {
    BpTree_merge(t, n, neighbor, neighbor_index, k_prime);
  } else {
    BpTree_redistribute(t, n, neighbor, neighbor_index, k_prime_index, k_prime);
  }
}

void BpTree_remove(BpTree* t, int key) {
  BpTreeNodeEntry entry = BpTree_find_entry(t, key);

  if (entry.r != NULL && entry.n != NULL) {
    BpTree_remove_entry(t, entry.n, key, (BpTreeNode*)entry.r);
    free(entry.r);
  }
}

int BpTree_find(BpTree* t, int key) {
}

void BpTree_print(BpTree* t) {
  printf("\n");
  string s = "";
  BpTreeNode_print_tree(t->root, 0, s);
}

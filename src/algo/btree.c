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
  int* items; // An array of items
  struct BTreeNode** c; // An array of child pointers
} BTreeNode;

static
BTreeNode* new_BTreeNode(bool leaf, int min) {
  BTreeNode* n = calloc(1, sizeof(BTreeNode));
  n->leaf = leaf;
  n->min  = min;
  n->items = calloc(2 * min - 1, sizeof(int));
  n->c    = calloc(2 * min, sizeof(BTreeNode*));
  return n;
}

static
void delete_BTreeNode(BTreeNode* n) {
  if (!n->leaf) {
    for (int i=0; i<=n->count; ++i) {
      delete_BTreeNode(n->c[i]);
    }
  }
  free(n->items);
  free(n->c);
  free(n);
}

static
bool BTreeNode_full(BTreeNode* n) {
  return n->count == 2 * n->min - 1;
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
static
void BTreeNode_split_child(BTreeNode* n, int i, BTreeNode* y)
{
	// Create a new node which is going to store (min-1) items
	// of y
	BTreeNode* z = new_BTreeNode(y->leaf, y->min);
	z->count = n->min - 1;

	// Copy the last (min-1) min of y to z
	for (int j = 0; j < n->min - 1; j++)
		z->items[j] = y->items[j + n->min];

	// Copy the last min children of y to z
	if (y->leaf == false)
	{
		for (int j = 0; j < n->min; j++)
			z->c[j] = y->c[j + n->min];
	}

	// Reduce the number of items in y
	y->count = n->min - 1;

	// Since this node is going to have a new child,
	// create space of new child
	for (int j = n->count; j >= i + 1; j--)
		n->c[j + 1] = n->c[j];

	// Link the new child to this node
	n->c[i + 1] = z;

	// A key of y will move to this node. Find location of
	// new key and move all greater keys one space ahead
	for (int j = n->count - 1; j >= i; j--)
		n->items[j + 1] = n->items[j];

	// Copy the middle key of y to this node
	n->items[i] = y->items[n->min - 1];

	// Increment count of keys in this node
	n->count++;
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
static
void BTreeNode_insert_non_full(BTreeNode* n, int value) {
	// Initialize index as index of rightmost element
	int i = n->count - 1;

	// If this is a leaf node
	if (n->leaf) {
		// The following loop does two things
		// a) Finds the location of new key to be inserted
		// b) Moves all greater keys to one place ahead
		while (i >= 0 && n->items[i] > value) {
			n->items[i + 1] = n->items[i];
			i--;
		}

		// Insert the new key at found location
		n->items[i + 1] = value;
		n->count++;
	}
	else // If this node is not leaf
	{
		// Find the child which is going to have the new key
		while (i >= 0 && n->items[i] > value)
			i--;

		// See if the found child is full
		if (BTreeNode_full(n->c[i + 1])) {
			// If the child is full, then split it
			BTreeNode_split_child(n, i + 1, n->c[i + 1]);

			// After split, the middle key of c[i] goes up and
			// c[i] is splitted into two. See which of the two
			// is going to have the new key
			if (n->items[i + 1] < value)
				i++;
		}
    BTreeNode_insert_non_full(n->c[i + 1], value);
	}
}

static
void BTreeNode_print_page(BTreeNode* n, int child, string str, BTreePrintFn print) {
  strcat_s(str, STR, "     ");
  for (int j = 1; j <= child; ++j) {
    strcat_s(str, STR, "| ");
  }
  printf(str);

  printf(" `---.");
  for (int i = 0; i < n->count; ++i) {
    print((void*)&n->items[i]);
  }
  printf("\n");
}

static
void BTreeNode_print_tree(BTreeNode* n, int child, string str, BTreePrintFn print) {
  if (n) {
    string s = "";
    strcpy_s(s, STR, str);
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
  BTreeNode* root; // Pointer to root node
};

BTree* new_BTree(int min_degree) {
  BTree* t = calloc(1, sizeof(BTree));
  t->min = min_degree;
  return t;
}

void delete_BTree(BTree* t) {
  if (t->root != NULL) {
    delete_BTreeNode(t->root);
  }
  free(t);
}

void BTree_insert(BTree* t, int value) {
  // If tree is empty
	if (t->root == NULL) {
		// Allocate memory for root
		BTreeNode* n = new_BTreeNode(true, t->min);
		n->items[0] = value; // Insert key
		n->count = 1; // Update number of keys in root
    t->root = n;
	}
	else // If tree is not empty
	{
		// If root is full, then tree grows in height
		if (BTreeNode_full(t->root)) {
			// Allocate memory for new root
			BTreeNode* n = new_BTreeNode(false, t->min);

			// Make old root as child of new root
			n->c[0] = t->root;

			// Split the old root and move 1 key to the new root
			BTreeNode_split_child(n, 0, t->root);

			// New root has two children now. Decide which of the
			// two children is going to have new key
			int i = 0;
			if (n->items[0] < value)
				i++;
			BTreeNode_insert_non_full(n->c[i], value);

			// Change root
			t->root = n;
		}
		else // If root is not full, call insertNonFull for root
    {
			BTreeNode_insert_non_full(t->root, value);
    }
	}
}

void BTree_delete(BTree* t, int value) {
}

void BTree_print(BTree* t, BTreePrintFn print) {
  string s = "";
  if (t->root != NULL) BTreeNode_print_tree(t->root, 0, s, print);
}

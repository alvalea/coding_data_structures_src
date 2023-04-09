#include <algo/container.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum MapNodeColor {
  BLACK,
  RED
} MapNodeColor;

typedef struct MapNodeKV {
  void*  key_data;
  size_t key_size;
  void*  value_data;
  size_t value_size;
} MapNodeKV;

typedef struct MapNode {
  struct MapNode* parent;
  struct MapNode* left;
  struct MapNode* right;
  MapNodeColor color;
  void* key;
  void* value;
} MapNode;

typedef struct Map {
  size_t key_size;
  size_t value_size;
  MapNode* root;
  MapNode* NIL;
} Map;

static
MapNode* new_MapNodeNIL(size_t key_size, size_t value_size) {
  MapNode* n = calloc(1, sizeof(MapNode));
  n->color = BLACK;
  n->key = calloc(1, key_size);
  n->value = calloc(1, value_size);
  return n;
}

static
MapNode* new_MapNode(MapNodeColor color, MapNodeKV* kv) {
  MapNode* n = calloc(1, sizeof(MapNode));
  n->color = color;

  n->key = malloc(kv->key_size);
  memcpy(n->key, kv->key_data, kv->key_size);

  n->value = malloc(kv->value_size);
  memcpy(n->value, kv->value_data, kv->value_size);

  return n;
}

static
void delete_MapNode(MapNode* n, MapNode* nil) {
  if (n != NULL && n != nil) {
    delete_MapNode(n->left, nil);
    delete_MapNode(n->right, nil);
    free(n->key);
    free(n->value);
    free(n);
  }
}

static
MapNode* MapNode_find(MapNode* n, MapNode* nil, void* key, size_t key_size) {
  MapNode* x = n;
  while (x != nil && memcmp(key, x->key, key_size) != 0) {
    if (memcmp(key, x->key, key_size) < 0) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  return x;
}

static
void Map_leftRotate(Map* m, MapNode* x) {
  MapNode* y = x->right;
  x->right = y->left;
  if(y->left != m->NIL) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == m->NIL) { //x is root
    m->root = y;
  }
  else if(x == x->parent->left) { //x is left child
    x->parent->left = y;
  }
  else { //x is right child
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static
void Map_rightRotate(Map* m, MapNode* x) {
  MapNode* y = x->left;
  x->left = y->right;
  if(y->right != m->NIL) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == m->NIL) { //x is root
    m->root = y;
  }
  else if(x == x->parent->right) { //x is left child
    x->parent->right = y;
  }
  else { //x is right child
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void Map_insertFixup(Map* m, MapNode* z) {
  while(z->parent->color == RED) {
    if(z->parent == z->parent->parent->left) { //z.parent is the left child

      MapNode* y = z->parent->parent->right; //uncle of z

      if(y->color == RED) { //case 1
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else { //case2 or case3
        if(z == z->parent->right) { //case2
          z = z->parent; //marked z.parent as new z
          Map_leftRotate(m, z);
        }
        //case3
        z->parent->color = BLACK; //made parent black
        z->parent->parent->color = RED; //made parent red
        Map_rightRotate(m, z->parent->parent);
      }
    }
    else { //z.parent is the right child
      MapNode* y = z->parent->parent->left; //uncle of z

      if(y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->left) {
          z = z->parent; //marked z.parent as new z
          Map_rightRotate(m, z);
        }
        z->parent->color = BLACK; //made parent black
        z->parent->parent->color = RED; //made parent red
        Map_leftRotate(m, z->parent->parent);
      }
    }
  }
  m->root->color = BLACK;
}

static
void Map_transplant(Map* m, MapNode* u, MapNode* v) {
  if(u->parent == m->NIL) {
    m->root = v;
  } else if(u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

static
MapNode* Map_minimum(Map* m, MapNode* x) {
  while(x->left != m->NIL) {
    x = x->left;
  }
  return x;
}

static
void Map_deleteFixup(Map* m, MapNode* x) {
  while(x != m->root && x->color == BLACK) {
    if(x == x->parent->left) {
      MapNode* w = x->parent->right;
      if(w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        Map_leftRotate(m, x->parent);
        w = x->parent->right;
      }
      if(w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      }
      else {
        if(w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          Map_rightRotate(m, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        Map_leftRotate(m, x->parent);
        x = m->root;
      }
    }
    else {
      MapNode* w = x->parent->left;
      if(w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        Map_rightRotate(m, x->parent);
        w = x->parent->left;
      }
      if(w->right->color == BLACK && w->left->color == BLACK) {
        w->color = RED;
        x = x->parent;
      }
      else {
        if(w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          Map_leftRotate(m, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        Map_rightRotate(m, x->parent);
        x = m->root;
      }
    }
  }
  x->color = BLACK;
}

Map* new_Map(size_t key_size, size_t value_size) {
  Map* m = calloc(1, sizeof(Map));
  m->key_size = key_size;
  m->value_size = value_size;
  m->NIL = new_MapNodeNIL(key_size, value_size);
  m->root = m->NIL;
  return m;
}

void delete_Map(Map* m) {
  delete_MapNode(m->root, m->NIL);
  free(m->NIL->key);
  free(m->NIL->value);
  free(m->NIL);
  free(m);
}

void Map_insert(Map* m, void* key, void* value) {
  MapNode* z = new_MapNode(RED, &(MapNodeKV){
      .key_data = key, .key_size = m->key_size,
      .value_data = value, .value_size = m->value_size
      });
  MapNode* y = m->NIL; //variable for the parent of the added node
  MapNode* x = m->root;

  while(x != m->NIL) {
    y = x;
    if(memcmp(z->key, x->key, m->key_size) < 0) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;

  if(y == m->NIL) { //newly added node is root
    m->root = z;
  }
  else if(memcmp(z->key, y->key, m->key_size) < 0) { //value of child is less than its parent, left child
    y->left = z;
  } else {
    y->right = z;
  }

  z->right = m->NIL;
  z->left = m->NIL;

  Map_insertFixup(m, z);
}

void Map_delete(Map* m, void* key) {
  MapNode* z = MapNode_find(m->root, m->NIL, key, m->key_size);
  if (z == NULL) {
    return;
  }

  MapNode* y = z;
  MapNode* x;
  MapNodeColor y_orignal_color = y->color;
  if(z->left == m->NIL) {
    x = z->right;
    Map_transplant(m, z, z->right);
  }
  else if(z->right == m->NIL) {
    x = z->left;
    Map_transplant(m, z, z->left);
  }
  else {
    y = Map_minimum(m, z->right);
    y_orignal_color = y->color;
    x = y->right;
    if(y->parent == z) {
      x->parent = y;
    }
    else {
      Map_transplant(m, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    Map_transplant(m, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if(y_orignal_color == BLACK) {
    Map_deleteFixup(m, x);
  }

  z->left = NULL;
  z->right = NULL;
  delete_MapNode(z, m->NIL);
}

void* Map_find(Map* m, void* key) {
  MapNode* n = MapNode_find(m->root, m->NIL, key, m->key_size);
  if (n != m->NIL) {
    return n->value;
  }
  return NULL;
}

static
const char IDENT[] = "    ";

static
const char MIDDLE[] = "---";

static
const char RIGHT[] = ".---";

static
const char LEFT[] = "`---";

static
const char LATERAL[] = "   |";

typedef struct Trunk {
  struct Trunk* prev;
  const char* str;
} Trunk;

static
void Map_show_trunks(Trunk* t) {
  if (t == NULL) {
    return;
  }

  char str[5] = {0};
  strncpy_s(str, 5, t->str, strlen(t->str));

  Map_show_trunks(t->prev);

  printf("%s", str);
}

static
void Map_print_value(MapNode* n, MapPrintFn print) {
  if (n->color == RED) {
    printf("\033[0;31m");
  }
  print(n->value);
  printf("\033[0m");
}

static
void Map_print_tree(MapNode* n, Trunk* prev, bool isLeft, MapPrintFn print) {
  if (n == NULL) {
    return;
  }

  const char* prev_str = IDENT;
  Trunk trunk = { prev, prev_str };

  Map_print_tree(n->right, &trunk, true, print);

  if (!prev) {
    trunk.str = MIDDLE;
  } else if (isLeft) {
    trunk.str = RIGHT;
    prev_str = LATERAL;
  } else {
    trunk.str = LEFT;
    prev->str = prev_str;
  }

  Map_show_trunks(&trunk);
  Map_print_value(n, print);

  if (prev) {
    prev->str = prev_str;
  }
  trunk.str = LATERAL;

  Map_print_tree(n->left, &trunk, false, print);
}

void Map_print(Map* m, MapPrintFn print) {
  printf("\n");
  Map_print_tree(m->root, NULL, false, print);
}


#include <stdio.h>
#include <stdlib.h>

/**
 * A node of the binary tree containing the node's integer value
 * and pointers to its right and left children (or null).
 */
struct Node {
  int value;
  struct Node *parent;
  struct Node *left;
  struct Node *right;
};

/**
 * Create a new node with no children.
 */
struct Node *create(int value) {
  struct Node *node = malloc(sizeof(*node));
  node->value = value;
  node->parent = 0;
  node->left = 0;
  node->right = 0;
  return node;
}

/**
 * Insert the node n into the binary tree rooted by toNode.
 */
void insert(struct Node *toNode, struct Node *n) {
  struct Node *t = toNode;
  if (n->value <= t->value) {
    if (t->left == NULL) {
      t->left = n;
      n->parent = t;
    } else {
      insert(t->left, n);
    }
  } else {
    if (t->right == NULL) {
      t->right = n;
      n->parent = t;
    } else {
      insert(t->right, n);
    }
  }
}

/**
 * Print the contents entire binary tree in order of ascending integer value.
 */
void printInOrder(struct Node *node) {
  if (node->left != NULL) printInOrder(node->left);
  printf("%d\n", node->value);
  if (node->right != NULL) printInOrder(node->right);
}

/**
 * Print path of values from root to specified node in orderer starting from
 * root. Each node in path indicates direction taken (i.e., left or right) from
 * parent to arive at node.
 */
void printPath(struct Node *node) {
  if (node->parent != NULL) printPath(node->parent);
  if (node->parent == NULL)
    printf("%s: %d\n", "from root", node->value);
  else if (node->parent->left == node)
    printf("%s: %d\n", "left to", node->value);
  else
    printf("%s: %d\n", "right to", node->value);
}

/**
 * Create a new tree populated with values provided on the command line and
 * print it in depth-first order.
 */
int main(int argc, char *argv[]) {
  struct Node *root = 0;
  // read values from command line and add them to the tree
  struct Node *lastNodeInserted = NULL;
  for (int i = 1; i < argc; i++) {
    int value = atoi(argv[i]);
    struct Node *node = create(value);
    if (i == 1)
      root = node;
    else
      insert(root, node);
    lastNodeInserted = node;
  }
  // print results
  if (root) {
    printf("In Order:\n");
    printInOrder(root);
    printf("Path to %d:\n", lastNodeInserted->value);
    printPath(lastNodeInserted);
  }
}
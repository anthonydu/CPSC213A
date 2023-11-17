/* This header file provides the interface for a simple binary search tree library. */
#pragma once

#include "element.h"

struct tree;
struct tree_node;

/**
 * Create a new, empty tree
 */
struct tree *tree_new();

/**
 * Delete tree and all of its nodes
 */
void tree_delete(struct tree *t);

/**
 * Insert a new element into the tree.
 * This tree takes ownership of the element pointer and will free it during deletion.
 */
struct tree_node *tree_insert_node(struct tree *t, struct element *e);

/**
 * Perform depth-first traversal of tree, printing the values
 * in the linked list rooted at each node.
 */
void tree_ordered_print(struct tree *t);

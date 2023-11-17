/* This header file provides the interface for a simple doubly-linked-list library. */
#pragma once

#include "element.h"

struct list;
struct list_node;

/**
 * Create new list
 */
struct list *list_new();

/**
 * Delete the list and all of the elements it contains.
 */
void list_delete(struct list *l);

/**
 * Add an element to the list.
 * This list takes ownership of the element pointer and will free it during deletion.
 * Returns pointer to new list node.
 */
struct list_node *list_add_element(struct list *l, struct element *e);

/**
 * Remove element from list, deleting it.
 */
void list_delete_element(struct list *l, struct list_node *n);

/**
 * Get the first node in the list.
 */
struct list_node *list_get_head(struct list *l);

/**
 * Get the next node of a list node.
 */
struct list_node *list_node_get_next(struct list_node *n);

/**
 * Get list node element. Caller should keep_ref.
 */
struct element *list_node_get_element(struct list_node *n);

/*
 * pairingheap.h
 *
 * A Pairing Heap implementation
 *
 * Portions Copyright (c) 2012-2014, PostgreSQL Global Development Group
 *
 * src/include/lib/pairingheap.h
 */

#ifndef PAIRINGHEAP_H
#define PAIRINGHEAP_H

/*
 * This represents an element stored in the heap. Embed this in a larger
 * struct containing the actual data you're storing.
 *
 * A node can have multiple children, which form a double-linked list.
 * first_child points to the node's first child, and the subsequent children
 * can be found by following the next_sibling pointers. The last child has
 * next_sibling == NULL. The prev_or_parent pointer points to the node's
 * previous sibling, or if the node is its parent's first child, to the
 * parent.
 */
typedef struct pairingheap_node
{
	struct pairingheap_node *first_child;
	struct pairingheap_node *next_sibling;
	struct pairingheap_node *prev_or_parent;
} pairingheap_node;

/*
 * For a max-heap, the comparator must return <0 iff a < b, 0 iff a == b,
 * and >0 iff a > b.  For a min-heap, the conditions are reversed.
 */
typedef int (*pairingheap_comparator) (const pairingheap_node *a,
									   const pairingheap_node *b,
									   void *arg);

/*
 * A pairing heap.
 *
 * You can use pairingheap_allocate() to create a new palloc'd heap, or embed
 * this in a larger struct, set ph_compare and ph_arg directly and initialize
 * ph_root to NULL.
 */
typedef struct pairingheap
{
	pairingheap_comparator ph_compare;	/* comparison function */
	void	   *ph_arg;					/* opaque argument to ph_compare */
	pairingheap_node *ph_root;			/* current root of the heap */
} pairingheap;

extern pairingheap *pairingheap_allocate(pairingheap_comparator compare,
					void *arg);
extern void pairingheap_free(pairingheap *heap);
extern void pairingheap_add(pairingheap *heap, pairingheap_node *node);
extern pairingheap_node *pairingheap_first(pairingheap *heap);
extern pairingheap_node *pairingheap_remove_first(pairingheap *heap);
extern void pairingheap_remove(pairingheap *heap, pairingheap_node *node);

/* Resets the heap to be empty. */
#define pairingheap_reset(h)			((h)->ph_root = NULL)

/* Is the heap empty? */
#define pairingheap_is_empty(h)			((h)->ph_root == NULL)

/* Is there exactly one node in the heap? */
#define pairingheap_is_singular(h) \
	((h)->ph_root && (h)->ph_root->first_child == NULL)

#endif   /* PAIRINGHEAP_H */

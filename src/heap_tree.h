#ifndef HEAP_H 
#define HEAP_H 
	#include "heap.h"
#endif

struct tree_node 
{
	struct basic_node *node;
	struct tree_node  *left;
	struct tree_node  *right;
};

/* Struttura che rappresenta un heap implementato come albero. */
struct tree_rappresentation
{
	struct tree_node *root; 	/* Radice dell'heap. */
	struct tree_node *last; 	/* Ultimo nodo dell'heap. */
	int    n; 					/* Numero di elementi della struttura dati. (Tale valore può essere 
								   diverso diverso da heapsize). */
};


heap_t get_new_heap_tree(enum heap_type type);

void destroy_heap_tree_data(heap_t heap);
void destroy_tree(struct tree_node *root, void (*Destroy_obj)(void *object));
void destroy_heap_tree(heap_t heap);

data_t get_new_heap_tree_data();
data_t tree_insert(data_t heap_data, int i, void *node);
data_t tree_delete(data_t heap_data, int i);

struct tree_node *get_i_node(struct tree_node *root, int i);

struct basic_node *get_tree_basic_node(void *node);

int in_heap_tree(heap_t heap, void *elem);
int get_tree_length(data_t heap_data);

void *parent_tree(data_t heap_data, void *elem);
void *right_tree(data_t heap_data, void *elem);
void *left_tree(data_t heap_data, void *elem);
void *get_tree_node(data_t heap_data, int i);
void *create_tree_node(void *object, float priority);

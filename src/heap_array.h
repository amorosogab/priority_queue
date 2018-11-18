#ifndef HEAP_H 
#define HEAP_H 
	#include "heap.h"
#endif
/* Dimensione di un blocco. Più in dettaglio, questa macro esprime il numero 
   di locazioni di un array che compongono un blocco.  								*/
#define BLOCK 20

/* Struttura che rappresenta un heap implementato come array. L'array è 
   di dimensione A[0..(n_blocks * BLOCK)-1]. Gli elementi validi dell'array, 
   sono gli elementi A[0..last]. Gli elementi validi dell'HEAP, sono gli 
   elementi A[0..heapsize-1] (heapsize è dichiarato in struct heap) 
   heapsize-1 <= last.   														*/
struct array_rappresentation
{
	struct basic_node **A; 			/* Array, struttura fisica dell'heap. */
	int 			  last; 		/* Indice dell'ultimo elemento dell'array. */
	int				  n_blocks; 	/* Numero di blocchi dell'array correnti. */
};

heap_t get_new_heap_array(enum heap_type type);

data_t get_new_heap_array_data();
data_t array_insert(data_t heap_data, int i, void *node);
data_t array_delete(data_t heap_data, int i);

void destroy_heap_array_data(heap_t heap);
void destroy_heap_array(heap_t heap);

void *create_array_node(void *object, float priority);
void *get_array_node(data_t heap_data, int i);
void *parent_array(data_t heap_data, void *i);
void *left_array(data_t heap_data, void *i);
void *right_array(data_t heap_data, void *i);

struct basic_node *get_array_basic_node(void *i);

int get_array_length(data_t heap_data);
int in_heap_array(heap_t heap, void *elem);

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "heap_tree.h"
#include "comparator.h"

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di inizializzare e restituire un heap che utilizza 
	un albero binario come rappresentazione interna.

	PARAMETI DI INPUT
	type: enum heap_type - Tipo dell'heap.
			
	VALORE DI RITORNO
	Restituisce un albero binario come rappresentazione interna. 							*/
heap_t get_new_heap_tree(enum heap_type type)
{
	heap_t heap = get_new_generic_heap();
	heap->data = get_new_heap_tree_data();
	if(heap->data.heap_tree == NULL)
	{
		destroy_generic_heap(heap);
		return NULL;
	}

	heap->type = type;
	/* riempio la struttura operations */
	heap->op->Create_node = create_tree_node;
	heap->op->Insert = tree_insert;
	heap->op->Delete = tree_delete;
	heap->op->Get_basic_node = get_tree_basic_node;
	heap->op->Get_node = get_tree_node;
	heap->op->Destroy_object = free;
	heap->op->Get_length = get_tree_length;
	heap->op->In_heap = in_heap_tree;
	heap->op->Parent = parent_tree;
	heap->op->Left = left_tree;
	heap->op->Right = right_tree;
	if(type == MIN_HEAP)
		heap->op->Compare = compare_min_heap, heap->op->SENTINEL = FLT_MAX; 
	else
		heap->op->Compare = compare_max_heap, heap->op->SENTINEL = -FLT_MAX;
  
	return heap;
}

/* 	DESCRIZIONE
	Alloca è inizializza la struttura fisica dell'heap, che in questo caso è un albero binario.
			
	VALORE DI RITORNO
	Struttura fisica inizializzata di un heap_tree. 												*/
data_t get_new_heap_tree_data()
{
	data_t data;

	data.heap_tree = (struct tree_rappresentation *) malloc(sizeof(struct tree_rappresentation));
	if(data.heap_tree != NULL)
	{
		data.heap_tree->root = NULL;
		data.heap_tree->last = NULL;
		data.heap_tree->n = 0;
	}
	
	return data;
}

/* 	DESCRIZIONE
	Distrugge l'intera struttura dati dell'heap.
			
	PARAMETRI DI INPUT
	heap_heap : heap_t - Heap. 								*/
void destroy_heap_tree_data(heap_t heap)
{
	if(heap != NULL)
	{
		destroy_tree(heap->data.heap_tree->root, heap->op->Destroy_object);
		heap->data.heap_tree->root = NULL;
		heap->data.heap_tree->last = NULL;
		heap->data.heap_tree->n = 0;
		free(heap->data.heap_tree);
	}
}

/* 	DESCRIZIONE
	Distrugge un heap di tipo heap_tree.
			
	PARAMETRI DI INPUT
	heap : heap_t - heap. 					*/
void destroy_heap_tree(heap_t heap)
{
	destroy_heap_tree_data(heap);
	destroy_generic_heap(heap);
}

/* 	DESCRIZIONE
	Distrugge l'albero (o sottoalbero) radicato in root.
			
	PARAMETRI DI INPUT
	root : struct tree_node * 	  	 - Radice dell'albero.
	Destroy_obj(obj : void *) : void - Puntatore a funzione che dealloca correttamente 
	 								   l'oggetto del nodo. 											*/
void destroy_tree(struct tree_node *root, void (*Destroy_obj)(void *object))
{
	if(root != NULL)
	{
		destroy_tree(root->left, Destroy_obj);
		destroy_tree(root->right, Destroy_obj);
		destroy_basic_node(root->node, Destroy_obj);
		free(root);
	}
}

/* 	DESCRIZIONE
	La seguente funzione restituisce in output l'i-esimo nodo dell'heap_tree.
			
	PARAMETRI DI INPUT
	root : struct tree_node * - Radice dell'albero.
	i : int 		  		  - Posizione del nodo.
												
	VALORE DI RITORNO
	i-esimo nodo dell'heap_tree. 															*/
struct tree_node *get_i_node(struct tree_node *root, int i)
{
	if(i < 0)
		return NULL;
	else
	{
		int mask;
	
		for(mask = pow(2, (int) (log2(i + 1) - 1)); mask > 0; mask >>= 1) 
			if(mask & (i+1))
				root = root->right;
			else
				root = root->left;
																			
		return root;
	}
}

/* 	DESCRIZIONE
	La seguente funzione crea un nodo della struttura dati heap. Nel caso di una rappresentazione
	ad albero, il nodo di un heap è rappresentato dalla struttura tree_node.

	PARAMETRI DI INPUT
	object : void * - oggetto.
	priority : float  - priorità
												
	VALORE DI RITORNO
	Restituisce un tree_node contenente i valori passati in input. 							*/
void *create_tree_node(void *object, float priority)
{
	struct tree_node *t_node = NULL;
	struct basic_node *bn = create_basic_node(object, priority);
	if(bn != NULL)
	{
		t_node = (struct tree_node *) malloc(sizeof(struct tree_node));
		t_node->node = bn;
		t_node->left = NULL;
		t_node->right = NULL;
	}

	return t_node;
}

/* 	DESCRIZIONE
	Inserisce una coppia <object, priority> nella posizione i dell'heap. 
	Usando questa funzione, un nodo può essere inserito in una qualsiasi
	posizione i: 0 <= i < n. Se un nodo viene inserito in una Posizione
	i < n, è il chiamante che deve farsi carico di dellocare correttamente
	il nodo base contenuto in quella posizione.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura fisica dell'heap.
	i : int 		   - Posizione di inserimento.
	node : void * 	   - Nodo da inserire nella posizione i.
																								
	VALORE DI RITORNO
	Nuovo heap contenente il nodo <object, priority> nell'i-esima posizione. 	*/
data_t tree_insert(data_t heap_data, int i, void *node)
{
	if(heap_data.heap_tree != NULL && i >= 0)
	{
		if(i == heap_data.heap_tree->n)
		{
			if(i > 0)
			{
				int i_parent = parent_index(i);
				struct tree_node *parent_node = get_tree_node(heap_data, i_parent);

				if(parent_node->left == NULL)
					parent_node->left = node;
				else
					parent_node->right = node;
			}
			else if(i == 0)
				heap_data.heap_tree->root = node;

			heap_data.heap_tree->last = node;
			heap_data.heap_tree->n += 1;
		}
		else if(i < heap_data.heap_tree->n)
		{
			struct tree_node *i_node = get_tree_node(heap_data, i);
			i_node->node = node;
		}
	}

	return heap_data;
}

/* 	DESCRIZIONE
	Cancella il nodo che occupa la posizione i nell'heap. Si noti che è compito del chiamante
	deallocare correttamente il nodo base contenuto alla Posizione i.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Heap.
	i : int     	   - Posizione dell'elemento da eliminare.
																
	VALORE DI RITORNO
	heap_data originario, senza il nodo che occupava la posizione i. 

	WARNING
	Usare direttamente questa funzione con un indice i < heapsize-1, potrebbe creare qualche incoerenza nella struttura. */	
data_t tree_delete(data_t heap_data, int i)
{
	if(heap_data.heap_tree != NULL)
	{
		if(i == 0)
		{
			/* E' compito del chiamante deallocare correttamente il basic_node
			 * contenuto in root. 	*/
			free(heap_data.heap_tree->root);
			heap_data.heap_tree->root = NULL;
			heap_data.heap_tree->last = NULL;
		}
		else
		{
			int i_parent = parent_index(i);
			struct tree_node *i_node = NULL;
			struct tree_node *parent_node = get_tree_node(heap_data, i_parent);
			
			if(left_index(i_parent) == i)
			{
				i_node = parent_node->left;
				parent_node->left = NULL;
				heap_data.heap_tree->last = get_tree_node(heap_data, heap_data.heap_tree->n - 2);
			}
			else
			{
				i_node = parent_node->right;
				parent_node->right = NULL;
				heap_data.heap_tree->last = parent_node->left;
			}

			free(i_node);		
		}

		heap_data.heap_tree->n -= 1;
	}

	return heap_data;
}

/* 	DESCRIZIONE
	Estrae il basic_node da node, e lo restituisce in output.
				
	PARAMETRI DI INPUT
	node : void * 	   - Nodo
												
	VALORE DI RITORNO
	Nodo base. 														*/
struct basic_node *get_tree_basic_node(void *node)
{
	if(node != NULL)
		return ((struct tree_node *) node)->node;
	else
		return NULL;
}

/* 	DESCRIZIONE
	Recupera il nodo i-esimo dalla struttura dati, e lo restituisce in output.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura dati heap.
	i : int 	       - indice.
																
	VALORE DI RITORNO
	Restituisce il nodo i. 														*/
void *get_tree_node(data_t heap_data, int i)
{
	if(heap_data.heap_tree != NULL)
	{
		if(i <= 0)
			return heap_data.heap_tree->root;
		else if(i == heap_data.heap_tree->n - 1)
			return heap_data.heap_tree->last;
		else
			return get_i_node(heap_data.heap_tree->root, i);
	}
	else
		return NULL;
}

/* 	DESCRIZIONE
	Calcola il numero di elementi presenti nella struttura. Tale valore può essere diverso da heapsize.
				
	PARAMETRI DI INPUT
	heap_data: data_t - Struttura heap
												
	VALORE DI RITORNO
	Restituisce il numero di elementi presenti nella struttura. 										*/
int get_tree_length(data_t heap_data)
{
	if(heap_data.heap_tree != NULL)
		return heap_data.heap_tree->n;
	else
		return -1;
}

/* 	DESCRIZIONE
	La seguente funzione ci dice se l'elemento elem, appartiene all'heap (poiché un elemento può essere 
	nella struttura, non facendo parte dell'heap). Questa funzione viene utilizzata solo in heapfy. 
	Nel caso della rappresentazione ad albero, viene effettuato un controllo per verificare se elem
	è NULL oppure no. (vedi heapfy)

	PARAMETRI DI INPUT
	heap : heap_t - heap.
	elem : void * - Elemento da testare.
	
	VALORE DI RITORNO
	Restituisce 1 se elem è diverso da NULL, 0 altrimenti. 													*/
int in_heap_tree(heap_t heap, void *elem)
{
	if(elem != NULL)
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Dato l'elemento in input, restituisci la posizione del figlio sinistro.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura heap.
	i : void * 	       - Posizione i.
																
	VALORE DI RITORNO
	Restituisce la posizione del figlio sinistro. 								*/
void *left_tree(data_t heap_data, void *elem)
{
	if(elem != NULL)
		return ((struct tree_node *) elem)->left;
	else
		return NULL;
}

/* 	DESCRIZIONE
	Dato l'elemento in input, restituisci la posizione del figlio destro.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura heap.
	i : void * 	       - Posizione i.
																
	VALORE DI RITORNO
	Restituisce la posizione del figlio destro. 							*/
void *right_tree(data_t heap_data, void *elem)
{
	if(elem != NULL)
		return ((struct tree_node *) elem)->right;
	else
		return NULL;
}

/* 	DESCRIZIONE
	Dato l'elemento in input, restituisci la posizione del padre
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura heap.
	i : void * 	   	   - Posizione i.
																
	VALORE DI RITORNO
	Restituisce la posizione del padre. 								*/
void *parent_tree(data_t heap_data, void *elem)
{
	int i = *((int *) elem);

	if(i < heap_data.heap_tree->n)
		return get_tree_node(heap_data, parent_index(i));
	else
		return NULL;
}

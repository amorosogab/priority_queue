#include <stdlib.h>

#include "heap.h"

/* 	DESCRIZIONE
	Alloca e inizializza un heap generico.
			
	VALORE DI RITORNO
	Restituisce un heap generico già inizializzato. 		*/
heap_t get_new_generic_heap()
{
	heap_t heap = (heap_t) malloc(sizeof(struct heap));
	struct operations *op = (struct operations *) malloc(sizeof(struct operations));
	if(op == NULL || heap == NULL)
	{
		if(op == NULL)
			free(op);
		if(heap == NULL)
			free(heap);
		return NULL;
	}

	heap->op = op;
	heap->heapsize = 0;

	return heap;
}

/* 	DESCRIZIONE
	Inserisce un nodo all'interno di un heap generico.
			
	PARAMETRI DI INPUT
	heap : heap_t 		  - Heap generico.
	object : void *object - Oggetto del nuovo nodo.
	priority : float 		  - Priorità del nuovo nodo.
															
	VALORE DI RITORNO
	Restituisce il nuovo heap con il nodo aggiunto. Se heap == NULL, il nuovo nodo 
	non sarà inserito. 															*/
heap_t insert_key(heap_t heap, void *object, float priority)
{
 	void *node = NULL;

	if(heap != NULL)
	{
		if((node = heap->op->Create_node(object, heap->op->SENTINEL)) == NULL)
			return heap;

		heap->heapsize += 1;
		heap->data = heap->op->Insert(heap->data, heap->heapsize-1, node);
		increase_key(heap, heap->heapsize - 1, priority);
	}

	return heap;
}

/* 	DESCRIZIONE
	Aumenta il valore della priorità del nodo i passato in input, al nuovo valore priority, 
	anch'esso passato in input. Si suppone che val sia almeno grande quanto il valore corrente 
	del nodo in posizione i.
			
	PARAMETRI DI INPUT
	heap : heap_t 		- Heap generico.
	i : int 		  	- Posizione del nodo da incrementare.
	val : float 			- Nuova priorità.
																		
	VALORE DI RITORNO
	Restituisce 1 se la funzione è andata a buon fine, -1 altrimenti. 				*/
int increase_key(heap_t heap, int i, float val)
{
	if(heap == NULL)
		return -1;

	void *i_node = heap->op->Get_node(heap->data, i);
	struct basic_node *bn_i_node = heap->op->Get_basic_node(i_node);
	if(heap->op->Compare(val, bn_i_node->priority) < 0)
		return -1;

	bn_i_node->priority = val;
	void *parent_node = heap->op->Get_node(heap->data, parent_index(i));
	struct basic_node *bn_parent_node = heap->op->Get_basic_node(parent_node);
	while(i > 0 && heap->op->Compare(bn_parent_node->priority, bn_i_node->priority) < 0)
	{
		swap_basic_node(bn_parent_node, bn_i_node);

		i = parent_index(i);
		i_node = parent_node;
		bn_i_node = bn_parent_node;
		parent_node = heap->op->Get_node(heap->data, parent_index(i));
		bn_parent_node = heap->op->Get_basic_node(parent_node);
	}
	
	return 1;
}

/* 	DESCRIZIONE
	Decrementa il valore della priorità del nodo i passato in input, al nuovo valore priority, 
	anch'esso passato in input. Si suppone che val sia più piccolo (o più grande)
	del valore corrente del nodo in posizione i.
			
	PARAMETRI DI INPUT
	heap : heap  	 - Heap generico.
	i : void *       - Posizione del nodo da decrementare.
	val : float 		 - Nuova priorità.
															
	VALORE DI RITORNO
	Restituisce un valore che indica la buona riuscita della funzione, o meno. 		*/
int decrease_key(heap_t heap, void *i, float val)
{
	if(heap == NULL || i == NULL)
		return -1;

	struct basic_node *bn_node = heap->op->Get_basic_node(i);
	if(heap->op->Compare(val, bn_node->priority) > 0)
		return -1;
	
	bn_node->priority = val;
	heapfy(heap, i);

	return 1;
}

/* 	DESCRIZIONE
	Dato un heap generico, restituisce l'elemento che si trova alla radice. 
	A seconda se heap è un maxheap o un minheap, l'elemento che si trova
	in radice può essere rispettivamente, l'elemento massimo o l'elemento minimo.
			
	PARAMETRI DI INPUT
	heap : heap_t 		 - Heap generico. 
							
	VALORE DI RITORNO
	Restituisce il nodo base con la massima priorità (o minima priorità). 				*/
struct basic_node *get_head_value(heap_t heap)
{
	if(heap != NULL && heap->heapsize > 0)
	{
		void *nodo = heap->op->Get_node(heap->data, 0);
		return heap->op->Get_basic_node(nodo);
	}
	else
		return NULL;
}

/* 	DESCRIZIONE
	Estrae l'elemento che si trova in testa all'heap.
			
	PARAMETRI DI INPUT
	heap : heap_t 		 - Heap generico. 				
	
	VALORE DI RITORNO
	Restituisce il nuovo heap, senza l'elemento che prima occupava la testa dell'heap. 		*/
heap_t extract_head(heap_t heap)
{
	if(heap != NULL && heap->heapsize > 0) 
	{
		void *head_nodo = heap->op->Get_node(heap->data, 0);
		struct basic_node *bn_head = heap->op->Get_basic_node(head_nodo);

		void *last_nodo = heap->op->Get_node(heap->data, heap->heapsize - 1);
		struct basic_node *bn_last_nodo = heap->op->Get_basic_node(last_nodo);

		swap_basic_node(bn_head, bn_last_nodo);
		destroy_basic_node(bn_last_nodo, heap->op->Destroy_object);
		heap->data = heap->op->Delete(heap->data, heap->heapsize - 1);
		heap->heapsize -= 1;

		/* A questo punto richiamo di nuovo get_node perché nella versione che gestisce un heap di tipo array,
		 * la chiamata a Delete, potrebbe decidere (se necessario) di rimpicciolire l'array. Tale ridimensionamento
		 * provoca la re-allocazione di tutto l'array e di conseguenza cambia l'indirizzo di head_nodo. questo bug mi ha fatto
		 * perdere la testa. 		*/
		head_nodo = heap->op->Get_node(heap->data, 0);
		heapfy(heap, head_nodo);
	}

	return heap;
}

/* 	DESCRIZIONE
 	Questa funzione ha lo scopo, dato l'heap dato in ingresso, di costruire un MAX_HEAP o MIN_HEAP
	e (a seconda dell'attributo heap->type).
			
	PARAMETRI DI INPUT
	heap: heap_t - Heap.
					
	VALORE DI RITORNO
	Restituisce un MAX_HEAP o un MIN_HEAP, a seconda del tipo dell'heap che si è scelto. 				*/
heap_t build_heap(heap_t heap)
{
	int i;

	heap->heapsize = heap->op->Get_length(heap->data);
	for(i = (heap->heapsize / 2) - 1; i >= 0; i--)
	{
		void *nodo = heap->op->Get_node(heap->data, i);
		heapfy(heap, nodo);
	}

	return heap;
}

/* 	DESCRIZIONE
    heapfy sul nodo alla posizione i.nodo
	
	PARAMETRI DI INPUT
	heap : heap_tree  - Heap generico.
	i : void * 		  - Posizione del nodo si cui eseguire heapfy. 			*/
void heapfy(heap_t heap, void *i)
{
	if(heap != NULL)
	{
		void *max_node = NULL;
		struct basic_node *bn_max_node = NULL;

		void *left_node = heap->op->Left(heap->data, i);
		void *right_node = heap->op->Right(heap->data, i);

		struct basic_node *bn_i_node = heap->op->Get_basic_node(i);
		struct basic_node *bn_left_node = heap->op->Get_basic_node(left_node);
		struct basic_node *bn_right_node = heap->op->Get_basic_node(right_node);

		if(heap->op->In_heap(heap, left_node) && heap->op->Compare(bn_left_node->priority, bn_i_node->priority) > 0)
			max_node = left_node, bn_max_node = bn_left_node;
		else
			max_node = i, bn_max_node = bn_i_node;
		
		if(heap->op->In_heap(heap, right_node) && heap->op->Compare(bn_right_node->priority, bn_max_node->priority) > 0)
			max_node = right_node, bn_max_node = bn_right_node;

		if(max_node != i)
		{
			swap_basic_node(bn_max_node, bn_i_node);
			heapfy(heap, max_node);
		}
	}
}

/* 	DESCRIZIONE
	Elimina il nodo che si trova in posizione i dall'heap generico dato in input.
			
	PARAMETRI DI INPUT
	heap : heap_t  - Heap generico.
	i : intero     - Posizione del nodo da eliminare.
												
	VALORE DI RITORNO
	Heap generico che non contiene il nodo che si trovava in posizione i. 					*/
heap_t delete_heap_node(heap_t heap, int i)
{
	if(heap != NULL && heap->heapsize > 0)
	{
		void *i_node = heap->op->Get_node(heap->data, i);
		struct basic_node *bn_i_node = heap->op->Get_basic_node(i_node);

		void *last_node = heap->op->Get_node(heap->data, heap->heapsize - 1);
		struct basic_node *bn_last_node = heap->op->Get_basic_node(last_node);
		swap_basic_node(bn_i_node, bn_last_node);

		if(heap->op->Compare(bn_last_node->priority, bn_i_node->priority) > 0)
			increase_key(heap, i, bn_last_node->priority);
		else
			decrease_key(heap, i_node, bn_last_node->priority);
		destroy_basic_node(bn_last_node, heap->op->Destroy_object);
		heap->data = heap->op->Delete(heap->data, heap->heapsize - 1);
		heap->heapsize -= 1;
	}

	return heap;
}

/* 	DESCRIZIONE
	Crea un nodo base e lo restituisce in output.
			
	PARAMETRI DI INPUT
	object : void * - Object del nodo base.
	priority : float  - Priorità del nodo base.
												
	VALORE DI RITORNO
	Restituisce un nodo base contenente gli elementi passati in input. La funzione Restituisce
	NULL se la malloc fallisce. 																	*/
struct basic_node *create_basic_node(void *object, float priority)
{
	struct basic_node *bn = NULL;

	bn = (struct basic_node *) malloc(sizeof(struct basic_node));
	if(bn != NULL)
	{
		bn->object = object;
		bn->priority = priority;
	}
	
	return bn;
}

/* 	DESCRIZIONE
	Scambia i nodi i e j.
			
	PARAMETRI DI INPUT
	i : struct basic_node * - Nodo da scambiare.
	j : struct basic_node * - Nodo da scambiare. 					*/
void swap_basic_node(struct basic_node *i, struct basic_node *j)
{
	if(i != NULL && j != NULL)
	{
		void *tmp_obj = i->object;
		float tmp_pri = i->priority;
		i->object = j->object;
		i->priority = j->priority;
		j->object = tmp_obj;
		j->priority = tmp_pri;
	}
}

/* 	DESCRIZIONE
	Distrugge (dealloca) il nodo base passato in input.
			
	PARAMETRI DI INPUT
	node : struct basic_node *       - Nodo da deallocare.
	Destroy_obj(obj : void *) : void - Puntatore a funzione che dealloca l'oggetto. 		*/
void destroy_basic_node(struct basic_node *node, void (*Destroy_obj)(void *obj))
{
	if(node != NULL)
	{
		if(node->object != NULL && Destroy_obj != NULL)
			Destroy_obj(node->object);
		free(node);
	}
}

/* 	DESCRIZIONE
	La seguente funzione ha lo scopo di deallocare un heap generico.
			
	PARAMETRI DI INPUT
	heap : heap_t - Heap generico. 											*/
void destroy_generic_heap(heap_t heap)
{
	if(heap != NULL)
	{
		if(heap->op != NULL)
			free(heap->op);
		free(heap);
	}
}

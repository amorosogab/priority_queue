#include <stdlib.h>
#include <float.h>

#include "heap_array.h"
#include "comparator.h"

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di inizializzare e restituire un heap che utilizza 
	un array come rappresentazione interna.
	
	PARAMETRI DI INPUT
	type : enum heap_type - Specifica il tipo dell'heap (MAX_HEAP o MIN_HEAP).

	VALORE DI RITORNO
	Restituisce un heap che utilizza un array come rappresentazione interna. 				*/
heap_t get_new_heap_array(enum heap_type type)
{
	heap_t heap = get_new_generic_heap();
	heap->data = get_new_heap_array_data();
	if(heap->data.heap_array == NULL)
	{
		destroy_generic_heap(heap);
		return NULL;
	}

	heap->type = type;
	heap->op->Create_node = create_array_node;
	heap->op->Insert = array_insert;
	heap->op->Delete = array_delete;
	heap->op->Get_basic_node = get_array_basic_node;
	heap->op->Get_node = get_array_node;
	heap->op->Destroy_object = free;
	heap->op->Get_length = get_array_length;
	heap->op->In_heap = in_heap_array;
	heap->op->Parent = parent_array;
	heap->op->Left = left_array;
	heap->op->Right = right_array;
	if(type == MIN_HEAP)
		heap->op->Compare = compare_min_heap, heap->op->SENTINEL = FLT_MAX; 
	else
		heap->op->Compare = compare_max_heap, heap->op->SENTINEL = -FLT_MAX;
  
	return heap;
}

/* 	DESCRIZIONE
	La seguente funzione crea un nodo della struttura dati heap. Nel caso della rapprentazione
	ad array, la seguente funzione crea un basic_node.

	PARAMETRI DI INPUT
	object : void * - oggetto.
	priority : float  - priorità.
												
	VALORE DI RITORNO
	Restituisce un basic_node contenente i dati passati in input. 					*/
void *create_array_node(void *object, float priority)
{
	return create_basic_node(object, priority);
}

/* 	DESCRIZIONE
	Alloca è inizializza la struttura fisica dell'heap, che in questo caso è un array.
			
	VALORE DI RITORNO
	Struttura fisica inizializzata di un heap_array. 									*/
data_t get_new_heap_array_data()
{
	data_t data;

	data.heap_array = (struct array_rappresentation *) malloc(sizeof(struct array_rappresentation));
	if(data.heap_array != NULL)
	{
		data.heap_array->A = NULL;
		data.heap_array->last = -1;
		data.heap_array->n_blocks = 0;
	}
	
	return data;
}

/* 	DESCRIZIONE
	Inserisce una coppia <object, priority> nella posizione i dell'heap_array.
	Utilizzando questa funzione, un nodo può essere inserito in una posizione qualsiasi
	A[i] con 0 <= i <= last + 1. Se un nodo viene inserito in una posizione i <= last,
	è il chiamante che deve farsi carico di deallocare correttamente il nodo che occupata
	quella posizione. Se i == last + 1, viene fatto un controllo per determinare se è
	necessario estendere l'array.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura fisica dell'heap.
	i : int 		   - Posizione di inserimento.
	node : void * 	   - Nodo da inserire nella posizione i.
																								
	VALORE DI RITORNO
	Nuovo heap contenente il nodo <object, priority> nell'i-esima posizione. 			*/
data_t array_insert(data_t heap_data, int i, void *node)
{
	if(heap_data.heap_array != NULL && i <= heap_data.heap_array->last + 1)
	{
		/* i == last + 1. In questo caso devi verificare se l'array deve essere esteso. */
		if(i > heap_data.heap_array->last)
		{
			if(heap_data.heap_array->A == NULL)
			{
				heap_data.heap_array->A = (struct basic_node **) calloc(BLOCK, sizeof(struct basic_node *));
				if(heap_data.heap_array->A != NULL)
					heap_data.heap_array->n_blocks += 1;
				else
					return heap_data;
			}
			else if(heap_data.heap_array->n_blocks * BLOCK <= heap_data.heap_array->last + 1)
			{
				data_t new_heap_data = get_new_heap_array_data();
				if(new_heap_data.heap_array != NULL)
				{
					new_heap_data.heap_array->A = (struct basic_node **) 
												   calloc(BLOCK * (heap_data.heap_array->n_blocks + 1), 
														  sizeof(struct basic_node *));
					if(new_heap_data.heap_array->A != NULL)
					{
						new_heap_data.heap_array->n_blocks = heap_data.heap_array->n_blocks + 1;
						int i = 0;
						for( ; i <= heap_data.heap_array->last; i++)
							new_heap_data.heap_array->A[i] = heap_data.heap_array->A[i];
				
						free(heap_data.heap_array->A);
						free(heap_data.heap_array);
						heap_data = new_heap_data;
					}
					else
					{
						free(new_heap_data.heap_array);
						return heap_data;
					}
				}
				else
					return heap_data;
			}

			heap_data.heap_array->last = i;
		}
		
		/* se i <= last, l'elemento verrà messo in una posizione gia occupata da un altro elemento. Il puntatore
		   all'elemento i verrà perso se il chiamante non salverà il puntatore, oppure eliminerà l'oggetto presente
		   in quella posizione. 	*/
		heap_data.heap_array->A[i] = (struct basic_node *) node;
	}

	return heap_data;
}

/* 	DESCRIZIONE
	Cancella il nodo che occupa la posizione i nell'heap. Nel caso della rappresentazione ad
	array, questa funzione decide anche se è il caso di 'restringere' l'array. Si noti che è
	compito del chiamante deallocare correttamente il nodo base contenuto alla posizione i.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Heap.
	i : int     	   - Posizione dell'elemento da eliminare.
																
	VALORE DI RITORNO
	heap_data originario, senza il nodo che occupava la posizione i. 						

	WARNING
	Usare direttamente questa funzione con un indice i < heapsize-1, potrebbe creare qualche incoerenza nella struttura. */
data_t array_delete(data_t heap_data, int i)
{
	if(heap_data.heap_array != NULL)
	{
		heap_data.heap_array->A[i] = NULL;

		if(i <= heap_data.heap_array->last)
		{
			if(i == heap_data.heap_array->last)
				heap_data.heap_array->last--;
			/* Posizione last all'ultimo indice valido (se esiste) */
			for( ; heap_data.heap_array->last >= 0 && heap_data.heap_array->A[heap_data.heap_array->last] == NULL; heap_data.heap_array->last--)
				;

			/* Se almeno due blocchi dell'array sono completamente liberi, allora elimina tutti 
		   		i blocchi completamente liberi, tranne uno. */
			if(heap_data.heap_array->n_blocks - ((heap_data.heap_array->last+1) / BLOCK) >= 2)
			{
				data_t new_heap_data = get_new_heap_array_data();
				new_heap_data.heap_array->n_blocks = ((heap_data.heap_array->last+1) / BLOCK) + 1;
				new_heap_data.heap_array->A = (struct basic_node **) 
											   calloc(BLOCK * new_heap_data.heap_array->n_blocks, 
													  sizeof(struct basic_node *));
				new_heap_data.heap_array->last = heap_data.heap_array->last;
				if(new_heap_data.heap_array->A != NULL)
				{
					int k = 0;
					for( ; k <= new_heap_data.heap_array->last; k++)
						new_heap_data.heap_array->A[k] = heap_data.heap_array->A[k];

					free(heap_data.heap_array->A);
					free(heap_data.heap_array);
					heap_data = new_heap_data;
				}
			}		
		}
	}

	return heap_data;
}

/* 	DESCRIZIONE
	Distrugge un heap di tipo heap_tree.
			
	PARAMETRI DI INPUT
	heap : heap_t - heap. 						*/
void destroy_heap_array(heap_t heap)
{
	destroy_heap_array_data(heap);
	destroy_generic_heap(heap);
}

/* 	DESCRIZIONE
	Distrugge l'intera struttura dati dell'heap.
			
	PARAMETRI DI INPUT
	heap : heap_t - Heap data. 						*/
void destroy_heap_array_data(heap_t heap)
{
	int i;

	if(heap != NULL)
	{
		for(i = heap->data.heap_array->last; i >= 0; i--)
			destroy_basic_node(heap->data.heap_array->A[i], heap->op->Destroy_object);

		free(heap->data.heap_array->A);
		free(heap->data.heap_array);
	}
}

/* 	DESCRIZIONE
	La seguente funzione ci dice se l'elemento elem, appartiene all'heap (poiché un elemento 
	può essere nella struttura, non facendo parte dell'heap). Questa funzione viene usata
	in heapfy.
				
	PARAMETRI DI INPUT
	heap : heap_t - heap.
	elem : void * - Elemento da testare.
	
	VALORE DI RITORNO
	Restituisce 1 se l'elemento elem è nell'heap, 0 altrimenti. 							*/
int in_heap_array(heap_t heap, void *elem)
{
	int i = (((struct basic_node **) elem) - heap->data.heap_array->A);

	if(i >= 0 && i < heap->heapsize)
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Estrae il basic_node da node, e lo restituisce in output. Nel caso della rapprentazione 
	ad array, elem è l'indirizzo di memoria contenente il nodo base.
				
	PARAMETRI DI INPUT
	node : void * 	   - Nodo
												
	VALORE DI RITORNO
	Nodo base. 																				*/
struct basic_node *get_array_basic_node(void *elem)
{
	if(elem != NULL)
		return *((struct basic_node **) elem);
	else
		return NULL;
}

/* 	DESCRIZIONE
	Recupera il nodo i-esimo dalla struttura dati, e lo restituisce in output.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura dati heap.
	i : int 	       - indice.
																
	VALORE DI RITORNO
	Restituisce il nodo i. 															*/
void *get_array_node(data_t heap_data, int i)
{
	if(i < 0 || i > heap_data.heap_array->last)
		return NULL;
	else
		return &(heap_data.heap_array->A[i]);
}

/* 	DESCRIZIONE
	Calcola il numero di elementi presenti nella struttura. Tale valore può essere diverso da heapsize.
	Nel caso della rappresentazione ad array, la variabile last contiene la posizione dell'ultimo
	elemento e quindi sommata ad 1, ci da il numero di elementi della struttura.
				
	PARAMETRI DI INPUT
	heap_data: data_t - Struttura heap
												
	VALORE DI RITORNO
	Restituisce il numero di elementi presenti nella struttura. 								*/
int get_array_length(data_t heap_data)
{
	return heap_data.heap_array->last + 1;
}

/* 	DESCRIZIONE
	Dato l'elemento in input, restituisci la posizione del padre
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura heap.
	i : void * 	   	   - Posizione i.
																
	VALORE DI RITORNO
	Restituisce la posizione del padre. 										*/
void *parent_array(data_t heap_data, void *i)
{
	int offset = (((struct basic_node **) i) - &(heap_data.heap_array->A[0]));

	return get_array_node(heap_data, parent_index(offset));
}

/* 	DESCRIZIONE
	Dato l'elemento in input, restituisci la posizione del figlio sinistro.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura heap.
	i : void * 	       - Posizione i.
																
	VALORE DI RITORNO
	Restituisce la posizione del figlio sinistro. 								*/
void *left_array(data_t heap_data, void *i)
{
	int offset = (((struct basic_node **) i) - &(heap_data.heap_array->A[0]));

	return get_array_node(heap_data, left_index(offset));
}

/* 	DESCRIZIONE
	Dato l'elemento in input, restituisci la posizione del figlio destro.
				
	PARAMETRI DI INPUT
	heap_data : data_t - Struttura heap.
	i : void * 	       - Posizione i.
																
	VALORE DI RITORNO
	Restituisce la posizione del figlio destro. 							*/
void *right_array(data_t heap_data, void *i)
{
	int offset = (((struct basic_node **) i) - &(heap_data.heap_array->A[0]));

	return get_array_node(heap_data, right_index(offset));
}

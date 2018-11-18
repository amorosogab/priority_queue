/* 	DESCRIZIONE
	Calcola la posizione del padre del nodo di indice i dato in input.
						
	PARAMETRI DI INPUT
	i : int 													*/
#define parent_index(i) 	(((i+1) / 2) - 1)

/* 	DESCRIZIONE
	Calcola la posizione del figlio sinistro del nodo di indice i dato in input.
			
	PARAMETRI DI INPUT
	i : int 															*/
#define left_index(i) 		(2*(i) + 1)

/* 	DESCRIZIONE
	Calcola la posizione del nodo destro del nodo di indice i dato in input.
			
	PARAMETRI DI INPUT
	i : int 															*/
#define right_index(i) 		(2*(i) + 2) 

/* Struttura che rappresenta un nodo heap base. */
struct basic_node 
{
	void *object; 	/* Oggetto generico. */
	float  priority;  /* Priorità dell'oggetto. */
};

typedef union data data_t;
union data  /* Unione che indica come può essere strutturato un heap. Secondo un array oppure un albero. */
{
	struct array_rappresentation *heap_array; 	/* Rappresentazione ad array. */
	struct tree_rappresentation  *heap_tree;    /* Rappresentazione ad albero. */
};

/* Enumerazione che serve per specificare se un heap è un MIN_HEAP oppure un MAX_HEAP */
enum heap_type
{
	MIN_HEAP,
	MAX_HEAP
};

/* Rappresenta il tipo di un heap generico. */
typedef struct heap * heap_t;
struct heap 	/* Struttura che rappresenta un heap generico. */
{
	data_t 			  data; 		/* Data. Rappresentazione degli elementi secondo una struttura generica. */
	int 			  heapsize; 	/* Dimensione dell'heap. */
	enum heap_type 	  type; 		/* Tipo dell'heap. Può essere un MIN-HEAP o un MAX-HEAP. */
	struct operations *op; 			/* Struttura delle operazioni. */
};

struct operations 
{
    /* Valore di sentinella che viene utilizzato per inizializzare un elemento appena inserito nell'heap. 
	   Tale valore vale -FLT_MAX nel caso di un MAX_HEAP, FLT_MAX nel caso di un MIN_HEAP.  					*/
	float 		     	SENTINEL;

	/* DESCRIZIONE
	   La seguente funzione crea un nodo della struttura dati heap.
					
	   PARAMETRI DI INPUT
	   object : void * - oggetto.
	   priority : float  - priorità
																	
	   VALORE DI RITORNO
       Restituisce un nodo della struttura dati. 										*/
	void *			   (*Create_node)(void *object, float priority);

	/* 	DESCRIZIONE
		Inserisce una coppia <object, priority> nella posizione i dell'heap.
					
		PARAMETRI DI INPUT
		heap_data : data_t - Struttura fisica dell'heap.
		i : int 		   - Posizione di inserimento.
		node : void * 	   - Nodo da inserire nella posizione i.
																									
		VALORE DI RITORNO
		Nuovo heap contenente il nodo <object, priority> nell'i-esima posizione. 		*/
	data_t 			   (*Insert)(data_t heap_data, int i, void *node);

	/* 	DESCRIZIONE
		Cancella il nodo che occupa la posizione i nell'heap.
					
		PARAMETRI DI INPUT
		heap_data : data_t - Heap.
		i : int     	   - Posizione dell'elemento da eliminare.
																	
		VALORE DI RITORNO
		Restituisce l'heap originariamente passato in input, senza il nodo i. 		*/										
	data_t 			   (*Delete)(data_t heap_data, int i);

	/* 	DESCRIZIONE
		Estrae il basic_node da node, e lo restituisce in output.
					
		PARAMETRI DI INPUT
		node : void * 	   - Nodo
													
		VALORE DI RITORNO
		Nodo base. 													*/
	struct basic_node *(*Get_basic_node)(void *i);

	/* 	DESCRIZIONE
		Recupera il nodo i-esimo dalla struttura dati, e lo restituisce in output.
					
		PARAMETRI DI INPUT
		heap_data : data_t - Struttura dati heap.
		i : int 	   	   - indice.
																
		VALORE DI RITORNO
		Restituisce il nodo i. 																*/
	void *			   (*Get_node)(data_t heap_data, int i);

	/* 	DESCRIZIONE
		Distrugge l'oggetto dato in input.
					
		PARAMETRI DI INPUT
		void *object - Oggetto da distruggere. 				*/
	void			   (*Destroy_object)(void *object);

	/* 	DESCRIZIONE
		Questa funzione ha lo scopo di comparare due elementi, per determinare, in base 
		all'ordinamento scelto (MIN-HEAP o MAX-HEAP), quale	dei due elementi debba
		comparire più in alto nella coda di priorità. Per esempio, supponiamo che i abbia 
		priorità 2 e j abbia priorità 7. Nel caso di un ordinamento MAX-HEAP, la funzione 
		decreterà che il nodo j deve comparire prima di j nel MAX-HEAP; nel caso di un MIN-HEAP, 
		la funzione decreterà che il nodo i deve comparire prima di j.
					
		PARAMETRI DI INPUT
		i : float - Intero da comparare.
		j : float - Intero da comparare.
																	
		VALORE DI RITORNO
		La funzione restituisce un valore strettamente maggiore di 0 se i, nell'ordinamento scelto, deve 
		comparire prima di j. In caso contrario la funzione restituirà un valore strettamente minore di 0. 
		Se la priorità dei due elementi è uguale, la funzione restituirà 0. 								*/
	int 			   (*Compare)(float i, float j);

	/* 	DESCRIZIONE
		Calcola il numero di elementi presenti nella struttura. Tale valore può essere diverso da heapsize.
					
		PARAMETRI DI INPUT
		heap_data: data_t - Struttura heap
													
		VALORE DI RITORNO
		Restituisce il numero di elementi presenti nella struttura. 										*/
	int 			   (*Get_length)(data_t heap_data);

	/* 	DESCRIZIONE
		La seguente funzione ci dice se l'elemento elem, appartiene all'heap (poiché un elemento può essere 
		nella struttura, non facendo parte dell'heap).
					
		PARAMETRI DI INPUT
		heap : heap_t - heap.
		elem : void * - Elemento da testare. 																*/
	int 			   (*In_heap)(heap_t heap, void *elem);

	/* 	DESCRIZIONE
		Dato l'elemento in input, restituisci la posizione del padre
					
		PARAMETRI DI INPUT
		heap_data : data_t - Struttura heap.
		i : void * 	   	   - Posizione i.
																	
		VALORE DI RITORNO
		Restituisce la posizione del padre. 							*/
	void *			   (*Parent)(data_t heap_data, void *i);

	/* 	DESCRIZIONE
		Dato l'elemento in input, restituisci la posizione del figlio sinistro.
					
		PARAMETRI DI INPUT
		heap_data : data_t - Struttura heap.
		i : void * 	       - Posizione i.
																	
		VALORE DI RITORNO
		Restituisce la posizione del figlio sinistro. 								*/
	void *			   (*Left)(data_t heap_data, void *i);

	/* 	DESCRIZIONE
		Dato l'elemento in input, restituisci la posizione del figlio destro.
					
		PARAMETRI DI INPUT
		heap_data : data_t - Struttura heap.
		i : void * 	   - Posizione i.
																	
		VALORE DI RITORNO
		Restituisce la posizione del figlio destro. 							*/
	void *			   (*Right)(data_t heap_data, void *i);
};



int increase_key(heap_t heap, int i, float val);
int decrease_key(heap_t heap, void *i, float val);

heap_t get_new_generic_heap();
heap_t extract_head(heap_t heap);
heap_t delete_heap_node(heap_t heap, int i);
heap_t insert_key(heap_t heap, void *object, float priority);
heap_t build_heap(heap_t heap);

struct basic_node *create_basic_node(void *object, float priority);
struct basic_node *get_head_value(heap_t heap);

void swap_basic_node(struct basic_node *i, struct basic_node *j);
void destroy_basic_node(struct basic_node *node, void (*Destroy_obj)(void *obj));
void heapfy(heap_t heap, void *i);

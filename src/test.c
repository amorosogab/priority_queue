#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#define N_CMD	15
#define LEN_CMD	50

/* 	COMMANDS 	<name_opitions>_<fun>	*/
#define MIN_HEAP_1   "MIN"
#define MAX_HEAP_1	 "MAX" 
#define HEAP_ARRAY_1 "A"
#define HEAP_TREE_1	 "T"

#define EXTRACT_ALL_NODE "ALL"

#define RANDOM_INSERT_3_11 "R"
#define NORMAL_INSERT_3_11 "N"


#include "heap_tree.h"
#include "heap_array.h"
#include "queue/queue.h"

void stampa_heap(heap_t heap, int rappresentation);
void stampa_heap_array(data_t data);
void stampa_heap_tree(data_t data);
void ordered_insert(heap_t heap, int n_node, int mode);
void non_ordered_insert(heap_t heap, int n_node, int mode);

int rand_num(int a, int b);

int getcmds(char cmd[][LEN_CMD]);
int is_integer(char *str);

int main(void)
{
	int esci;
	int rappresentation; /* 0 = heap array; 1 = heap tree */
	int type; 			 /* 0 = min-heap; 1 = max-heap */
	heap_t heap = NULL;
	srand(time(NULL));

	do
	{
		int fun, n_args;
		char cmd[N_CMD][LEN_CMD];
		esci = 0;

		printf("1: Crea heap.\n"
			   "2: Stampa heap.\n"
			   "3: Inserisci chiave.\n"
			   "4: Incrementa chiave. \n"
			   "5: Decrementa chiave.\n"
			   "6: Stampa l'elemento in testa.\n"
			   "7: Estrai l'elemento in testa.\n"
			   "8: Costruisci heap.\n"
			   "9: Esegui heapfy.\n"
			   "10: Elimina una chiave.\n"
			   "11: Inserisci nella struttura.\n"
			   "12: Elimina dalla struttura.\n"
			   "13: Distruggi heap."
			   "\n"
			   "Inserisci Comando: ");
		scanf("%d", &fun);

		switch(fun)
		{
			case 1:
			{
				printf("\n1: Crea heap.\n"
					   "Sintassi: <rappresentazione> <tipo>\n"
					   "<rappresentazione> ::= %s (rappresentazione ad array) | %s (rappresentazione ad albero)\n"
					   "<tipo> ::= %s (min-heap) | %s (max-heap)"
					   "es: > %s %s crea un heap rappresentato con array, di tipo max-heap\n"
					   "> ", HEAP_ARRAY_1, HEAP_TREE_1, MIN_HEAP_1, MAX_HEAP_1, HEAP_ARRAY_1, MAX_HEAP_1);
				if((n_args = getcmds(cmd)) < 2 											   ||
				   (strcmp(cmd[0], HEAP_ARRAY_1) != 0 && strcmp(cmd[0], HEAP_TREE_1) != 0) ||
				   (strcmp(cmd[1], MIN_HEAP_1) != 0 && strcmp(cmd[1], MAX_HEAP_1) != 0))
				{
					printf("Errore di sintassi\n");
					break;
				}

				rappresentation = strcmp(cmd[0], HEAP_ARRAY_1) == 0 ? 0 : strcmp(cmd[0], HEAP_TREE_1) == 0 ? 1 : -1;
				type = strcmp(cmd[1], MIN_HEAP_1) == 0 ? type = MIN_HEAP : strcmp(cmd[1], MAX_HEAP_1) == 0 ? MAX_HEAP : -1;
				if(type == 0)
					type = MIN_HEAP;
				else
					type = MAX_HEAP;

				if(rappresentation == 0)
					heap = get_new_heap_array(type);
				else
					heap = get_new_heap_tree(type);
			}
			break;

			case 2:
			{
				printf("\n2: Stampa heap.\n");
				if(heap)
					stampa_heap(heap, rappresentation);
			}
			break;

			case 3:
			{
				int n_node;

				printf("\n3: Inserisci chiave.\n"
					   "Sintassi: [<modalità>] <numero_nodi>\n"
					   "modalità ::= %s (inserimento random) | %s (inserimento manuale) (default: M)\n"
					   "> ", RANDOM_INSERT_3_11, NORMAL_INSERT_3_11);
				if((n_args = getcmds(cmd)) < 1 		   																						 || 
				   (n_args > 1 && !is_integer(cmd[1]) && strcmp(cmd[0], RANDOM_INSERT_3_11) != 0 && strcmp(cmd[0], NORMAL_INSERT_3_11) != 0) ||
				   (n_args == 1 && !is_integer(cmd[0])))
				{
					printf("Errore di sintassi\n");
					break;
				}
				if(!heap)
				{
					printf("Heap non esistente\n");
					break;
				}

				int err;
				if(n_args == 1)
				{
					if((err = sscanf(cmd[0], "%d", &n_node)) && n_node > 0)
						ordered_insert(heap, n_node, 0);
					else if(err)
						printf("<numero_nodi> troppo grande\n");
					else
						printf("<numero_nodi> deve essere maggiore di 0\n");
				}	
				else
				{
					if((err = sscanf(cmd[1], "%d", &n_node)) && n_node > 0)
					{
						if(strcmp(cmd[0], RANDOM_INSERT_3_11) == 0)
							ordered_insert(heap, n_node, 1);
						else /* cmd[0] == NORMAL_INSERT_3_11 */
							ordered_insert(heap, n_node, 0);
					}
				}	
			}
			break;

			case 4:
			{
				float new_priority;
				int i;

				printf("\n4: Incrementa chiave. \n"
					   "Sintassi: <posizione_nodo> <nuova_priorita>\n"
					   "> ");
				if((n_args = getcmds(cmd)) < 2)
					printf("Errore di sintassi.\n");
				else if(!is_integer(cmd[0]) || !is_integer(cmd[1]) || !sscanf(cmd[0], "%d", &i) || !sscanf(cmd[1], "%f", &new_priority))
					printf("Errore d'inserimento.\n");
				else
				{
					if(increase_key(heap, i, new_priority))
						printf("incremento riuscito.\n");
					else
						printf("incremento fallito.\n");
				}
			}
			break;

			case 5:
			{
				float new_priority;
				int i;

				printf("\n5: Decrementa chiave. \n"
					   "Sintassi: <posizione_nodo> <nuova_priorita>\n"
					   "> ");
				if((n_args = getcmds(cmd)) < 2)
					printf("Errore di sintassi.\n");
				else if(!is_integer(cmd[0]) || !is_integer(cmd[1]) || !sscanf(cmd[0], "%d", &i) || !sscanf(cmd[1], "%f", &new_priority))
					printf("Errore d'inserimento.\n");
				else
				{
					void *node_heap = heap->op->Get_node(heap->data, i);
					if(!node_heap)
					{
						printf("Nodo non esistente\n");
						break;
					}

					if(decrease_key(heap, node_heap, new_priority))
						printf("decremento riuscito.\n");
					else
						printf("decremento fallito.\n");
				}
			}
			break;

			case 6:
			{
				struct basic_node *bn_head = get_head_value(heap);
				if(!heap)
				{
					printf("\nHeap non esistente.\n");
					break;
				}
				printf("6: Stampa l'elemento in testa.\n");

				if(bn_head)
					if(bn_head->object)
					   printf("Testa: %d\n", *((int *) bn_head->object));
					else
						printf("Testa: NULL\n");
				else
					printf("Testa non esistente.\n");
			}
			break;

			case 7:
			{
				int n_args, n_node;

				if(!heap)
				{
					printf("\nHeap non esistente.\n");
					break;
				}

				printf("7: Estrai l'elemento in testa.\n"
					   "Sintassi: <numero_nodi> (estrae <numero_nodi> nodi) | %s (estrae tutti i nodi)\n"
					   "> ", EXTRACT_ALL_NODE);
				if((n_args = getcmds(cmd)) != 1 ||
				   (strcmp(cmd[0], EXTRACT_ALL_NODE) != 0 && !is_integer(cmd[0])))
				{
					printf("Errore di sintassi.\n");
					break;
				}

				if(strcmp(cmd[0], EXTRACT_ALL_NODE) == 0)
					n_node = heap->op->Get_length(heap->data);
				else
					if(sscanf(cmd[0], "%d", &n_node))
						printf("Errore inserimento.\n");

				int i;
				for(i = 1; i <= n_node; i++)
				{
					struct basic_node *bn_head = get_head_value(heap);
					if(bn_head && bn_head->object)
					   printf("i: %d \t (object: %d; priority: %f)\n", i, *((int *) bn_head->object), bn_head->priority);
					heap = extract_head(heap);
				}
				printf("Testa estratta.\n");
			}
			break;

			case 8:
			{
				if(!heap)
				{
					printf("\nHeap non esistente.\n");
					break;
				}
				printf("8: Costruisci heap.\n");
				heap = build_heap(heap);
				printf("Heap costruito.\n");
			}
			break;

			case 9:
			{
				int i;

				printf("\n9: Esegui heapfy.\n"
					   "Inserisci la posizione del nodo a partire dalla quale eseguire heapfy: ");
				scanf("%d", &i);

				void *node_heap = heap->op->Get_node(heap->data, i);
				if(!node_heap)
				{
					printf("Nodo non esistente\n");
					break;
				}
				heapfy(heap, node_heap);
				printf("heapfy eseguito\n");
			}
			break;

			case 10:
			{
				int i;

				printf("\n10: Elimina una chiave.\n"
					   "Inserisci la posizione del nodo da eliminare: ");
				scanf("%d", &i);
				heap = delete_heap_node(heap, i);
				printf("Nodo eliminato\n");
			}
			break;

			case 11:
			{
				int n_node;

				printf("\n11: Inserisci nella struttura.\n"
					   "Sintassi: [<modalità>] <numero_nodi>\n"
					   "modalità ::= %s (inserimento random) | %s (inserimento manuale) (default: M)\n"
					   "WARNING: Questa funzione agisce solo sulla struttura fisica. Usare con cautela.\n"
					   "> ", RANDOM_INSERT_3_11, NORMAL_INSERT_3_11);
				if((n_args = getcmds(cmd)) < 1 		   || 
				   (n_args > 1 && !is_integer(cmd[0]) && strcmp(cmd[0], RANDOM_INSERT_3_11) != 0 && strcmp(cmd[0], NORMAL_INSERT_3_11) != 0) ||
				   (n_args == 1 && !is_integer(cmd[0])))
				{
					printf("Errore di sintassi\n");
					break;
				}
				if(!heap)
				{
					printf("Heap non esistente\n");
					break;
				}

				int err;
				if(n_args == 1)
				{
					if((err = sscanf(cmd[0], "%d", &n_node)) && n_node > 0)
						non_ordered_insert(heap, n_node, 0);
					else if(err)
						printf("<numero_nodi> troppo grande\n");
					else
						printf("<numero_nodi> deve essere maggiore di 0\n");
				}	
				else
				{
					if((err = sscanf(cmd[1], "%d", &n_node)) && n_node > 0)
					{
						if(strcmp(cmd[0], RANDOM_INSERT_3_11) == 0)
							non_ordered_insert(heap, n_node, 1);
						else /* cmd[0] == NORMAL_INSERT_3_11 */
							non_ordered_insert(heap, n_node, 0);
					}
				}
			}
			break;

			case 12:
			{
				int i;

				if(!heap)
				{
					printf("Heap non esistente.\n");
					break;
				}
				printf("\n12: Elimina dalla struttura.\n"
					   "WARNING: Questa funzione agisce solo sulla struttura fisica. Usare con cautela.\n"
					   "Inserisci la posizione: ");
				scanf("%d", &i);

				heap->data = heap->op->Delete(heap->data, i);
			}
			break;

			case 13:
			{
				printf("Distruggi hap.\n");
				if(rappresentation == 0)
					destroy_heap_array(heap);
				else
					destroy_heap_tree(heap);
				printf("Fatto.\n");
			}

			default: esci = 1;
		}

		printf("\n\n");
	}while(!esci);
	
	return 0;
}

void stampa_heap(heap_t heap, int rappresentation)
{
	if(heap)
	{
		printf("heap: %p\n"
			   "heapsize: %d\n", heap, heap->heapsize);
		if(heap->type == MAX_HEAP)
			printf("type: MAX_HEAP\n");
		else
			printf("type: MIN_HEAP\n");

		if(rappresentation == 0)
			stampa_heap_array(heap->data);
		else
			stampa_heap_tree(heap->data);
	}
}

void stampa_heap_array(data_t data)
{
	int i;

	printf("last: %d\n" 
		   "n_blocks: %d\n", data.heap_array->last, data.heap_array->n_blocks);
	for(i = 0; i < data.heap_array->n_blocks * BLOCK; i++)
		if(data.heap_array->A[i])
			if(data.heap_array->A[i]->object)
				printf("%d: \t object: %d \t priority: %f\n", i, *((int *) data.heap_array->A[i]->object),
															  		data.heap_array->A[i]->priority);
			else
				printf("%d: \t object: NULL \t priority: %f\n", i, data.heap_array->A[i]->priority);
		else
			printf("%d: \t NULL\n", i);
}

void stampa_heap_tree(data_t data)
{
	int i = 0;;

	printf("root: %p\n"
		   "last: %p\n"
		   "n: %d\n", data.heap_tree->root, data.heap_tree->last, data.heap_tree->n);
	if(data.heap_tree->n <= 0)
		return;

	/* Scorri albero in ampiezza. */
	struct tree_node *root = data.heap_tree->root;
	queue_t Q = new_queue(0);
	Q = enqueue(Q, root);
	while(Q && i < data.heap_tree->n)
	{
		root = top(Q);

		if(root && root->node)
			if(root->node->object)
				printf("%d: \t object: %d \t priority: %f\n", i, *((int *)(root->node->object)), root->node->priority);
			else
				printf("%d: \t object: NULL \t priority: %f\n", i, root->node->priority);
		else if(!root)
			printf("NULL\n");
		else
			printf("%d: \t NULL\n", i);

		Q = dequeue(Q);
		if(root)
		{
			Q = enqueue(Q, root->left);
			Q = enqueue(Q, root->right);
		}

		i++;
	}

	queue_destroy(Q);
}

int getcmds(char cmd[][LEN_CMD])
{
	int a, b;
	char c, prev_c;

	if(cmd)
	{
		/* clear input buffer */
		while((c = getchar()) != '\n' && c != EOF)
			;

		prev_c = EOF;
		a = b = 0;
		while(a < N_CMD && (c = getchar()) != '\n' && c != EOF)
		{
			if(c != ' ' && b < LEN_CMD)
				cmd[a][b++] = c;
			else
			{
				cmd[a][b] = 0;
				if(b != 0)
					a++;
				b = 0;
			}

			prev_c = c;
		}
	}

	if(a < N_CMD && b < LEN_CMD)
		cmd[a][b]=0;

	if(prev_c == ' ')
		return a;
	else
		return a+1;
}

int is_integer(char *str)
{
	int i = 0;
	while(str[i] != 0 && (isdigit(str[i]) || str[i] == '-' || str[i] == '+'))
		i++;

	if(str[i] == 0)
		return 1;
	else
		return 0;
}

void ordered_insert(heap_t heap, int n_node, int mode)
{
	int i = 0;
	for(i = 0; i < n_node; i++)
	{
		float priority;
		int *key = (int *) malloc(sizeof(int));
		if(!key)
		{
			printf("allocazione key fallita\n");
			break;
		}

		printf("elemento %d\n", i);
		if(mode == 0) /* input */
		{
			printf("Key: ");
			scanf("%d", key);
			printf("Priorità: ");
			scanf("%f", &priority);
		}
		else
		{
			*key = rand_num(-100, 100);
			priority = rand_num(-100, 100);
			printf("key: %d \t priority: %f\n", *key, priority);
		}

		heap = insert_key(heap, key, priority);
	}
}

void non_ordered_insert(heap_t heap, int n_node, int mode)
{
	int i = 0;
	for(i = 0; i < n_node; i++)
	{
		float priority;
		int *key = (int *) malloc(sizeof(int));
		if(!key)
		{
			printf("allocazione key fallita\n");
			break;
		}
		printf("\nmode: %d\n", mode);
		printf("elemento %d\n", i);
		if(mode == 0) /* input */
		{
			printf("Key: ");
			scanf("%d", key);
			printf("Priorità: ");
			scanf("%f", &priority);
		}
		else
		{
			*key = rand_num(-100, 100);
			priority = rand_num(-100, 100);
			printf("key: %d \t priority: %f\n", *key, priority);
		}

		heap->data = heap->op->Insert(heap->data, i, heap->op->Create_node(key, priority));
	}
}

int rand_num(int a, int b)
{
	return a + (rand() % (b-a+1));
}

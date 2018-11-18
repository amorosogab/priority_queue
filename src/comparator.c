#include "comparator.h"

/* 	DESCRIZIONE
	Confronta due elementi di un max-heap.
			
	PARAMETRI DI INPUT
	i: float - Primo elemento.
	j: float - Secondo elemento.
												
	VALORE DI RITORNO
	Restituisce 1 se i > j, -1 se i < j, 0 se i = j 			*/
int compare_max_heap(float i, float j)
{
	if(i > j)
		return 1;
	else if(i < j)
		return -1;
	else
		return 0;
}

/* 	DESCRIZIONE
 	Confronta due elementi di un min-heap.
 
    PARAMETRI DI INPUT
 	i: float - Primo elemento.
 	j: float - Secondo elemento.
 
 	VALORE DI RITORNO
 	Restituisce -1 se i > j, 1 se i < j, 0 se i == j. 	*/
int compare_min_heap(float i, float j)
{
	if(i > j)
		return -1;
	else if(i < j)
		return 1;
	else
		return 0;
}

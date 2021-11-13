#include "../include/heuristic.h"


/**
 * Launch the resolution using heuristics.
 *
 * Creates a chainedList according to schedulingMode, then resolves the problem with the heuristic according to heuristicMode. Use an Instance.
 */
int* Heuristic_getDirectCoding(int heuristicMode, int schedulingMode, Instance *instance)
{
	int* directCoding = NULL;
	ChainedList *chainedList = ChainedList_generationMode(instance, schedulingMode);

	if (heuristicMode <= 2)
	{
		directCoding = Heuristic(heuristicMode, chainedList, instance);
	}
	else
	{
		printf("\n-Error- (heuristic.c) Heuristic_type : %d is not an available heuristic mode.\n", heuristicMode);
		exit(EXIT_FAILURE);
	}
	
	if (!directCoding)
	{
		printf("\n-Error- (structures/heuristic.c) Heuristic_type : didn't return directCoding.\n");
	}
	return directCoding;
}


/**
 * Heuristic static or dynamic according to heuristicMode
 *
 * Gives directCoding computed using an heuristic (static or dynamic according to heuristicMode)
 */
int* Heuristic(int heuristicMode, ChainedList *chainedList, Instance *instance)
{
	//init directCoding
	int *directCoding = (int*) malloc(instance->nbObject*sizeof(int));
	for (int i = 0; i < instance->nbObject; i++)
	{
		directCoding[i] = 0;
	}

	while (chainedList)
	{
		if (heuristicMode == 2)
		{
			if  (
					chainedList->next
					&&
					(
						instance->objects[chainedList->place]->value
						<
						instance->objects[chainedList->next->place]->value
					)
				)
			{
				int tmp = chainedList->place;
				chainedList->place = chainedList->next->place;
				chainedList->next->place = tmp;
			}
		}

		int iterator = instance->objects[chainedList->place]->id - 1;
		//printf("%d %d\n", iterator, instance->objects[iterator]->value); //debug
		directCoding[iterator] = 1;
		if (!Solution_isPossible(directCoding, instance))
		{
			directCoding[iterator] = 0;
		}
		
		ChainedList *Q = chainedList;
		chainedList = chainedList->next;
		free(Q);
	}
	/*
	for (int i = 0; i < instance->nbObject; i++) //debug
	{
		if (directCoding[i])
		{
			switch (heuristicMode)
			{
				case 1:
					printf("Static : directCoding[%d] = 1\n", i);
					break;
				case 2:
					printf("Dynamic : directCoding[%d] = 1\n", i);
					break;
			}
		}
	}
	putchar('\n');
	*/
	return directCoding;
}



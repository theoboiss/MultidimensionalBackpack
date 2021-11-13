#include "../../include/structures/chainedList.h"


int ChainedList_init(ChainedList *chainedList)
{
	if (!chainedList) 
	{
		return 1;
	}
	chainedList->place = -1;
	chainedList->next = NULL;
	return 0;
}

ChainedList* ChainedList_new(void)
{
	ChainedList *chainedList = (ChainedList*) malloc(sizeof(ChainedList));
	if (chainedList) 
	{
		if (ChainedList_init(chainedList))
		{
			free(chainedList);
			chainedList = NULL;
		}
	}
	return chainedList;
}

void ChainedList_del(ChainedList *chainedList)
{
	while (chainedList->next)
	{
		ChainedList *Q = chainedList;
		chainedList = chainedList->next;
		free(Q);
	}
	free(chainedList);
}

///////////////////////////////////////////////////////////

/**
 * Manages the generation of a chainedList
 *
 * Initializes chainedList, then calls the right scheduling strategy to generate it
 */
ChainedList* ChainedList_generationMode(Instance *instance, int mode)
{
	ChainedList *chainedList = NULL;
	if (mode == 1)
	{
		chainedList = ChainedList_randRange(instance);
	}
	else if (mode > 1 && mode <= 9)
	{
		chainedList = ChainedList_similarRangeWays(instance, mode);
	}
	else
	{
		printf("\n-Error- (structures/chainedList.c) ChainedList_generationMode : %d is not an available scheduling method mode.\n", mode);
		exit(EXIT_FAILURE);
	}
	return chainedList;
}


/**
 * Returns 1 if randomNb is in the chained list
 *
 * Browses the chained list and returns 1 if randomNb is inside
 */
int ChainedList_alreadyIn(ChainedList *current, int randomNb)
{
	ChainedList *B = current;
	while (B)
	{
		if (B->place == randomNb)
		{
			return 1;
		}
		B = B->next;
	}
	return 0;
}


/**
 * Generates chainedList with the random strategy
 *
 * Fills chainedList with random objects from the instance. Each one must be inside but never two times
 */
ChainedList* ChainedList_randRange(Instance *instance)
{
	srand(time(NULL));
	int randomNb;
	ChainedList *current = NULL;

	for (int i = 0; i < instance->nbObject; i++) //until we have added nbObject objects to chainedList
	{
		do {randomNb = rand() % 100;}
		while (ChainedList_alreadyIn(current, randomNb));
		
		//add the randomNb-th object to chainedList
		ChainedList *Q = ChainedList_new();
		Q->place = instance->objects[randomNb]->id-1; //the position of an object in the DB  is id-1
		Q->next = current;
		current = Q;
	}
	/*
	ChainedList *backup = current;
	for (int i = 0; i < instance->nbObject; i++) //debug
	{
		printf("Position dans ChainedList : %d\t Valeur dans Objects : %d\n", i, instance->objects[current->place]->value);
		current = current->next;
	}
	printf("\n");
	current = backup;
	*/
	return current;
}


/**
 * Generates chainedList with the chosen strategy according to mode
 *
 * Fills chainedList with objects from the instance using the chosen strategy according to mode. Each one must be inside but never two times
 */
ChainedList* ChainedList_similarRangeWays(Instance *instance, int mode)
{
	ChainedList *current = NULL;

	//creates the transition table which we are about to sort
	Object *table[instance->nbObject];
	for (int i = 0; i < instance->nbObject; i++)
	{
		table[i] = instance->objects[i];
	}

	int criticDimension = 0;
	int ratio = 0;
	int i, j;
	Object *tmp;
	//classical sort of a table
	for (i = 0; i < instance->nbObject; i++)
	{
		for (j = i; j < instance->nbObject; j++)
		{
			
			switch(mode) //change the condition according to schedullingMode
			{
				case 2: //descending
					if (table[j]->value < table[i]->value)
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
					break;
				
				case 3: //crescent
					if (table[j]->value > table[i]->value)
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
					break;
				
				case 4: //ratio value/totalWeight descending
					if (
							(float) table[j]->value / (float) Object_calculateTotalWeight(table[j])
							<
							(float) table[i]->value / (float) Object_calculateTotalWeight(table[i])
						 )
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
					break;
				
				case 5: //ratio value/totalWeight crescent
					if (
							(float) table[j]->value / (float) Object_calculateTotalWeight(table[j])
							>
							(float) table[i]->value / (float) Object_calculateTotalWeight(table[i])
						 )
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
					break;
				
				case 6: //ratio value/weightOfMostCriticalDimension descending
					if (Object_calculateRatioValueCriticDimension(table[j]) < Object_calculateRatioValueCriticDimension(table[i]))
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
                    
					break;
				
				case 7: //ratio value/weightOfMostCriticalDimension crescent
					if (Object_calculateRatioValueCriticDimension(table[j]) > Object_calculateRatioValueCriticDimension(table[i]))
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
                    
					break;
				
				case 8: //ratio value/weightOfMostFullDimension descending
					if (Object_calculateRatioValueCriticDimensionMax(instance, table[j]) < Object_calculateRatioValueCriticDimensionMax(instance, table[i]))
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
                    
					break;

				case 9: //ratio value/weightOfMostFullDimension crescent
					if (Object_calculateRatioValueCriticDimensionMax(instance, table[j]) > Object_calculateRatioValueCriticDimensionMax(instance, table[i]))
					{
						tmp = table[i];
						table[i] = table[j];
						table[j] = tmp;
					}
                    
					break;
				
			}
		}
	}
	/*
	for (int i = 0; i < instance->nbObject; i++) //debug
	{
		printf("%d ", table[i]->id);
	}
	printf("\n");
   */
	for (int i = 0; i < instance->nbObject; i++) //saves the position of the table objects in chainedList
	{
		ChainedList *Q = ChainedList_new();
		Q->place = table[i]->id-1;
		Q->next = current;
		current = Q;
	}
	/*
	ChainedList *backup = current;
	switch(mode) //debug
	{
		case 4: //ratio value/totalWeight descending
			for (int i = 0; i < instance->nbObject; i++)
			{
				printf("Position dans ChainedList : %d\t Ratio : %d%%\n", i, 100 * instance->objects[current->place]->value / Object_calculateTotalWeight(instance->objects[current->place]));
				current = current->next; //WARNING : MOVES CURRENT SO IT CREATES BUGS
			}
			printf("\n");
			break;
		
		case 5:
			for (int i = 0; i < instance->nbObject; i++)
			{
				printf("Position dans ChainedList : %d\t Ratio : %f%%\n", i, 100 * Object_calculateRatioValueCriticDimension(instance->objects[current->place]));
				current = current->next; //WARNING : MOVES CURRENT SO IT CREATES BUGS
			}
			printf("\n");
			break;

		default:
			for (int i = 0; i < instance->nbObject; i++)
			{
				printf("Position dans ChainedList : %d\t Valeur dans Objects : %d\n", i, instance->objects[current->place]->value);
				current = current->next; //WARNING : MOVES CURRENT SO IT CREATES BUGS
			}
			printf("\n");
			break;
	}
	current = backup;
	*/
	return current;
}


/**
 * Returns the ratio of the most critical Dimension
 *
 * Get the biggest weight/value
 */
float Object_calculateRatioValueCriticDimension(Object *object)
{
	float ratio = 0;
    //printf("object->id : %d, object->value : %d\n", object->id, object->value); //debug
	for (int k = 0; k < object->nbDimension; k++)
	{
		//printf("object poids dim : %d\n", object->listValue[k]); //debug
		if ( (float) object->listValue[k] / (float) object->value > ratio)
		{
			ratio = (float) object->listValue[k] / (float) object->value;
		}
		//printf("ratio : %f\n", ratio); //debug
	}
	//printf("\n\n"); //debug
    return ratio;
}


/**
 * Returns the ratio of the most full dimension
 *
 * Get the biggest weight/(value*weightMax)
 */
float Object_calculateRatioValueCriticDimensionMax(Instance *instance, Object *object)
{
	float ratio = 0;
    //printf("object->id : %d, object->value : %d\n", object->id, object->value); //debug
	for (int k = 0; k < object->nbDimension; k++)
	{
		//printf("object poid dim : %d\n DimMaxWeight : %d\n", object->listValue[k], instance->listMaxWeight[k]); //debug
		if ( (float) object->listValue[k] / (float) (object->value * instance->listMaxWeight[k]) > ratio )
		{
			ratio = (float) object->listValue[k] / (float) (object->value * instance->listMaxWeight[k]);
		}
		//printf("ratio : %f\n", ratio); //debug
	}
	//printf("\n\n"); //debug
    return ratio;
}



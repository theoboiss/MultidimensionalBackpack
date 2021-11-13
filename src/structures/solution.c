#include "../../include/structures/solution.h"


int Solution_init(Solution *solution)
{
	if (!solution) return 1;
	solution->instance = NULL;
	solution->status = 1;
	solution->directCoding = NULL;
	solution->listTotalWeight = NULL;
	solution->objective = 0.0;
	solution->heuristicMode = 0;
	solution->schedulingMode = 0;
    solution->timer = 0.0;
	return 0;
}

Solution* Solution_new(void)
{
	Solution *solution = (Solution*) malloc(sizeof(Solution));
	if (solution) 
	{
		if (Solution_init(solution))
		{
			free(solution);
			solution = NULL;
		}
	}
	return solution;
}

void Solution_del(Solution *solution)
{
	solution->instance = NULL;
	free(solution->directCoding);
	free(solution->listTotalWeight);
	free(solution);
}

///////////////////////////////////////////////////////////

/**
 * Manages the generation of a solution
 *
 * Initializes and allocates a new solution, then calls the heuristic and deducts its attributes
 */
Solution* Solution_generation(int heuristicMode, int schedulingMode, Instance* instance)
{
	Solution *solution = Solution_new();
	solution->instance = instance;

	solution->directCoding		= Heuristic_getDirectCoding(heuristicMode, schedulingMode, instance);
	solution->status			= Solution_isPossible(solution->directCoding, instance);
	solution->listTotalWeight 	= Solution_calculateWeights(solution->directCoding, instance);
	solution->objective			= Solution_objective(instance, solution);
	solution->heuristicMode		= heuristicMode;
	solution->schedulingMode	= schedulingMode;

    return solution;
}


/**
 * Gives the solution listTotalWeight
 *
 * Allocates listTotalWeight, computes the total weight using every objects and fill listTotalWeight with
 */
int* Solution_calculateWeights(int *directCoding, Instance *instance)
{
	//allocates listTotalWeight
	int *listTotalWeight = (int*) malloc((size_t) instance->nbDimension*sizeof(int));
	for (int i = 0; i < instance->nbDimension; i++)
	{
		listTotalWeight[i] = 0;
	}

	//computes the occupation of the bag for each dimension
	for (int i = 0; i < instance->nbObject; i++)
	{
		if (directCoding[i]) //if the object is taken
		{
			for (int j = 0; j < instance->nbDimension; j++) 
			{
				listTotalWeight[j] += instance->objects[i]->listValue[j];
			}
		}
	}
	return listTotalWeight;
}


/**
 * Updates the listTotalWeight of the solution without allocation
 *
 * Resets listTotalWeight, computes the total weight using every objects and fill listTotalWeight with
 */
void Solution_updateWeights(Solution *solution)
{
	//init
	for (int i = 0; i < solution->instance->nbDimension; i++)
	{
		solution->listTotalWeight[i] = 0;
	}

	//calculates the occupation of the bag for each dimension
	for (int i = 0; i < solution->instance->nbObject; i++)
	{
		if (solution->directCoding[i])
		{
			for (int j = 0; j < solution->instance->nbDimension; j++) 
			{
				solution->listTotalWeight[j] += solution->instance->objects[i]->listValue[j];
			}
		}
	}
}


/**
 * Returns 1 if the solution is possible, 0 otherwise
 *
 * Calls Solution_calculateWeights and uses directCoding to determine if listMaxWeight is respected
 */
int Solution_isPossible(int *directCoding, Instance *instance)
{
	int *listTotalWeight = Solution_calculateWeights(directCoding, instance); //Solution_calculateWeights allocates so listTotalWeight will have to be freed
	for (int i = 0; i < instance->nbDimension; i++) //tests every dimension
	{
		if (listTotalWeight[i] > instance->listMaxWeight[i]) //if at least one object has too large a dimension
		{
			free(listTotalWeight);
			return 0;
		}
	}
	free(listTotalWeight);
	return 1;
}


/**
 * Returns the objective score
 *
 * Multiplies the average ratio of the occupation of the bag to its total value, then returns the score as an integer
 */
int Solution_objective(Instance *instance, Solution *solution)
{
	float objective = 0.0;
	//calculates the ratio of the occupation of the bag for each dimension
	for (int i = 0; i < solution->instance->nbDimension; i++)
	{
		objective += (float) solution->listTotalWeight[i] / (float) solution->instance->listMaxWeight[i];
	}
	//calculates an average
	objective /= (float) solution->instance->nbDimension;
	objective *= (float) Solution_getValue(instance->nbObject, instance->objects, solution->directCoding);
	return (int) objective;
}


/**
 * Returns the total value of the bag
 *
 * Sums the value of every object that has to be taken according to directCoding
 */
int Solution_getValue(int nbObject, Object** objects, int* directCoding)
{
	int value = 0;
	for (int i = 0; i < nbObject; i++) //for each object
	{
        if (directCoding[i]) //if the object is taken
        {
		    value += (float) objects[i]->value;
        }
	}
	return value;
}


/**
 * Prints the properties of the solution
 *
 * For each instance, prints the properties of every object in the bag, then the properties of the solution
 */
void Solution_print(Solution* solution)
{
	printf("\n- Instance %d -\n", solution->instance->id);
	for (int i = 0; i < solution->instance->nbObject; i++)
	{
		if (solution->directCoding[i]) //if the object is taken
		{
			printf("\nObject %d\n\tValue = %d\n", solution->instance->objects[i]->id, solution->instance->objects[i]->value);
			printf("\tDimensions :");
			for (int j = 0; j < solution->instance->nbDimension; j++) 
			{
				printf(" %d", solution->instance->objects[i]->listValue[j]); //print value of the taken object in each dimension
			}
			putchar('\n'); //to the line
		}
	}
	printf("\nTotal of weights :");
	for (int j = 0; j < solution->instance->nbDimension; j++)
	{
		printf(" %d", solution->listTotalWeight[j]);
	}
	printf("\nObjective = %d\n", solution->objective * 100);
	if (solution->objective)
	{
		printf("\nFeasible solution.\n\n");
	}
	else
	{
		printf("\nSolution not feasible.\n\n");
	}
}



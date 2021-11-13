#include "../../include/structures/instance.h"


int Instance_init(Instance *instance)
{
	if (!instance) return 1;
	instance->id = 0;
	instance->nbDimension = 0;
	instance->listMaxWeight = NULL;
	instance->nbObject = 0;
	instance->objects = NULL;
return 0;
}

Instance* Instance_new(void)
{
	Instance *instance = (Instance*) malloc(sizeof(Instance));
	if (instance) 
	{
		if (Instance_init(instance))
		{
			free(instance);
			instance = NULL;
		}
	}
	return instance;
}

void Instance_del(Instance *instance)
{
	for (int i = 0; i < instance->nbObject; i++)
	{
		Object_del(instance->objects[i]);
	}
	free(instance->listMaxWeight);
	free(instance->objects);
	free(instance);
}

///////////////////////////////////////////////////////////

/**
 * Gives the instance nbObject and nbDimension.
 *
 * Reads in the file nbObject and nbDimension of the current instance. For debugging or simple tests, it can go to the next instance after each reading.
 */
void Instance_getParameters(FILE *file, Instance *instance)
{
	if (!file || !instance)
	{
		printf("\n-Error- (structures/instance.c) Instance_getParameters : file or instance is missing.\n");
		exit(EXIT_FAILURE);
	}
	
	//gets nbObject and nbDimension
	fscanf(file, "%d", &instance->nbObject);
	fscanf(file, "%d", &instance->nbDimension);
}


/**
 * Gives the instance listMaxWeight
 *
 * Allocates listMaxWeight and fill it with what we read in the data file
 */
void Instance_getMaxWeight(FILE *file, Instance *instance)
{
	if (!file || !instance)
	{
		printf("\n-Error- (structures/instance.c) Instance_getParameters : file or instance is missing.\n");
		exit(EXIT_FAILURE);
	}
  
	//allocates the list of max weights
	if (!instance->listMaxWeight)
	{
		instance->listMaxWeight = (int*) malloc((size_t) instance->nbDimension*sizeof(int));
	}
	//gets the max weight for each dimension
	for (int i = 0; i < instance->nbDimension; i++)
	{
		fscanf(file, "%d", &instance->listMaxWeight[i]);
	}
}


/**
 * Returns a copy of the instance
 *
 * Creates a new instance by allocating it and gives it all the old instance's attributes
 */
Instance* Instance_copy(Instance *instance)
{
	//init instance
	Instance* instanceCopy = Instance_new();

	//copies attributes
	instanceCopy->nbObject = instance->nbObject;
	instanceCopy->nbDimension = instance->nbDimension;
	//allocates listMaxWeight then copies
	instanceCopy->listMaxWeight = (int*) malloc(instanceCopy->nbDimension * sizeof(int));
	for (int i = 0; i < instanceCopy->nbDimension; i++)
	{
		instanceCopy->listMaxWeight[i] = instance->listMaxWeight[i];
	}
	//allocates objects then copies
	instanceCopy->objects = (Object**) malloc((size_t) instanceCopy->nbObject * sizeof(Object*));
    for (int i = 0; i < instanceCopy->nbObject; i++)
    {
		//init object
		instanceCopy->objects[i] = Object_new();
		instanceCopy->objects[i]->nbDimension = instance->objects[i]->nbDimension;
		instanceCopy->objects[i]->id = instance->objects[i]->id;
        instanceCopy->objects[i]->value = instance->objects[i]->value;
		//allocates listValue of object then copies
		instanceCopy->objects[i]->listValue = (int*) malloc((size_t) (instanceCopy->nbDimension)*sizeof(int));
        for (int j = 0; j < instanceCopy->objects[i]->nbDimension; j++)
		{
			instanceCopy->objects[i]->listValue[j] = instance->objects[i]->listValue[j];
		}
    }
	return instanceCopy;
}
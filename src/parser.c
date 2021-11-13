#include "../include/parser.h"


/**
 * Parses the data.
 *
 * Creates a database and store a maximum of informations about the instances from the data file into.
 */
InstanceDB* parse(FILE *file)
{
	//new DB
	InstanceDB *DB = InstanceDB_generate(file);
	//printf("Number of instance(s) in the database = %d.\n\n", DB->nbInstance); //debug

	for (int i = 0; i < DB->nbInstance; i++) //fill DB with instances
	{
		DB->listInstance[i] = Instance_new();
		DB->listInstance[i]->id = i+1;
		Instance_getParameters(file, DB->listInstance[i]);

		File_skipLines(file, 2); //skips the suggested solutions

		Parser_giveObjectsToInstance(file, DB->listInstance[i]);

		Instance_getMaxWeight(file, DB->listInstance[i]);

		//printf("\tInstance%d\tnbObject=%d\tnbDimension=%d\n", i+1, DB->listInstance[i]->nbObject, DB->listInstance[i]->nbDimension); //debug
	}
	return DB;
}


/**
 * Fill the instance with objects.
 *
 * Stores into the instance every informations about the objects from the data file.
 */
void Parser_giveObjectsToInstance(FILE *file, Instance *instance)
{
  if (!instance)
	{
		printf("\n-Error- (parser.c) Parser_giveObjectstoInstance : instance is missing.\n");
		exit(EXIT_FAILURE);
	}
	
	//fill the instancies with objects
	instance->objects = (Object**) malloc((size_t) (instance->nbObject)*sizeof(Object*));

	for (int i = 0; i < instance->nbObject; i++) //give them their values
	{
		instance->objects[i] = Object_new();
		instance->objects[i]->id = i+1;
		fscanf(file, "%d", &instance->objects[i]->value);
    	instance->objects[i]->nbDimension = instance->nbDimension;
    	//printf("%d\t", instance->objects[i]->value); //debug
	}

	for (int i = 0; i < instance->nbObject; i++)
	{
		instance->objects[i]->listValue = (int*) malloc((size_t) (instance->nbDimension)*sizeof(int));
	}

	//gives each object its dimensions
	for (int j = 0; j < instance->nbDimension; j++)
	{
		for (int i = 0; i < instance->nbObject; i++)
		{
    		fscanf(file, "%d", &instance->objects[i]->listValue[j]);
		}
	}
}



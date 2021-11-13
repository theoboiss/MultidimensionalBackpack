#include "../../include/structures/object.h"


int Object_init(Object *object)
{
	if (!object) return 1;
		object->id = 0;
	object->value = 0;
	object->nbDimension = 0;
	object->listValue = NULL;
	return 0;
}

Object* Object_new(void)
{
	Object *object = (Object*) malloc(sizeof(Object));
	if (object)
	{
		if (Object_init(object))
		{
			free(object);
			object = NULL;
		}
	}
	return object;
}

void Object_del(Object *object)
{
	free(object->listValue);
	free(object);
}

///////////////////////////////////////////////////////////

/**
 * Returns total wieght of the object
 *
 * Add up all the weights of an object, independently of a dimension, then returns it
 */
int Object_calculateTotalWeight(Object *object)
{
	int weight = 0;
	//printf("%d ", object->nbDimension); //debug
	for (int i = 0; i < object->nbDimension; i++)
	{
		weight += object->listValue[i];
	}
	return weight;
}



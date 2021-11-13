#define INSTANCE_H

#ifndef OBJECT_H
#include "object.h"
#endif

#include <stdio.h>
#include <stdlib.h>


typedef struct instance
{
	int id;
	int nbDimension;
	int *listMaxWeight;
	int nbObject;
	Object **objects;
} Instance;


int Instance_init(Instance *instance);

Instance* Instance_new(void);

void Instance_del(Instance *instance);

////////////////////////////////////////////////

void Instance_getParameters(FILE *file, Instance *instance);

void Instance_getMaxWeight(FILE *file, Instance *instance);

Instance* Instance_copy(Instance *instance);


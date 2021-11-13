#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>


typedef struct
{
	int id;
	int value;
	int nbDimension;
	int *listValue;
} Object;


int Object_init(Object *object);

Object* Object_new(void);

void Object_del(Object *object);

int Object_calculateTotalWeight(Object *object);


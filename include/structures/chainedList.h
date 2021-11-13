#define CHAINEDlIST_H

#ifndef INSTANCE_H
#include "instance.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct __ChainedList__
{
	int place;
	struct __ChainedList__ *next;
} ChainedList;

int ChainedList_init(ChainedList *chainedList);

ChainedList* ChainedList_new(void);

void ChainedList_del(ChainedList *chainedList);

////////////////////////////////////////////////

ChainedList* ChainedList_generationMode(Instance *instance, int mode);

int ChainedList_InObjects(ChainedList *current, int randomNb);

ChainedList* ChainedList_randRange(Instance *instance);

ChainedList* ChainedList_similarRangeWays(Instance *instance, int mode);

float Object_calculateRatioValueCriticDimension(Object *object);

float Object_calculateRatioValueCriticDimensionMax(Instance *instance, Object *object);


#define SOLUTION_H

#ifndef INSTANCE_H
#include "instance.h"
#endif

#include "../heuristic.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct
{
	Instance *instance;
	int status;
	int *directCoding;
	int *listTotalWeight;
	int objective;
    int schedulingMode;
    int heuristicMode;
    float timer;
} Solution;


int Solution_init(Solution *solution);

Solution* Solution_new(void);

void Solution_del(Solution *solution);

/////////////////////////////////////////////////

Solution* Solution_generation(int heuristicMode, int schedulingMode, Instance* instance);

int* Solution_calculateWeights(int *directCoding, Instance *instance);

void Solution_updateWeights(Solution *solution);

int Solution_isPossible(int *directCoding, Instance *instance);

int Solution_objective(Instance *instance, Solution *solution);

int Solution_getValue(int nbObject, Object** objects, int* directCoding);

void Solution_print(Solution* solution);


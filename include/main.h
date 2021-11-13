#include "structures/solution.h"
#include "filesystem.h"
#include "parser.h"
#include "metaheuristic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


float TimerOver(clock_t timerStarted);

void getModes(int* heuristicMode, int* schedulingMode);

Solution** sorter (InstanceDB *DB, Solution** listSolutions[], int *nbSolutionsPerInstance, int nbSolutionsExpected);

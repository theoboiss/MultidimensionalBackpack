#define HEURISTIC_H

#ifndef INSTANCE_H
#include "structures/instance.h"
#endif

#ifndef CHAINEDlIST_H
#include "structures/chainedList.h"
#endif

#ifndef SOLUTION_H
#include "structures/solution.h"
#endif


int* Heuristic_getDirectCoding(int heuristicMode, int schedulingMode, Instance *instance);

int* Heuristic(int heuristicMode, ChainedList *chainedList, Instance *instance);


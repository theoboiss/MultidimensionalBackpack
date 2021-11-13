#define METAHEURISTIC_H

#ifndef INSTANCE_H
#include "structures/instance.h"
#endif

#ifndef CHAINEDlIST_H
#include "structures/chainedList.h"
#endif

#ifndef SOLUTION_H
#include "structures/solution.h"
#endif

#include <time.h>

void Metaheuristic_localResearch(Solution* solution);
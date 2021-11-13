#include <stdio.h>
#include <stdlib.h>

#include "structures/solution.h"
#include "filesystem.h"
#include "parser.h"
#include "metaheuristic.h"

void Test_dataBase();

void Test_Object(Instance *Instance, Object *object);

Solution* Test_solution(Instance *instance);

ChainedList* Test_chainedList(Instance *instance);

void Test_heuristic(Solution *solution, ChainedList *chainedList, Instance *instance);

void Test_metaheuristic(Solution *solution);
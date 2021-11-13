#define FILESYSTEM_H

#ifndef INSTANCEdb_H
#include "structures/instanceDB.h"
#endif

#ifndef SOLUTION_H
#include "structures/solution.h"
#endif

#include <stdio.h>
#include <stdlib.h>


void File_skipLines(FILE *file, int nbOfLinesToSkip);

FILE* File_open(char *fileName, char *mode);

FILE* File_writeSolutions(FILE *file, Solution *solution);

void File_getStringsOfModes(char** heuristicStr, int heuristicMode, char** schedulingStr, int schedulingMode);

void File_getStringsOfModesForSpreadsheet(char** heuristicStr, int heuristicMode, char** schedulingStr, int schedulingMode);

FILE* File_writeInstances(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, float elapsedTime);

FILE* File_writeInstancesForSpreadsheet(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, float elapsedTime);

FILE* File_writeMetaheuristics(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, int metaheuristicMode, float elapsedTime);

FILE* File_writeMetaheuristicsForSpreadsheet(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, int metaheuristicMode, float elapsedTime);
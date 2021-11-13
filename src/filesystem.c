#define _GNU_SOURCE //for getline
#include "../include/filesystem.h"


/**
 * Skip lines.
 *
 * Load nbOfLinesToSkip lines using getline() and forget them so it can skip lines.
 */
void File_skipLines(FILE *file, int nbOfLinesToSkip)
{
	//init
	char *line_buf = NULL;
 	size_t line_buf_size = 0;
	
	//load lines to skip them
	for (int i = 0; i < nbOfLinesToSkip; i++)
	{
		int line_size = getline(&line_buf, &line_buf_size, file);
	}
	free(line_buf);
}


/**
 * Opens a file securely.
 *
 * Opens a file using the mode entered. Return the file or print an error and return NULL. 
 */
FILE* File_open(char *fileName, char *mode)
{
	FILE *file = fopen(fileName, mode);
	if (!file)
	{
		printf("\n-Error- (filesystem.c) File_open : Cannot open the file : %s.\n", fileName);
		exit(EXIT_FAILURE);
	}
	//printf("%s opened.\n", fileName); //debug
	return file;
}


/**
 * Opens a file securely and write some solutions' results informations into.
 *
 * Opens a file using File_open() and writes stats about the solutions.
 */
FILE* File_writeSolutions(FILE *file, Solution *solution)
{
	//write
	if (ftell(file))
	{
		fprintf(file, "\n");
	}
	if (solution->heuristicMode)
	{
		switch(solution->heuristicMode)
		{
			case 1:
				fprintf(file, "Static\n");
				break;
			case 2:
				fprintf(file, "Dynamic\n");
				break;
		}
	}
	if (solution->schedulingMode) fprintf(file, "%d\n", solution->schedulingMode);
	if (solution->heuristicMode || solution->schedulingMode) fprintf(file, "\n");

	fprintf(file, "Direct coding\n");
  
	for (int i = 0; i < solution->instance->nbObject; i++)
	{
		fprintf(file, "%d\t", solution->directCoding[i]);
	}
  
	fprintf(file, "\n\nTotal weights\n");

	for (int i = 0; i < solution->instance->nbDimension; i++)
	{
		fprintf(file, "%d\t", solution->listTotalWeight[i]);
	}

	fprintf(file, "\n\nStatus\n%d\n", solution->status);
	
	fprintf(file, "\nObjective\n%d\n\n", solution->objective);
	//printf("\nsolution objective %d\n", solution->objective); //debug

	return file;
}


/**
 * Gives the strings corresponding to their modes (.txt version)
 *
 * Modify heuristicStr to correspond to heuristicMode and schedulingStr to correspond to schedulingMode (.txt version)
 */
void File_getStringsOfModes(char** heuristicStr, int heuristicMode, char** schedulingStr, int schedulingMode)
{
	switch (heuristicMode) {
		case 1:
			*heuristicStr = "Static ";
			break;
		case 2:
			*heuristicStr = "Dynamic";
			break;
	}
	switch (schedulingMode) {
		case 1:
			*schedulingStr = "random                                              ";
			break;
		case 2:
			*schedulingStr = "descending                                          ";
			break;
		case 3:
			*schedulingStr = "crescent                                            ";
			break;
		case 4:
			*schedulingStr = "ratio value/totalWeight descending                  ";
			break;
		case 5:
			*schedulingStr = "ratio value/totalWeight crescent                    ";
			break;
		case 6:
			*schedulingStr = "ratio value/weightOfMostCriticalDimension descending";
			break;
		case 7:
			*schedulingStr = "ratio value/weightOfMostCriticalDimension crescent  ";
			break;
		case 8:
			*schedulingStr = "ratio value/weightOfMostFullDimension descending    ";
			break;
		case 9:
			*schedulingStr = "ratio value/weightOfMostFullDimension crescent      ";
			break;
	}
}


/**
 * Gives the strings corresponding to their modes (.csv version)
 *
 * Modify heuristicStr to correspond to heuristicMode and schedulingStr to correspond to schedulingMode (.csv version)
 */
void File_getStringsOfModesForSpreadsheet(char** heuristicStr, int heuristicMode, char** schedulingStr, int schedulingMode)
{
	switch (heuristicMode) {
		case 1:
			*heuristicStr = "Static";
			break;
		case 2:
			*heuristicStr = "Dynamic";
			break;
	}
	switch (schedulingMode) {
		case 1:
			*schedulingStr = "random";
			break;
		case 2:
			*schedulingStr = "descending";
			break;
		case 3:
			*schedulingStr = "crescent";
			break;
		case 4:
			*schedulingStr = "ratio value/totalWeight descending";
			break;
		case 5:
			*schedulingStr = "ratio value/totalWeight crescent";
			break;
		case 6:
			*schedulingStr = "ratio value/weightOfMostCriticalDimension descending";
			break;
		case 7:
			*schedulingStr = "ratio value/weightOfMostCriticalDimension crescent";
			break;
		case 8:
			*schedulingStr = "ratio value/weightOfMostFullDimension descending";
			break;
		case 9:
			*schedulingStr = "ratio value/weightOfMostFullDimension crescent";
			break;
	}
}


/**
 * Opens a file securely and write some parsed instances' results informations into (.txt version)
 *
 * Opens a file using File_open() and writes stats about the parser and instances (.txt version)
 */
FILE* File_writeInstances(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, float elapsedTime)
{
	//write
	if (!ftell(file)) //writes the legend if the file is empty
	{
		fprintf(file, "Instance  \t\tObjective\t\t");
		if (schedulingMode) fprintf(file, "Scheduling\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
		if (heuristicMode) fprintf(file, "Heuristic\t\t");
		fprintf(file, "Duration (in s)\n");
	}

	char *heuristicStr = NULL;
    char *schedulingStr = NULL;
	File_getStringsOfModes(&heuristicStr, heuristicMode, &schedulingStr, schedulingMode);

	fprintf(file, "instance%d \t\t%d\t\t\t", solution->instance->id, solution->objective);
	if (schedulingMode) fprintf(file, "%s\t\t\t", schedulingStr);
	if (heuristicMode) fprintf(file, "%s\t\t\t", heuristicStr);
	fprintf(file, "%f\n", elapsedTime);
	return file;
}


/**
 * Opens a csv file securely and write some parsed instances' results informations into (.csv version)
 *
 * Opens a csv file using File_open() and writes stats about the parser and instances.
 */
FILE* File_writeInstancesForSpreadsheet(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, float elapsedTime)
{
	//write
	if (!ftell(file)) //writes the legend if the file is empty
	{
		fprintf(file, "Instance\tObjective\t");
		if (schedulingMode) fprintf(file, "Scheduling\t");
		if (heuristicMode) fprintf(file, "Heuristic\t");
		fprintf(file, "Duration (in s)\n");
	}

	char *heuristicStr = NULL;
    char *schedulingStr = NULL;
	File_getStringsOfModesForSpreadsheet(&heuristicStr, heuristicMode, &schedulingStr, schedulingMode);

	fprintf(file, "instance%d\t%d\t", solution->instance->id, solution->objective);
	if (schedulingMode) fprintf(file, "%s\t", schedulingStr);
	if (heuristicMode) fprintf(file, "%s\t", heuristicStr);
	fprintf(file, "%f\n", elapsedTime);
	return file;
}


/**
 * Opens a csv file securely and writes the informations of the best solution found (.txt version)
 *
 * Opens a csv file using File_open() and writes stats about the parser and instances (.txt version)
 */
FILE* File_writeMetaheuristics(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, int metaheuristicMode, float elapsedTime)
{
	//write
	if (!ftell(file)) //writes the legend if the file is empty
	{
		fprintf(file, "Instance  \t\tObjective\t\tScheduling\t\t\t\t\t\t\t\t\t\t\t\t\t\tHeuristic\t\tMetaheuristic\t\tDuration (in s)\n");
	}

	char *heuristicStr = NULL;
    char *schedulingStr = NULL;
    char *metaheuristicStr = NULL;
	File_getStringsOfModes(&heuristicStr, heuristicMode, &schedulingStr, schedulingMode);
    switch (metaheuristicMode) {
        case 1:
            metaheuristicStr = "    Local Research";
            break;
    }

	fprintf(file, "instance%d \t\t%d\t\t\t%s\t\t\t%s\t\t%s\t\t%f\n", solution->instance->id, solution->objective,schedulingStr, heuristicStr, metaheuristicStr, elapsedTime);
	return file;
}


/**
 * Opens a csv file securely and writes the informations of the best solution found (.csv version)
 *
 * Opens a csv file using File_open() and writes stats about the parser and instances (.csv version)
 */
FILE* File_writeMetaheuristicsForSpreadsheet(FILE *file, Solution *solution, int schedulingMode, int heuristicMode, int metaheuristicMode, float elapsedTime)
{
	//write
	if (!ftell(file)) //writes the legend if the file is empty
	{
		fprintf(file, "Instance\tObjective\tScheduling\tHeuristic\tMetaheuristic\tDuration (in s)\n");
	}

	char *heuristicStr = NULL;
    char *schedulingStr = NULL;
    char *metaheuristicStr = NULL;
	File_getStringsOfModesForSpreadsheet(&heuristicStr, heuristicMode, &schedulingStr, schedulingMode);

    switch (metaheuristicMode) {
        case 1:
            metaheuristicStr = "Local Research";
            break;
    }

	fprintf(file, "instance%d\t%d\t%s\t%s\t%s\t%f\n", solution->instance->id, solution->objective,schedulingStr, heuristicStr, metaheuristicStr, elapsedTime);
	return file;
}



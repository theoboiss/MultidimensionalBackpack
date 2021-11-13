#include "../include/tests.h"


Solution* Test_solution(Instance *instance)
{
	//init solution_test, then uses instance and copy exemple
	int exemple[100] = {0,1,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0};
	Solution *solution_test = Solution_new();
	solution_test->instance = instance;
	solution_test->instance->nbObject = 100;
	solution_test->directCoding = malloc(100*sizeof(int));
	for (int i = 0; i < solution_test->instance->nbObject; i++)
	{
		solution_test->directCoding[i] = exemple[i];
	}

	//test Solution_calculateWeights
	solution_test->listTotalWeight = Solution_calculateWeights(solution_test->directCoding, solution_test->instance);
	/*for (int i = 0; i < solution_test->instance->nbDimension; i++)
	{
    	printf("Max weight : %d\nTotal weight of the solution : %d\n\n", solution_test->instance->listMaxWeight[i], solution_test->listTotalWeight[i]);
	}*/

	//test Solution_isPossible & objective
	solution_test->status = Solution_isPossible(solution_test->directCoding, solution_test->instance);
	solution_test->objective = Solution_objective(solution_test->instance, solution_test);
	/*printf("\nSatisfaction : %0.02f%%\nSolution is possible : %d\n", solution_test->objective*100, solution_test->status);*/

	//test Solution_print
	//Solution_print(solution_test);

	//test File_writeSolutions
	char output[23];
	sprintf(output, "solutions/solution.%d.txt", solution_test->instance->id);
	//File_writeSolutions(output, solution_test);

	return solution_test;
	//Solution_del(solution_test);
}


void Test_dataBase()
{
	//test init and del
	Object* object = Object_new();
	Object_del(object);
	Instance* instance = Instance_new();
	Instance_del(instance);
	InstanceDB* instanceDB = InstanceDB_new();
	InstanceDB_del(instanceDB);
	Solution* solution = Solution_new();
	Solution_del(solution);

	//cannot use sleep for making the program last longer because it stops the timer
	//for (long i = 0; i < (long) 1000000000; i++) {} //1,000,000,000 lasts around 5s (measured by myself but can depends on the connection)
}


void Test_Object(Instance *instance, Object *object)
{
	//int totalWeight = Object_calculateTotalWeight(object);
	//printf("%d", totalWeight); //debug
    //float ratio = Object_calculateRatioValueCriticDimension(object); //debug
	//float ratio = Object_calculateRatioValueCriticDimensionMax(instance, object); //debug
}


ChainedList* Test_chainedList(Instance *instance)
{
	/*
	1 = random
	2 = descending
	3 = crescent
	4 = ratio value/totalWeight descending
	5 = ratio value/totalWeight crescent
	6 = ratio value/weightOfMostCriticalDimension descending
	7 = ratio value/weightOfMostCriticalDimension crescent
	8 = ratio value/weightOfMostFullDimension descending
	9 = ratio value/weightOfMostFullDimension crescent
	*/
	int mode = 5;
	ChainedList *chainedList = ChainedList_generationMode(instance, mode);

	return chainedList;
}


void Test_heuristic(Solution *solution, ChainedList *chainedList, Instance *instance)
{
	if (solution->directCoding)
	{
		free(solution->directCoding);
	}
	
	solution->directCoding = Heuristic_getDirectCoding(1, 4, instance);
	/*
	for (int i = 0; i<instance->nbObject; i++)
	{
		printf("%d ", solution->directCoding[i]);
	}*/
	solution->status = Solution_isPossible(solution->directCoding, solution->instance);
	
	if (solution->listTotalWeight)
	{
		free(solution->listTotalWeight);
	}
	solution->listTotalWeight = Solution_calculateWeights(solution->directCoding, instance);

	solution->objective = Solution_objective(solution->instance, solution);

	char output[23];
	sprintf(output, "solutions/solution.%d.txt", solution->instance->id);
	FILE *outputFile = File_open(output, "a"); //creates the file if it does not exist and writes at the end

	outputFile = File_writeSolutions(outputFile, solution);
	fclose(outputFile);
}


void Test_metaheuristic(Solution *solution)
{
    Metaheuristic_localResearch(solution);
}



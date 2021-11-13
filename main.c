#include "include/main.h"


int main(void)
{
	//must be kept up-to-date
	int nbHeuristicMode = 2; //number of available heuristic modes
	int nbSchedulingMode = 9; //number of available scheduling modes
    int nbMetaheuristic = 1; //number of available metaheuristic modes
	char *dataFileName = "_mknapcb1_res.txt";
	char *outputStatsFileName = "outputs/sortie.txt";
	char *outputStatsCSVFileName = "outputs/sortie.csv";
    char *heuristicsFileName = "outputs/heuristic.txt";
	char *heuristicsCSVFileName = "outputs/heuristic.csv";
    char *metaheuristicFileName = "outputs/metaheuristic.txt";
    char *metaheuristicCSVFileName = "outputs/metaheuristic.csv";


	////////// init //////////

	int heuristicMode = 0;
	int schedulingMode = 0;
    int metaheuristicMode = 0;
	int nbSolutionsPerInstance = nbHeuristicMode * nbSchedulingMode;
	int nbSolutionsExpected = -1;
	printf("How many solutions are expected per instance ? (all = -1)\n");
	scanf("%d", &nbSolutionsExpected);
    if (nbSolutionsExpected > nbSolutionsPerInstance || nbSolutionsExpected < (-1))
    {
        do {
            printf("Number of solutions must be more than -1\n");
            scanf("%d", &nbSolutionsExpected);
        } while (nbSolutionsExpected > nbSolutionsPerInstance || nbSolutionsExpected < (-1));
    }
	/* //can be activated if a new metaheuristic is added
    printf("How many metaheuristic are expected per instance ?\n");
	scanf("%d", &nbMetaheuristic);
    if (nbMetaheuristic>1 || nbMetaheuristic<1)
    {
        do {
            printf("Number of metaheuristic must be between 1 and 1 included\n");
            scanf("%d", &nbMetaheuristic);
        } while(nbMetaheuristic>1 || nbMetaheuristic<1);
    }
	*/
	clock_t timerStarted = clock(); //timer starts


	////////// parser //////////

	FILE *data = File_open(dataFileName, "r");
	InstanceDB *DB = parse(data);
	float timerParser = TimerOver(timerStarted);
	fclose(data);


	////////// resolution //////////

	Solution **listSolutions[DB->nbInstance]; //will store and classify the solutions 
	for (int i = 0; i < DB->nbInstance; i++)
	{
		Solution **listSolutionsPerInstance = (Solution**) malloc(nbSolutionsPerInstance*sizeof(Solution*));
		int counterPerInstance = 0;
		for (int j = 1; j <= nbSchedulingMode; j++)
		{
			for (int k = 1; k <= nbHeuristicMode; k++)
			{
				//generates all the interesting solutions
                clock_t timerSolutionStarted = clock();
				listSolutionsPerInstance[counterPerInstance] = Solution_generation(k, j, DB->listInstance[i]);
                listSolutionsPerInstance[counterPerInstance++]->timer = TimerOver(timerSolutionStarted);
			}
		}
		listSolutions[i] = listSolutionsPerInstance;
	}


	////////// sorter //////////

    *listSolutions = sorter(DB, listSolutions, &nbSolutionsPerInstance, nbSolutionsExpected);


    ////////// metaheuristic /////////

    Solution **listSolutionsMetaheuristic[DB->nbInstance];

    for (int i = 0; i < DB->nbInstance; i++)
    {

        Solution **listSolutionsPerMetaheuristic = (Solution**) malloc(nbMetaheuristic*sizeof(Solution*));

        for (int k = 0; k < nbMetaheuristic; k++)
        {
			clock_t timerMetaSolutionStarted = clock();
            listSolutionsPerMetaheuristic[k] = Solution_new();
            *listSolutionsPerMetaheuristic[k] = *listSolutions[i][0];

            switch (k) {
                case 0 :
                    Metaheuristic_localResearch(listSolutionsPerMetaheuristic[k]);
					listSolutionsPerMetaheuristic[k]->timer += TimerOver(timerMetaSolutionStarted);
                    break;
            }

        }
        listSolutionsMetaheuristic[i] = listSolutionsPerMetaheuristic;
        //printf("objective listSolutions : %d objective meta : %d\n", listSolutions[i][0]->objective, listSolutionsMetaheuristic[i][0]->objective);//debug

    }


	////////// conclusion //////////

	FILE *outputStats = File_open(outputStatsFileName, "a");
	FILE *outputStatsCSV = File_open(outputStatsCSVFileName, "a");
    FILE *heuristic = File_open(heuristicsFileName, "a");
	FILE *heuristicCSV = File_open(heuristicsCSVFileName, "a");
    FILE *metaheuristic = File_open(metaheuristicFileName, "a");
    FILE *metaheuristicCSV = File_open(metaheuristicCSVFileName, "a");
	for (int i = 0; i < DB->nbInstance; i++)
	{
		for (int j = 0; j < nbSolutionsPerInstance; j++)
		{
			//writes in sortie.txt
			File_writeInstances(outputStats, listSolutions[i][j], 0, 0, listSolutions[i][j]->timer);

			File_writeInstancesForSpreadsheet(outputStatsCSV, listSolutions[i][j], 0, 0, listSolutions[i][j]->timer);


            //writes in heuristic.txt
            File_writeInstances(heuristic, listSolutions[i][j], listSolutions[i][j]->schedulingMode, listSolutions[i][j]->heuristicMode, listSolutions[i][j]->timer);

			File_writeInstancesForSpreadsheet(heuristicCSV, listSolutions[i][j], listSolutions[i][j]->schedulingMode, listSolutions[i][j]->heuristicMode, listSolutions[i][j]->timer);


			//writes in solutions/
			char outputSolutions[33];
			sprintf(outputSolutions, "outputs/solutions/solution.%d.txt", listSolutions[i][j]->instance->id);

			FILE *outputSolutionsFile = File_open(outputSolutions, "a"); //creates the file if it does not exist and writes at the end
			outputSolutionsFile = File_writeSolutions(outputSolutionsFile, listSolutions[i][j]);


            if (!j)//pour les métaheuristiques
            {
                for (int k = 0; k < nbMetaheuristic; k++)
                {
                    File_writeMetaheuristics(metaheuristic, listSolutionsMetaheuristic[i][k], listSolutionsMetaheuristic[i][k]->schedulingMode, listSolutionsMetaheuristic[i][k]->heuristicMode, k+1, listSolutionsMetaheuristic[i][k]->timer);

                    File_writeMetaheuristicsForSpreadsheet(metaheuristicCSV, listSolutionsMetaheuristic[i][k], listSolutionsMetaheuristic[i][k]->schedulingMode, listSolutionsMetaheuristic[i][k]->heuristicMode, k+1, listSolutionsMetaheuristic[i][k]->timer);
                
                    //printf("objective listSolutions : %d objective meta : %d\n", listSolutions[i][0]->objective, listSolutionsMetaheuristic[i][k]->objective);//debug
                    free(listSolutionsMetaheuristic[i][k]);
                }
            }
			fclose(outputSolutionsFile);
			Solution_del(listSolutions[i][j]);
		}
		free(listSolutions[i]);
        free(listSolutionsMetaheuristic[i]);
	}
	fclose(outputStats);
	fclose(outputStatsCSV);
    fclose(heuristic);
    fclose(heuristicCSV);
    fclose(metaheuristic);
    fclose(metaheuristicCSV);

	//timer ends
	float duration = TimerOver(timerStarted);
	printf("Parser duration : %fs\nTotal duration : %fs\n", timerParser, duration);

	InstanceDB_del(DB);
	return 0;
}



float TimerOver(clock_t timerStarted)
{
	float duration = ((float) clock() - (float) timerStarted) /CLOCKS_PER_SEC;
	duration *= 2; //it looks like repl.it's CLOCKS_PER_SEC is twice too small
	//printf("duration : %fs\n\n", duration); //debug
	return duration;
}


void getModes(int* heuristicMode, int* schedulingMode)
{
	printf("Heuristic modes available\n\t1 = Static\n\t2 = Dynamic\n");

	printf("\nScheduling modes available\n\t1 = random\n\t2 = descending\n\t3 = crescent\n\t4 = ratio value/totalWeight descending\n\t5 = ratio value/totalWeight crescent\n\t6 = ratio value/weightOfMostCriticalDimension descending\n\t7 = ratio value/weightOfMostCriticalDimension crescent\n\t8 = ratio value/weightOfMostFullDimension descending\n\t9 = ratio value/weightOfMostFullDimension crescent\n\n");
	printf("Enter the heuristicMode : ");
	scanf("%d", heuristicMode);
	printf("Enter the schedulingMode : ");
	scanf("%d", schedulingMode);
	putchar('\n');
}


Solution** sorter (InstanceDB *DB, Solution** listSolutions[], int *nbSolutionsPerInstance, int nbSolutionsExpected)
{
	//sort from best to worst
	for (int i = 0; i < DB->nbInstance; i++)
	{
		for (int sort1 = 0; sort1 < *nbSolutionsPerInstance; sort1++)
		{
			for (int sort2 = sort1; sort2 < *nbSolutionsPerInstance; sort2++)
			{
				if (listSolutions[i][sort2]->objective > listSolutions[i][sort1]->objective)
				{
					Solution* tmp = listSolutions[i][sort1];
					listSolutions[i][sort1] = listSolutions[i][sort2];
					listSolutions[i][sort2] = tmp;
				}
			}
		}
	}

	//if activated, stores only the expected number of solutions and del the others
	if (nbSolutionsExpected != -1 && nbSolutionsExpected < *nbSolutionsPerInstance)
	{
		for (int i = 0; i < DB->nbInstance; i++)
		{
			if (listSolutions[i][nbSolutionsExpected])
			{
				for (int j = nbSolutionsExpected; j < *nbSolutionsPerInstance; j++)
				{
					Solution_del(listSolutions[i][j]);
				}
			}
		}
		*nbSolutionsPerInstance = nbSolutionsExpected;
	}
    return *listSolutions;
}



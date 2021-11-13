#include "../include/metaheuristic.h"


/**
 * Metaheuristic static or dynamic according to heuristicMode
 *
 * Modify initialSolution using a metaheuristic (local research)
 */
void Metaheuristic_localResearch(Solution* initialSolution)
{
    Solution bestSolution = *initialSolution;
    int continuer = 1;

    while (continuer)
    {
        //printf("metaheuristic : objective value : %d\n", bestSolution.objective);//debug
        for (int i = 0; i<bestSolution.instance->nbObject; i++)
        {
            //part 1 : all cases of adding objects
            //as long as there are still workable solutions continue to add objects
            //for adding objects no need to test the objective value: if an object is added, the solution will necessarily be better than before

            if (bestSolution.directCoding[i]==0)
            {
                bestSolution.directCoding[i] = 1;
            }
            Solution_updateWeights(&bestSolution);
            bestSolution.objective = Solution_objective(bestSolution.instance, &bestSolution);
            Solution voisineSolution = bestSolution;

            for (int k = 0; k<voisineSolution.instance->nbObject; k++)
            {
                for (int j = 0; j<voisineSolution.instance->nbObject; j++)
                {
                //part 2 : all cases of exchange of items
                    if (voisineSolution.directCoding[k]==0 && voisineSolution.directCoding[j]==1)
                    {
                        voisineSolution.directCoding[k] = 1;
                        voisineSolution.directCoding[j] = 0;

                        if (!Solution_isPossible(voisineSolution.directCoding, voisineSolution.instance))
                        {
                        //if the new solution is not feasible we cancel it
                            voisineSolution.directCoding[k] = 0;
                            voisineSolution.directCoding[j] = 1;
                        }
                        else
                        {
                            Solution_updateWeights(&voisineSolution);
                            voisineSolution.objective = Solution_objective(voisineSolution.instance, &voisineSolution);

                            //printf("metaheuristic : objective value bestSolution : %d\n", bestSolution.objective);//debug
                            //printf("metaheuristic : objective value voisineSolution : %d\n", voisineSolution.objective);//debug
                            
                            if (voisineSolution.objective > bestSolution.objective)
                            {
                            //if the objective value of the new solution is better than the best one found before, it is kept
                                bestSolution = voisineSolution;
                            }
                        }
                    }
                }
            }

            if(!Solution_isPossible(bestSolution.directCoding, bestSolution.instance))
            //if none of the neighboring solutions calculated by adding the object i and swapping objects is feasible we cancel the addition
            {
                //if we can't add the object we don't do it
                bestSolution.directCoding[i] = 0;
            }
        }
        
        if (bestSolution.objective <= initialSolution->objective)
        {
        //loop stop condition: the objective value of the solution has not changed
            continuer = 0;
        }
        else
        {
            *initialSolution = bestSolution;
        }
    }
}
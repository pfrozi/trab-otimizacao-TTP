#include <iostream>
#include <list>
#include "Individual.h"
#include <math.h>
#include "Util.h"
#include <limits>       // std::numeric_limits
#include <algorithm>

#ifndef POPULATION_H
#define POPULATION_H

class Population
{
    public:

        Population();
        virtual ~Population();

        void GenerateRandom(int length);

        void SetLength(int len);
        void SetNTeams(int n);
        void CalcFitness();
        void SelectParents(float eliteRate);
        void ParentsCrossover(float cRate);
        void ParentsMutation(float pRate, float mRate);

        void SetDistMatrix(float** matrix);

        void GenerateNewPopulation(Population* newPopulation);

        float GetBestFitness();
        Individual* GetBestIndividual();

        void CopyIndividual(int index, Individual* individual);

    protected:

    private:

        int         length;
        int         nTeams;
        int         rounds;

        float       bestFitness;
        float       worstFitness;
        float       avgFitness;

        Individual* bestIndividual;
        Individual** individuals;

        float** matrixDist;                          // Matriz de distancias entre as cidades dos times

        std::list<Individual*>   lsBestParents;

        std::vector<Individual*>* bestParents;
        std::vector<Individual*>* childCrossover;
        std::vector<Individual*>* childMutation;

        void setBestIndividual(Individual* i);
        bool verifyStoppage();

        void printParents();
        void printCrossOver();
        void printMutation();

};

#endif // POPULATION_H

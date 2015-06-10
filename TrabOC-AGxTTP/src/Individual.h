#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sstream>

#include "Util.h"

#define FIT_ERR pow(10.0,10.0)
#define FIT_PLAY_YOURSELF   17.0
#define FIT_MATCHS_PER_RND  16.0
#define FIT_UNIQUE          15.0
#define FIT_ONE_GAME_ROUND  14.0
#define FIT_3GAMES_HOME     13.0
#define FIT_3GAMES_OUT      12.0
#define FIT_EACH_OTHER_AGN  11.0

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class Individual
{
    public:


        Individual();
        virtual ~Individual();


        bool operator ==(const Individual& i);
        bool operator !=(const Individual& i);
        bool operator >=(const Individual& i);
        bool operator <=(const Individual& i);
        bool operator  >(const Individual& i);
        bool operator  <(const Individual& i);

        void        SetLengthChromo(int len);
        void        SetNTeams(int n);
        void        SetDistMatrix(float** matrix);

        bool        GetAllele(int index);
        void        SetAllele(int index, bool value);

        std::string ToString();
        std::string ToString(std::vector<std::string> teams);
        float       GetFitness();
        void        SetFitness(float newFitness);

        float       CheckFitness();     // Verifica / Calcula o valor da funcao fitness;

        void        GenerateRdm();

        Individual*  Crossover(Individual* individual);
        Individual*  Crossover(Individual* individual, Individual* newIndividual);
        Individual*  Mutate(float mRate);
        Individual*  Mutate(float mRate, Individual* newIndividual);

        float             fitness;            // Valor do fitness, Representa o resultado da funcao de fitness aplicada a instancia (cromossomo)

    protected:
    private:

        int         length;                  // Quantidade de alelos do individuo
		int         nTeams;
        int         rounds;
		bool 		initialized;
		int*		truePositions;
		int 		truePositionsLenght;

		float**     matrixDist;               // Matriz de distancias entre as cidades dos times

        bool*             chromosome;         // Representacao da solucao


        void        SetTruePositionValue(int i, int j, int k, int value);
        int         GetTruePositionsValue(int i, int j, int k);
        int         GetTruePositions(int i, int j, int k);
        void        GetTruePositionsInit();
        void        SetPositionValue(int i, int j, int k, int value);
        int         GetPosition(int i, int j, int k);
        int         GetPositionValue(int i, int j, int k);
        int         ValidatePlayYourself();
        int         ValidateMatchsPerRound();
        int         ValidateGameOneTime();
        int         ValidateOneGame();
        int         ValidateMaxThreeGamesHome();
        int         ValidateMaxThreeGamesOut();
        int         ValidatePlayEachOtherAgain();
        float       ObjectiveFunction();
        int         ValidateOneGamePerTeamPerRound();
        float       ObjectiveFunctionFirstRound();
        float       ObjectiveFunctionMidleRounds();
        float       ObjectiveFunctionLastRound();
        int         ValidateUniqueGame();

        void        MutationInRound(Individual* newIndividual, float mRate);
        void        MutationRestrict(Individual* newIndividual, float mRate);
        void        MutationInRoundWithAll(Individual* newIndividual,float mRate);
        void        MutationRestrictSwap(Individual* newIndividual,float mRate);
        void        MutationRestrictToUnique(Individual* newIndividual,float mRate);
        void        MutationRestrictSwapUnique(Individual* newIndividual,float mRate);

        bool        CheckFitnessType(float type);


};

#endif // INDIVIDUAL_H

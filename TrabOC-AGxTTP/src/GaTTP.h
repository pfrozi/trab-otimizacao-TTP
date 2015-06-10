#include <fstream>
#include <string>
#include <iostream>
#include <list>
#include <time.h>

#include "Population.h"#include "Individual.h"

#include "Util.h"

#ifndef GATTP_H
#define GATTP_H

class GaTTP
{
    public:


        GaTTP();
        virtual ~GaTTP();
        void GenerateInitial();                         // Gera a população inicial que será utilizada pelo algoritimo
        void GenerateInitial(int length);               // Gera a população inicial que será utilizada pelo algoritimo
        void Solve();                                   // Processa o algoritmo até que os critérios de parada sejam aceitos

        std::string GetCurrent();

        time_t GetStartTime();
        time_t GetEndTime();

        double TimeElapsedInMinutes();
        std::string StrTimeElapsed();

        void SetNTeams(int n);
        void SetNPopInitial(int n);
        void SetPRate(float rate);
        void SetCRate(float rate);
        void SetMRate(float rate);
        void SetStopN(int n);
        void SetStopTime(int minutes);
        void SetDistMatrix(std::string strMatrix);
        void SetOutputFile(std::string file);

    protected:



    private:

        // Parametros de entrada

        std::vector<std::string> teams;              // Lista dos times considerados
        float** matrixDist;                          // Matriz de distancias entre as cidades dos times

        int   nTeams;
        int   nPopIn;
        float pRate;
        float cRate;
        float mRate;
        float eliteRate;
        int   stopQuant;
        int   stopTime;

        std::string outputFile;

        time_t startClock, endClock;

        Population* current;

        long        generation;
        int         genNoImprov;

        void nextGeneration();
        bool verifyStoppage();

        Individual bestIndividual;
};

#endif // GATTP_H

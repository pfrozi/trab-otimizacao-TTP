#include "GaTTP.h"


GaTTP::GaTTP()
{
    cRate = 0;
    pRate = 0;

    generation = 0;
    genNoImprov = 0;

    current = new Population;
    startClock = clock();
}

GaTTP::~GaTTP()
{

}

double GaTTP::TimeElapsedInMinutes(){

    return (((float)endClock-startClock)/CLOCKS_PER_SEC)/60.0;

}
std::string GaTTP::StrTimeElapsed(){

    std::stringstream out;

    out << ((float)endClock-startClock)/CLOCKS_PER_SEC;
    out << " segundos";

    return out.str();

}

void GaTTP::SetNTeams(int n){

    nTeams = n;

}
void GaTTP::SetNPopInitial(int n){

    nPopIn = n;

}


void GaTTP::SetMRate(float rate){

    mRate = rate;

}
void GaTTP::SetPRate(float rate){

    pRate = rate;
    eliteRate  = 1.0f - (pRate+cRate);

}
void GaTTP::SetCRate(float rate){

    cRate = rate;
    eliteRate  = 1.0f - (pRate+cRate);

}
void GaTTP::SetStopN(int n){

    stopQuant = n;

}
void GaTTP::SetStopTime(int minutes){

    stopTime = minutes;

}

void GaTTP::GenerateInitial(){

    GenerateInitial(nPopIn);

}

// Gera a populacao inicial aleatoria
void GaTTP::GenerateInitial(int nPop){

    nPopIn = nPop;
    current->SetNTeams(nTeams);
    current->SetDistMatrix(matrixDist);
    current->GenerateRandom(nPopIn);
}

void GaTTP::SetDistMatrix(std::string strMatrix){

    float** matrix = readMatrix(strMatrix, &teams, matrixDist);

    matrixDist = new float*[nTeams];
    for(int i=0;i<nTeams;i++){
        matrixDist[i] = new float[nTeams];
        for(int j=0;j<nTeams;j++){

            matrixDist[i][j]=matrix[i][j];
        }
    }

}

void GaTTP::nextGeneration(){

    Population* nextPop = new Population;

    current->GenerateNewPopulation(nextPop);

    generation++;

    nextPop->CalcFitness();

    if(nextPop->GetBestFitness()>=current->GetBestFitness()) {
        genNoImprov++;
    }
    else{
        genNoImprov = 0;
    }

    delete current;
    current = nextPop;

    //bestIndividual = newPop->GetBestIndividual();

    endClock = clock();

}

void GaTTP::Solve() {

    // Gera uma populacao randomica inicial
    GenerateInitial();
    current->CalcFitness();

    do{

        // Realiza a selecao dos pais
        current->SelectParents(eliteRate);

        // Realiza a reproducao por mutacao, crossover e elitismo
        current->ParentsMutation(pRate, mRate);
        current->ParentsCrossover(cRate);

        nextGeneration();

    }while(!verifyStoppage()); // verifica condicao de parada

    std::cout << "Tempo Total: ";
    std::cout << StrTimeElapsed() << std::endl;
    std::cout << "Total de geracoes: ";
    std::cout << generation << std::endl;
    std::cout << "**** Solucao **** ";
    std::cout << current->GetBestIndividual()->ToString(teams);

    std::ofstream out(outputFile.c_str(), std::ofstream::out);
    out << "Tempo Total: ";
    out << StrTimeElapsed() << std::endl;
    out << "Total de geracoes: ";
    out << generation << std::endl;
    out << "**** Solucao **** ";
    out << current->GetBestIndividual()->ToString(teams);
    out.close();

    delete current;


}

bool GaTTP::verifyStoppage(){

    // Parada por tempo OU parada por quantidade de geracoes sem melhora
    bool time = (TimeElapsedInMinutes()>stopTime);
    bool gen = (genNoImprov>stopQuant);

    return(time || gen);

}

void GaTTP::SetOutputFile(std::string file){

    outputFile = file;

}

std::string GaTTP::GetCurrent(){



}

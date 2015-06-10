#include "Population.h"


Population::Population()
{
    //ctor
    bestParents = new std::vector<Individual*>;
    childCrossover = new std::vector<Individual*>;
    childMutation = new std::vector<Individual*>;


    individuals     = NULL;
    bestIndividual  = NULL;
}

Population::~Population()
{

    bestParents->clear();

    for(int i=0;i<childCrossover->size();i++) delete childCrossover->at(i);
    childCrossover->clear();

    for(int i=0;i<childMutation->size();i++) delete childMutation->at(i);
    childMutation->clear();

    for(int i=0;i<length;i++) delete individuals[i];

    delete individuals;

    delete bestParents;
    delete childCrossover;
    delete childMutation;


}

void Population::SetNTeams(int n){

    nTeams = n;
    rounds = 2*(nTeams-1);
}


void Population::GenerateRandom(int length){

    SetLength(length);

    for(int l=0;l<length;l++){


        individuals[l]->SetLengthChromo(nTeams*nTeams*rounds);
        individuals[l]->SetNTeams(nTeams);
        individuals[l]->SetDistMatrix(matrixDist);

        individuals[l]->GenerateRdm();

    }



}
void Population::CalcFitness(){

    float curr, sum = 0;

    bestFitness = std::numeric_limits<float>::max();
    worstFitness = 0;

    for(int i=0;i<length;i++){

        curr = individuals[i]->CheckFitness();
        if(curr<bestFitness){

            bestFitness = curr;
            setBestIndividual(individuals[i]);

        }
        else if(curr>worstFitness){

            worstFitness = curr;

        }
        sum+=curr;

    }
    avgFitness = sum / (float)length;

}
void Population::SetLength(int len){

    length = len;

    if(individuals!=NULL){
        for(int i=0;i<length;i++) delete individuals[i];
        delete individuals;
    }
    if(bestIndividual!=NULL) delete bestIndividual;
    individuals = new Individual*[length];

    for(int i=0;i<length;i++){
        individuals[i] = new Individual;
    }

}
void Population::setBestIndividual(Individual* i){

    bestIndividual = i;

}

bool const compareIndividuals(const Individual* lhs,const Individual* rhs)
{

  return lhs->fitness < rhs->fitness;
}
void Population::SelectParents(float eliteRate){

    int parentsLen = ceil(length*eliteRate);

    for(int i=0;i<length;i++){

        bestParents->push_back(individuals[i]);

    }


    std::sort(bestParents->begin(),bestParents->end(),compareIndividuals);
    bestParents->resize(parentsLen);

}

void Population::printParents() {

    std::cout << "######### Best Parents #########" << std::endl;

    for(int i=0;i<bestParents->size();i++){
        std::cout << bestParents->at(i)->ToString();
    }

}
void Population::printCrossOver() {

    std::cout << "######### CrossOver #########" << std::endl;

    for(int i=0;i<childCrossover->size();i++){
        std::cout << childCrossover->at(i)->ToString();
    }

}
void Population::printMutation() {

    std::cout << "######### Mutation #########" << std::endl;

    for(int i=0;i<childMutation->size();i++){
        std::cout << childMutation->at(i)->ToString();
    }

}

void Population::ParentsCrossover(float cRate){

    int crossLen = ceil(length*cRate);
    int parent1,parent2;

    for(int i=0;i<crossLen;i++){
        parent1 = GetRdmInt(0,bestParents->size()-1);

        if(parent1>(bestParents->size()/2)){
            parent2 = GetRdmInt(0,parent1-1);
        }
        else{
            parent2 = GetRdmInt(parent1+1,bestParents->size()-1);
        }


        Individual* individual1 = bestParents->at(parent1);
        Individual* individual2 = bestParents->at(parent2);

        Individual* newIndividual = new Individual;
        individual1->Crossover(individual2,newIndividual);
        childCrossover->push_back(newIndividual);

    }



//    printCrossOver();

}

void Population::ParentsMutation(float pRate, float mRate){

    int mutateLen = ceil(length*pRate);
    int parent1;

    for(int i=0;i<mutateLen;i++){
        parent1 = GetRdmInt(0,bestParents->size()-1);

        Individual* individual1   = bestParents->at(parent1);
        Individual* newIndividual = new Individual;

        individual1->Mutate(mRate,newIndividual);

        childMutation->push_back(newIndividual);
    }

//    printMutation();
}

void Population::SetDistMatrix(float** matrix){

    matrixDist = matrix;
}

float Population::GetBestFitness()
{
    return bestFitness;
}

void Population::GenerateNewPopulation(Population* newPopulation){

    int newLength = bestParents->size()+childCrossover->size()+childMutation->size();

    newPopulation->SetLength(newLength);
    newPopulation->SetNTeams(nTeams);
    newPopulation->SetDistMatrix(matrixDist);

    int k = 0;
    for(int l=0;l<bestParents->size();l++){
        newPopulation->CopyIndividual(k,bestParents->at(l));
        k++;
    }
    for(int i=0;i<childCrossover->size();i++){
        newPopulation->CopyIndividual(k,childCrossover->at(i));
        k++;
    }
    for(int j=0;j<childMutation->size();j++){
        newPopulation->CopyIndividual(k,childMutation->at(j));
        k++;
    }



}

void Population::CopyIndividual(int index, Individual* individual){


    individuals[index]->SetLengthChromo(nTeams*nTeams*rounds);
    individuals[index]->SetNTeams(nTeams);
    individuals[index]->SetDistMatrix(matrixDist);

    for(int i=0;i<nTeams*nTeams*rounds;i++){
        individuals[index]->SetAllele(i,individual->GetAllele(i));
    }

}

Individual* Population::GetBestIndividual(){

    return bestIndividual;

}

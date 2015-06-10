#include "Individual.h"

Individual::Individual()
{
    //ctor
    chromosome    = NULL;
    truePositions = NULL;

    initialized = true;


}

Individual::~Individual()
{
    delete chromosome;
    delete truePositions;

}
bool Individual::operator ==(const Individual& i)
{
    return (fitness == i.fitness);
}
bool Individual::operator !=(const Individual& i)
{
    return (fitness != i.fitness);
}
bool Individual::operator >=(const Individual& i)
{
    return (fitness >= i.fitness);
}
bool Individual::operator <=(const Individual& i)
{
    return (fitness <= i.fitness);
}
bool Individual::operator >(const Individual& i)
{
    return (fitness > i.fitness);
}
bool Individual::operator <(const Individual& i)
{
    return (this->fitness < i.fitness);
}

void Individual::SetLengthChromo(int len){

    length = len;

    if(truePositions!=NULL){

        delete truePositions;
        truePositions = NULL;
    }
    if(chromosome!=NULL){

        delete chromosome;
        chromosome = NULL;
    }

    chromosome = new bool[length];

    for(int i=0;i<length;i++) chromosome[i]=false;

}

void Individual::SetNTeams(int n){

    nTeams = n;
    rounds = 2*(nTeams-1);
}

void Individual::GenerateRdm(){

    bool games[nTeams][nTeams];
    int i_new;
    int j_new;

    for(int i=0;i<nTeams;i++){
        for(int j=0;j<nTeams;j++){
            games[i][j]=false;
        }
    }

    int nMatch = nTeams/2;

    int iPrev[nMatch];
    int jPrev[nMatch];

    for(int k=0;k<rounds;k++){

        for(int m=0;m<nMatch;m++) iPrev[m]=-1;
        for(int m=0;m<nMatch;m++) jPrev[m]=-1;

        for(int l=0;l<nMatch;l++){

            i_new = (rand() % nTeams);
            j_new = (rand() % nTeams);
            if(i_new==j_new) j_new=((i_new+1) % nTeams);

            if(games[i_new][j_new]){
                bool ok = false;
                for(i_new = 0;i_new<nTeams;i_new++){
                    for(j_new = 0;j_new<nTeams;j_new++){
                        if(i_new!=j_new && !(games[i_new][j_new])){
                            ok = true;
                            break;
                        }
                    }
                    if(ok) break;
                }
            }

            games[i_new][j_new] = true;

            SetPositionValue(i_new,j_new,k,true);

            iPrev[l]=i_new;
            jPrev[l]=j_new;
        }


    }
}

void Individual::SetDistMatrix(float** matrix){

    matrixDist = matrix;
}

float Individual::CheckFitness() {


    float total = 0.0f;
	float fo = ObjectiveFunction();
	int playYourself = ValidatePlayYourself();
	int matchsPerRound = ValidateMatchsPerRound();
	int oneGamePerTeamPerRound = ValidateOneGamePerTeamPerRound();
	int maxThreeGamesHome = ValidateMaxThreeGamesHome();
	int maxThreeGamesOut = ValidateMaxThreeGamesOut();
	int playEachOtherAgain = ValidatePlayEachOtherAgain();
	int validateUniqueGame = ValidateUniqueGame();

	total += playYourself               * pow(10.0,FIT_PLAY_YOURSELF);
	total += matchsPerRound             * pow(10.0,FIT_MATCHS_PER_RND);
	total += validateUniqueGame         * pow(10.0,FIT_UNIQUE);
	total += oneGamePerTeamPerRound     * pow(10.0,FIT_ONE_GAME_ROUND);
	total += maxThreeGamesHome          * pow(10.0,FIT_3GAMES_HOME);
	total += maxThreeGamesOut           * pow(10.0,FIT_3GAMES_OUT);
	total += playEachOtherAgain         * pow(10.0,FIT_EACH_OTHER_AGN);

	total += fo;

    fitness = total;

	return total;
}

float Individual::ObjectiveFunctionFirstRound() {
    float total = 0.0f;

    int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int firstRoundIndex = 0;
    int firstRound[300];
    int firstRoundLength = 0;
    int x = 0;
    for (x = 0; x < (nTeams/2) * 3; x++){
        firstRound[x] = 0;
    }
    firstRoundLength = x;
    //std::cout << "firstRoundLength: " << firstRoundLength <<endl;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
        j0 = truePositions[index+1];
    	k0 = truePositions[index+2];

        // store i,j,k of first round
        if(k0 == 0) {
            firstRound[firstRoundIndex] = i0;
            firstRound[firstRoundIndex+1] = j0;
            firstRound[firstRoundIndex+2] = k0;
            firstRoundIndex += 3;
        }
    }

    for(int x=0; x < firstRoundLength; x+=3){
        i0 = firstRound[x];
        j0 = firstRound[x+1];
        k0 = firstRound[x+2];
        total += matrixDist[i0][j0];
    }

    return total;
}

float Individual::ObjectiveFunctionMidleRounds() {
	float total = 0.0f;

	int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
    	j0 = truePositions[index+1];
		k0 = truePositions[index+2];

        for(int jndex=0; jndex < truePositionsLenght; jndex+=3){
            i1 = truePositions[jndex];
            j1 = truePositions[jndex+1];
		    k1 = truePositions[jndex+2];

			if(k0 + 1 == k1){

				// time i0 viaja ate a casa de j1
				if(i0 == j1){

                   total += float(matrixDist[i0][i1]);
				}

				// time j0 viaga para casa
				if(j0 == i1){

                    total += float(matrixDist[i0][i1]);
				}

				// time j0 viaga para casa de i1
				if(j0 == j1){

                    total += float(matrixDist[i0][i1]);
				}

				// i0 permanece em casa
				if(i0 == i1) {

					total += 0;
				}
			}
		}
	}

    return total;
}

float Individual::ObjectiveFunctionLastRound() {
    float total = 0.0f;

    int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int lastRoundIndex = 0;
    int lastRound[300];
    int lastRoundLength = 0;
    int x = 0;
    for (x = 0; x < (nTeams/2) * 3; x++){
        lastRound[x] = 0;
    }
    lastRoundLength = x;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
        j0 = truePositions[index+1];
		k0 = truePositions[index+2];

        // store i,j,k of last round
        if(k0 == rounds-1) {
            lastRound[lastRoundIndex] = i0;
            lastRound[lastRoundIndex+1] = j0;
            lastRound[lastRoundIndex+2] = k0;
            lastRoundIndex += 3;
        }
    }

    for(int x=0; x < lastRoundLength; x+=3){
        i0 = lastRound[x];
        j0 = lastRound[x+1];
        k0 = lastRound[x+2];

        total += matrixDist[i0][j0];
    }

    return total;
}

float Individual::ObjectiveFunction() {
    GetTruePositionsInit();

    float total = 0.0f;
    float firstRound = ObjectiveFunctionFirstRound();
    float midleRounds = ObjectiveFunctionMidleRounds();
    float lastRound = ObjectiveFunctionLastRound();

    total += firstRound + midleRounds + lastRound;

    return total;
}

int Individual::GetPosition(int i, int j, int k){
	return i*nTeams*rounds+j*rounds+k;
}

int Individual::GetPositionValue(int i, int j, int k){
    return chromosome[GetPosition(i,j,k)];
}

void Individual::SetPositionValue(int i, int j, int k, int value){
	chromosome[GetPosition(i,j,k)] = value;
}

Individual* Individual::Crossover(Individual* individual){

    Individual* newIndividual = new Individual;

    newIndividual->SetLengthChromo(nTeams*nTeams*rounds);
    newIndividual->SetNTeams(nTeams);
    newIndividual->SetDistMatrix(matrixDist);

    for(int i=0;i<length;i++){

        if(i<length/2){
            newIndividual->SetAllele(i,chromosome[i]);
        }
        else{
            newIndividual->SetAllele(i,individual->GetAllele(i));
        }
    }

    return newIndividual;
}
Individual* Individual::Crossover(Individual* individual, Individual* newIndividual){

    newIndividual->SetLengthChromo(nTeams*nTeams*rounds);
    newIndividual->SetNTeams(nTeams);
    newIndividual->SetDistMatrix(matrixDist);

    for(int i=0;i<length;i++){

        if(i<length/2){
            newIndividual->SetAllele(i,chromosome[i]);
        }
        else{
            newIndividual->SetAllele(i,individual->GetAllele(i));
        }
    }

    return newIndividual;
}

Individual* Individual::Mutate(float mRate){

    Individual* newIndividual = new Individual;

    newIndividual->SetLengthChromo(nTeams*nTeams*rounds);
    newIndividual->SetNTeams(nTeams);
    newIndividual->SetDistMatrix(matrixDist);

    newIndividual->fitness = this->fitness;

    for(int a=0;a<nTeams*nTeams*rounds;a++){
        newIndividual->SetAllele(a,chromosome[a]);
    }


    MutationRestrictSwap(newIndividual,mRate);


    return newIndividual;

}

Individual* Individual::Mutate(float mRate, Individual* newIndividual){

    newIndividual->SetLengthChromo(nTeams*nTeams*rounds);
    newIndividual->SetNTeams(nTeams);
    newIndividual->SetDistMatrix(matrixDist);

    newIndividual->fitness = this->fitness;

    for(int a=0;a<nTeams*nTeams*rounds;a++){
        newIndividual->SetAllele(a,chromosome[a]);
    }


    MutationRestrictSwapUnique(newIndividual,mRate);


    return newIndividual;

}

void Individual::MutationRestrict(Individual* newIndividual, float mRate){


        int matches[nTeams][nTeams];
        int ks[nTeams][nTeams][rounds];

        for(int i=0;i<nTeams;i++){
            for(int j=0;j<nTeams;j++){
                matches[i][j] = 0;
            }
        }

        for(int k=0;k<rounds;k++){
            for(int i=0;i<nTeams;i++){
                for(int j=0;j<nTeams;j++){
                    if(newIndividual->GetPositionValue(i,j,k)) {
                        ks[i][j][matches[i][j]] = k;
                        matches[i][j]++;

                    }
                }
            }
        }
        bool keep;
        do{
            int k_newgame = -1;
            keep = false;
            for(int i=0;i<nTeams;i++){
                for(int j=0;j<nTeams;j++){
                    if(k_newgame<0 && matches[i][j]>1) {
                        k_newgame = ks[i][j][matches[i][j]-1];
                        newIndividual->SetPositionValue(i,j,k_newgame,false);
                        matches[i][j]--;
                        keep = true;
                        break;
                    }
                }
                if(k_newgame>=0) break;
            }
            if(!keep)break;
            for(int i=0;i<nTeams;i++){
                bool stop=false;

                for(int j=0;j<nTeams;j++){

                    bool ok = true;
                    if(i!=j && matches[i][j]<1){

                        for(int l=0;l<nTeams;l++) ok&=!newIndividual->GetPositionValue(l,i,k_newgame) && !newIndividual->GetPositionValue(i,l,k_newgame);
                        if(!ok) continue;
                        for(int l=0;l<nTeams;l++) ok&=!newIndividual->GetPositionValue(l,j,k_newgame) && !newIndividual->GetPositionValue(j,l,k_newgame);
                        if(!ok) continue;
                        newIndividual->SetPositionValue(i,j,k_newgame,true);
                        stop = true;
                        break;
                    }
                }
                if(stop) break;
            }

        }while(keep);


}

void Individual::MutationInRound(Individual* newIndividual,float mRate)
{
    std::vector<int> in;
    std::vector<int> out;

    for(int k=0;k<rounds;k++){

        if(GetRdmBool(mRate)){

            for(int i=0;i<nTeams;i++){
                for(int j=0;j<nTeams;j++){

                    if(newIndividual->GetPositionValue(i, j, k)){
                        in.push_back(i);
                        out.push_back(j);

                        newIndividual->SetPositionValue(i,j,k,false);
                    }
                }
            }

            while(in.size()>0 && out.size()>0){

                int r1 = rand() % in.size();
                int r2 = rand() % out.size();

                int iNew,jNew;

                if(rand()%2==0){
                    iNew = in.at(r1);
                    jNew = out.at(r2);

                    in.erase(in.begin()+r1);
                    out.erase(out.begin()+r2);
                }
                else {
                    iNew = out.at(r1);
                    jNew = in.at(r2);

                    out.erase(out.begin()+r1);
                    in.erase(in.begin()+r2);
                }

                newIndividual->SetPositionValue(iNew,jNew,k,true);
            }


        }
    }

    in.clear();
    out.clear();
}

void Individual::MutationRestrictSwap(Individual* newIndividual,float mRate)
{
    bool tChanged[nTeams][nTeams];
    int iRdm1, jRdm1, kRdm1;
    int iRdm2, jRdm2, kRdm2;

    for(int l=0;l<nTeams;l++) for(int m=0;m<nTeams;m++) tChanged[l][m] = false;

    //std::cout << newIndividual->ToString() << std::endl;
    for(int g=0;g<(nTeams/4)*rounds;g++){

        if(!GetRdmBool(mRate)) continue;

        iRdm1 = rand() % nTeams;
        jRdm1 = rand() % nTeams;
        if(iRdm1==jRdm1) jRdm1 = (iRdm1+1) % nTeams;

        iRdm2 = rand() % nTeams;
        jRdm2 = rand() % nTeams;
        if(iRdm2==jRdm2) jRdm2 = (iRdm2+1) % nTeams;

        if(tChanged[iRdm1][jRdm1]){

            iRdm1 = -1;
            jRdm1 = -1;

            for(int q=nTeams-1;q>=0 && iRdm1<0;q--){

                for(int p=nTeams-1;p>=0;p--){

                    if(q!=p && !tChanged[p][q]){

                        iRdm1 = p;
                        jRdm1 = q;
                        break;
                    }

                }
            }
        }

        if(tChanged[iRdm2][jRdm2]
        || (iRdm1==iRdm2 || iRdm1==jRdm2 || jRdm1==iRdm2 || jRdm1==jRdm2)){

            iRdm2 = -1;
            jRdm2 = -1;

            for(int q=nTeams-1;q>=0 && iRdm2<0;q--){

                for(int p=nTeams-1;p>=0;p--){

                    if(q!=p && !tChanged[p][q]
                        && !(iRdm1==p || iRdm1==q || jRdm1==p || jRdm1==q)){

                        iRdm2 = p;
                        jRdm2 = q;
                        break;
                    }

                }
                if(!iRdm2<0) break;
            }
        }

        tChanged[iRdm1][jRdm1]=true;
        tChanged[iRdm2][jRdm2]=true;

        // realiza o swap
        int k1=-1,k2=-1;
        for(int k=0;k<rounds || (k1<0&&k2<0);k++){

            if(newIndividual->GetPositionValue(iRdm1,jRdm1,k)){
                k1 = k;
            }
            if(newIndividual->GetPositionValue(iRdm2,jRdm2,k)){
                k2 = k;
            }
        }
        newIndividual->SetPositionValue(iRdm1,jRdm1,k1,false);
        newIndividual->SetPositionValue(iRdm1,jRdm1,k2,true);

        newIndividual->SetPositionValue(iRdm2,jRdm2,k2,false);
        newIndividual->SetPositionValue(iRdm2,jRdm2,k1,true);

    }


}
void Individual::MutationRestrictSwapUnique(Individual* newIndividual,float mRate)
{
    bool tChanged[nTeams][nTeams];
    std::vector<int> kWithGames;

    int iRdm1, jRdm1, kRdm1;
    int iRdm2, jRdm2, kRdm2;

    for(int l=0;l<nTeams;l++) for(int m=0;m<nTeams;m++) tChanged[l][m] = false;

    //std::cout << newIndividual->ToString() << std::endl;
    for(int g=0;g<(nTeams/4)*rounds;g++){

        if(!GetRdmBool(mRate)) continue;

        iRdm1 = rand() % nTeams;
        jRdm1 = rand() % nTeams;
        if(iRdm1==jRdm1) jRdm1 = (iRdm1+1) % nTeams;

        if(tChanged[iRdm1][jRdm1]){

            iRdm1 = -1;
            jRdm1 = -1;

            for(int q=nTeams-1;q>=0 && iRdm1<0;q--){

                for(int p=nTeams-1;p>=0;p--){

                    if(q!=p && !tChanged[p][q]){

                        iRdm1 = p;
                        jRdm1 = q;
                        break;
                    }

                }
            }
        }

        // verificar quais os k que possuem jogo de iRdm1 X jRdm1

        for(int k=0;k<rounds;k++){
            if(!newIndividual->GetPositionValue(iRdm1,jRdm1,k)){
                kWithGames.push_back(k);
            }
        }

        iRdm2 = -1;
        jRdm2 = -1;

        int k2Rdm = kWithGames[rand() % kWithGames.size()];
        for(int i=0;i<nTeams;i++){
            for(int j=0;j<nTeams;j++){
                if(newIndividual->GetPositionValue(i,j,k2Rdm)){
                    if(iRdm2<0 || GetRdmBool(0.5)){
                        iRdm2 = i;
                        jRdm2 = j;
                    }
                }

            }
        }

        tChanged[iRdm1][jRdm1]=true;
        tChanged[iRdm2][jRdm2]=true;

        // realiza o swap
        int k1=-1,k2=k2Rdm,kVar;
        for(int k=0;k<rounds;k++){

            if(newIndividual->GetPositionValue(iRdm1,jRdm1,k)){
                if(k1<0){
                    k1 = k;
                }
                else if((!newIndividual->GetPositionValue(iRdm2,jRdm2,k) && !newIndividual->GetPositionValue(jRdm2,iRdm2,k))){

                    if(k>0 && (!newIndividual->GetPositionValue(iRdm2,jRdm2,k-1) && !newIndividual->GetPositionValue(jRdm2,iRdm2,k-1))){
                        kVar = k;
                    }
                }

            }
        }
        if(k1<0) k1=kVar;

        newIndividual->SetPositionValue(iRdm1,jRdm1,k1,false);
        newIndividual->SetPositionValue(iRdm1,jRdm1,k2,true);

        newIndividual->SetPositionValue(iRdm2,jRdm2,k2,false);
        newIndividual->SetPositionValue(iRdm2,jRdm2,k1,true);

    }

    kWithGames.clear();
}

void Individual::MutationRestrictToUnique(Individual* newIndividual,float mRate)
{
    bool tArray[nTeams][nTeams];
    int iNew, jNew;


    if(GetRdmBool(mRate)){

        for(int l=0;l<nTeams;l++) for(int m=0;m<nTeams;m++) tArray[l][m] = false;

        for(int k=0;k<rounds;k++){

            for(int i=0;i<nTeams;i++){
                for(int j=0;j<nTeams;j++){

                    newIndividual->SetPositionValue(i,j,k,false);

                }
            }

            int games = 0;

            do {

                iNew = rand() % nTeams;
                jNew = rand() % nTeams;
                if(iNew==jNew) jNew = (iNew+1) % nTeams;

                if(tArray[iNew][jNew]){
                    iNew = -1;
                    jNew = -1;
                    for(int q=nTeams-1;q>=0;q--){

                        for(int p=nTeams-1;p>=0;p--){

                            if(!tArray[p][q]){

                                iNew = p;
                                jNew = q;
                                break;
                            }
                            if(!iNew<0 && !jNew<0) break;

                        }


                    }
                }


                tArray[iNew][jNew]=true;

                newIndividual->SetPositionValue(iNew,jNew,k,true);

                games++;


            }while(games!=(nTeams / 2));



        }
    }



}
void Individual::MutationInRoundWithAll(Individual* newIndividual,float mRate)
{
    bool tArray[nTeams];
    int iNew, jNew;


    for(int k=0;k<rounds;k++){

        if(GetRdmBool(mRate)){

            for(int l=0;l<nTeams;l++) tArray[l] = false;

            for(int i=0;i<nTeams;i++){
                for(int j=0;j<nTeams;j++){

                    newIndividual->SetPositionValue(i,j,k,false);

                }
            }

            int games=0;

            do {

                iNew = rand() % nTeams;
                jNew = rand() % nTeams;
                if(iNew==jNew) jNew = (iNew+1) % nTeams;

                if(tArray[iNew]){

                    for(int q=nTeams-1;q>=0;q--) if(!tArray[q]){ iNew=q; break; }
                }

                if(tArray[jNew]){

                    for(int p=nTeams-1;p>=0;p--) if(!tArray[p] && p!=iNew){ jNew=p; break; }
                }

                tArray[iNew]=true;
                tArray[jNew]=true;

                newIndividual->SetPositionValue(iNew,jNew,k,true);

                games++;


            }while(games!=(nTeams / 2));



        }
    }


}
void Individual::SetAllele(int index, bool value){

    chromosome[index] = value;

}
bool Individual::GetAllele(int index){

    return chromosome[index];

}

void Individual::GetTruePositionsInit() {



    if(truePositions!=NULL) {

        delete truePositions;
        truePositions = NULL;
    }
    truePositions = new int[1000];

    int count = 0;

	for(int i=0; i < nTeams; i++){
		for(int j=0; j < nTeams; j++){
			for(int k=0; k < rounds; k++){
                int value = GetPositionValue(i,j,k);
                if(value == 1)
				{
//					std::cout << "value 1 - i j k:" << i << j << k << std::endl;
                    truePositions[count] = i;
                    count += 1;
					truePositions[count] = j;
                    count += 1;
					truePositions[count] = k;
                    count += 1;
				}
			}
		}
	}

	truePositionsLenght = count;


	initialized = true;
}


int Individual::GetTruePositions(int i, int j, int k){
    return i*9+j*3+k;
}

int Individual::GetTruePositionsValue(int i, int j, int k){
	return truePositions[GetTruePositions(i,j,k)];
}

void Individual::SetTruePositionValue(int i, int j, int k, int value){
	truePositions[GetTruePositions(i,j,k)] = value;
}

int Individual::ValidatePlayYourself(){
    GetTruePositionsInit();
    int i = 0;
    int j = 0;
    int k = 0;

    int index = 0;
	while(index < truePositionsLenght) {
		i = truePositions[index];
		j = truePositions[index+1];
		k = truePositions[index+2];

        if(i == j) {

            return 1;
        }

		index += 3;
	}


	return 0;
}

int Individual::ValidateMatchsPerRound(){
    GetTruePositionsInit();
	int i = 0;
    int j = 0;
    int k = 0;
	int countk[rounds];

    for (int x = 0; x < rounds; x++){
      countk[x] = 0;
    }

	int index = 0;
	while(index < truePositionsLenght) {
		i = truePositions[index];
		j = truePositions[index+1];
		k = truePositions[index+2];

        countk[k] = countk[k] + 1;

        if(countk[k] > nTeams/2) {

            return 1;
		}

		index += 3;
	}

	for (int x = 0; x < rounds; x++){
        if(countk[x] != nTeams/2)
        {

            return 1;
        }
    }

	return 0;
}

int Individual::ValidateOneGamePerTeamPerRound(){
    GetTruePositionsInit();

	int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;

    int mount=0;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
    	j0 = truePositions[index+1];
		k0 = truePositions[index+2];

        for(int jndex=0; jndex < truePositionsLenght; jndex+=3){
            if(jndex != index) {
                i1 = truePositions[jndex];
                j1 = truePositions[jndex+1];
		        k1 = truePositions[jndex+2];

                if(k0 == k1) {
                    if((i0 == i1) || (j0 == j1) || (i0 == j1) || (j0 == i1)) {

                        mount++;
                    }
                }
            }
        }
    }

	return mount;
}

int Individual::ValidateUniqueGame(){
    GetTruePositionsInit();
    int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;

    for(int index=0; index < truePositionsLenght; index+=3){

        i0 = truePositions[index];
        j0 = truePositions[index+1];
        k0 = truePositions[index+2];

        for(int jndex=0; jndex < truePositionsLenght; jndex+=3){

            if(index!=jndex){

                i1 = truePositions[jndex];
                j1 = truePositions[jndex+1];
                k1 = truePositions[jndex+2];

                if(i0 == i1 && j0 == j1)
                {
                    return 1;
                }
            }
        }
    }


    return 0;
}

int Individual::ValidateMaxThreeGamesHome(){
    GetTruePositionsInit();
	int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;

    int i2 = 0;
    int j2 = 0;
    int k2 = 0;

    int i3 = 0;
    int j3 = 0;
    int k3 = 0;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
    	j0 = truePositions[index+1];
		k0 = truePositions[index+2];

        for(int jndex=0; jndex < truePositionsLenght; jndex+=3){
            i1 = truePositions[jndex];
            j1 = truePositions[jndex+1];
		    k1 = truePositions[jndex+2];

            if((k0+1 == k1) && (i0 == i1)) {
                for(int xndex=0; xndex < truePositionsLenght; xndex+=3){
                    i2 = truePositions[xndex];
                    j2 = truePositions[xndex+1];
            	    k2 = truePositions[xndex+2];

                    if((k1+1 == k2) && (i0 == i2)) {
                        for(int zndex=0; zndex < truePositionsLenght; zndex+=3){
                            i3 = truePositions[zndex];
                            j3 = truePositions[zndex+1];
                            k3 = truePositions[zndex+2];

                            if((k2+1 == k3) && (i0 == i3)) {

                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }

	return 0;
}

int Individual::ValidateMaxThreeGamesOut(){
    GetTruePositionsInit();
	int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;

    int i2 = 0;
    int j2 = 0;
    int k2 = 0;

    int i3 = 0;
    int j3 = 0;
    int k3 = 0;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
    	j0 = truePositions[index+1];
		k0 = truePositions[index+2];

        for(int jndex=0; jndex < truePositionsLenght; jndex+=3){
            i1 = truePositions[jndex];
            j1 = truePositions[jndex+1];
		    k1 = truePositions[jndex+2];

            if((k0+1 == k1) && (j0 == j1)) {
                for(int xndex=0; xndex < truePositionsLenght; xndex+=3){
                    i2 = truePositions[xndex];
                    j2 = truePositions[xndex+1];
            	    k2 = truePositions[xndex+2];

                    if((k1+1 == k2) && (j0 == j2)) {
                        for(int zndex=0; zndex < truePositionsLenght; zndex+=3){
                            i3 = truePositions[zndex];
                            j3 = truePositions[zndex+1];
                            k3 = truePositions[zndex+2];

                            if((k2+1 == k3) && (j0 == j3)) {

                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }


	return 0;
}

int Individual::ValidatePlayEachOtherAgain(){
    GetTruePositionsInit();
	int i0 = 0;
    int j0 = 0;
    int k0 = 0;

    int i1 = 0;
    int j1 = 0;
    int k1 = 0;

    for(int index=0; index < truePositionsLenght; index+=3){
        i0 = truePositions[index];
    	j0 = truePositions[index+1];
		k0 = truePositions[index+2];

        for(int jndex=0; jndex < truePositionsLenght; jndex+=3){
            i1 = truePositions[jndex];
            j1 = truePositions[jndex+1];
		    k1 = truePositions[jndex+2];

            if(k0+1 == k1){
                if(i0 == j1 && i1 == j0 || i0 == i1 && j0 == j1)
                {

                    return 1;
                }
            }
        }
    }

	return 0;
}

std::string Individual::ToString(){

    std::stringstream out;
    out << "--------------------------------" << std::endl;
    out << "Cromossomo: ";

    for(int i=0;i<length;i++){

        if(chromosome[i]){
            out << "1";
        }
        else {
            out << "0";
        }

    }

    out << std::endl;
    out << "Fitness: ";
    out << fitness;
    out << std::endl;
    out << "--- Representacao da Solucao ---" << std::endl;

    for(int k=0;k<rounds;k++){

        out << "Rodada " << (k+1) << ":" << std::endl;

        for(int i=0;i<nTeams;i++){
            for(int j=0;j<nTeams;j++){

                if((bool)GetPositionValue(i,j,k)){

                    out << "Time " << (i+1) << " X Time " << (j+1) << std::endl;

                }

            }
        }
    }

    out << "--------------------------------"<< std::endl;
    return out.str();

}

std::string Individual::ToString(std::vector<std::string> teams){

    std::stringstream out;
    out << "--------------------------------" << std::endl;
    out << "Cromossomo: ";

    for(int i=0;i<length;i++){

        if(chromosome[i]){
            out << "1";
        }
        else {
            out << "0";
        }

    }

    out << std::endl;
    out << "Fitness: ";
    out << fitness;
    out << std::endl;
    out << "--- Representacao da Solucao ---" << std::endl;

    for(int k=0;k<rounds;k++){

        out << "Rodada " << (k+1) << ":" << std::endl;

        for(int i=0;i<nTeams;i++){
            for(int j=0;j<nTeams;j++){

                if((bool)GetPositionValue(i,j,k)){

                    out << teams.at(i) << " X " << teams.at(j) << std::endl;

                }

            }
        }
    }

    out << "--------------------------------"<< std::endl;
    return out.str();

}
bool Individual::CheckFitnessType(float type){
    int expFit = log10(fitness);
    return(expFit==type);
}

# TEAMS são o conjunto de times que participam do torneio
set TEAMS;
 
# Matriz DIST representa a matriz de distancias entre as casas de cada um dos times
param DIST {TEAMS, TEAMS};
 
# N eh a quntidade de times que participam do campeonato
param N := card(TEAMS),integer;
 
# O numero de times deverá ser par
check: N mod 2 = 0;
 
# Kn representa a quantidade de rodadas do campeonato
param Kn := 2 * (N - 1);
 
# K representa as rodadas do campeonato
set K := 1..Kn;

# Q é a quantidade de jogos por rodada
param Q := N / 2;
 
# Variaveis
var x{TEAMS, TEAMS, K} binary;
var y{TEAMS, TEAMS, TEAMS, K} binary;
var z{TEAMS, TEAMS, TEAMS, K} binary;
var w{TEAMS, TEAMS, TEAMS, K} binary;
 
minimize distance :
    sum{i in TEAMS, j in TEAMS, l in TEAMS, k in 2..Kn} DIST[l,i]*y[i,j,l,k]
  + sum{i in TEAMS, j in TEAMS, l in TEAMS, k in 2..Kn} DIST[i,l]*z[i,j,l,k]
  + sum{i in TEAMS, j in TEAMS, l in TEAMS, k in 2..Kn} DIST[i,j]*w[i,j,l,k]
  + sum{i in TEAMS, j in TEAMS} DIST[i,j]*x[i,j,1]
  + sum{i in TEAMS, j in TEAMS} DIST[i,j]*x[i,j,Kn];
 
# Nenhum time joga consigo mesmo
s.t. PlayYourself{i in TEAMS,j in TEAMS : i = j}: sum{k in K} x[i,j,k] = 0;
 
# Quantidade de jogos por rodada
s.t. matchsPerRound{k in K}: sum{i in TEAMS, j in TEAMS : i <> j} x[i,j,k] = Q;

# Todo o jogo de i x j (par de times) ocorre apenas 1 vez
s.t. OneTime{i in TEAMS, j in TEAMS:i<>j}: sum{k in K} x[i,j,k] = 1;
 
# Em uma rodada em que i joga com j, tanto i quanto j NAO podem jogar outros jogos na mesma rodada
s.t. OneHome{i in TEAMS, k in K}: sum{j in TEAMS : i <> j} (x[i,j,k] + x[j,i,k]) = 1;
 
# Todo time joga no Kn/2 vezes como local
s.t. GamesHome{i in TEAMS}: sum{j in TEAMS,k in K : i <> j} x[i,j,k] = Kn / 2;
# Todo time joga no Kn/2 vezes como visitante
s.t. GamesOut{j in TEAMS}: sum{i in TEAMS,k in K : i <> j} x[i,j,k] = Kn / 2;
 
# No maximo tres jogos consecutivos fora
s.t. Max3TimesOut{j in TEAMS, k in 1..Kn-2}: sum{i in TEAMS : i <> j}( x[i,j,k]+x[i,j,k+1]+x[i,j,k+2]) <= 3;
 
# No maximo tres jogos consecutivos em casa
s.t. Max3TimesHome{i in TEAMS, k in 1..Kn-2}: sum{j in TEAMS : i <> j}( x[i,j,k]+x[i,j,k+1]+x[i,j,k+2]) <= 3;
 
# Dois times não podem se enfrentar em duas rodadas consecutivas
s.t. maxOneGame{i in TEAMS, j in TEAMS, k in 1..Kn-1 : i <> j}: x[i,j,k]+x[j,i,k+1] <= 1;
 
# Restricoes para a "linearizacao" da funcao objetivo
s.t. linY{i in TEAMS, j in TEAMS, l in TEAMS, k in 2..Kn : i<>j && i<>l && j<>l}: y[i,j,l,k] >= x[i,j,k]+x[l,i,k-1]-1;
s.t. linZ{i in TEAMS, j in TEAMS, l in TEAMS, k in 2..Kn : i<>j && i<>l && j<>l}: z[i,j,l,k] >= x[i,j,k]+x[l,j,k-1]-1;
s.t. linW{i in TEAMS, j in TEAMS, l in TEAMS, k in 2..Kn : i<>j && i<>l && j<>l}: w[i,j,l,k] >= x[i,j,k]+x[j,l,k-1]-1;

solve;
 
for {k in K}{
	printf"####################\nRodada %s:\n", k;
    for{i in TEAMS, j in TEAMS:x[i,j,k]} {
               
        printf "    %s X %s\n",i, j;
    }
}

#printf "\nMATRIZ DE JOGOS\n";
#printf "\nRodada";
#for{i in TEAMS}{
#printf "    %s", i;
#}
#for{k in K,i in TEAMS, j in TEAMS:x[i,j,k]} {
#
#printf "\n";
#printf "    %s%s", k, if x[j,i,k] then "@"else" ",if x[j,i,k] then "@"else" ",j;
#}

data;
 
set TEAMS :=
ATL 
NYM 
PHI 
MON 
FLA 
PIT 
;
 
param DIST: 
	ATL NYM PHI MON FLA PIT  :=
 ATL  0  745  665  929  605  521  
 NYM 745   0   80  337 1090  315  
 PHI 665  80    0  380 1020  257  
 MON 929 337  380    0 1380  408  
 FLA 605 1090 1020 1380   0 1010  
 PIT 521  315  257  408 1010   0;
  
  
  
  
  
  
  

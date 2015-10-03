#include <iostream>
#include <stdio.h>
#include "gamePlay.h"
/*
 * gamePlay.cpp
 *
 *  Created on: Feb 15, 2015
 *  Author: Daniel Kang & Yachen Sun
 *  Description: GamePlay, which will handle the play of the game
 *
 */

//MIGHT USE
 #define count(ARRAY) ((sizeof (ARRAY))/sizeof(ARRAY[0]))

using namespace std;

//default constructor
gamePlay::gamePlay()
{
    srand(time(0));

    playerCount = 0;
    teamNumberCounter = 0;

    //Set every value in the 1st column of matrix as 0
    for (int i=0; i<=PLAYERMAXNUM; i++){
        state[i][0][0]=0;
        state[i][0][1]=0;
        state[i][0][2]=0;
    }
    //Set every value in the 1st row of matrix as 0
    for (int j=0; j<=PLAYERMAXNUM; j++){
        state[0][j][0]=0;
        state[0][j][1]=0;
        state[0][j][2]=0;
    }
}

//constructor that creates desired amount of players (Calls the default constructor)
gamePlay::gamePlay(int num):gamePlay::gamePlay(){

    if(num <= 0 || num > PLAYERMAXNUM){
        exit(1);
    }
}

//default destructor
gamePlay::~gamePlay()
{
    //delete everything from multidimensional array
    for (int i=0; i<=PLAYERMAXNUM; i++){
        for (int j=0; j<=PLAYERMAXNUM; j++){
            for (int k=0; k<3; k++){}
        }
        delete[] state[i];
    }
    delete[] state;

    delete[] players;

}

//default copy constructor
gamePlay::gamePlay(const gamePlay& other)
{
    //copy all the state values from other gamePlay
    memcpy(&state[0][0][0], &other.state[0][0][0], sizeof(other.state));
    playerCount = other.playerCount;
    teamNumberCounter = other.teamNumberCounter;
}

//play a round of game
/*PseudoCode :
 * 1. Reset the values in matrix that should be reset
 * 2. Play only if the players' ID is greater than the other players' ID to prevent running game twice
 *    (Otherwise, the same game will be played again if we get to the other player and play from his point of view)
 * 3. When we are done iterating through the players, update the matrix
 */
 int gamePlay::playRound(int detail, ofstream& printer, int teamPlay){

    //If we have fewer than two players, we can't play the game
    if(playerCount <= 1)
        return 1;

     // Set all z=1 values of first row and first column to 0, since it keeps track of
     // the total result one achieved in the round
    for(int i=1; i<=playerCount;i++){
        state[0][i][1]=0;
        state[i][0][1]=0;
    }
    //keeps track of total number of rounds
    state[0][0][0] = state[0][0][0] + 1;
    if(detail == 1)
        printer << "Round : " << state[0][0][0] << endl;


    // Make all combinations of players play each other
    for(int i=1; i<=playerCount; i++){
        player* currentPlayer = players[i];
        for(int j=1; j<i; j++){
            player* opponentPlayer = players[j];
            playPair(currentPlayer,opponentPlayer, detail, printer, teamPlay);
        }
    }
    if(detail == 1)
        printer << "-------------------------------------------------\n";

    return 0;
 }

//play the game between two players (Returns 0 if successful)
/*PseudoCode :
 * 1. Check what second player did for the last game against first player.
 * 2. Using that result, first player makes a move.
 * 3. Check what first player did for the last game against second player.
 * 4. Using that result, second player makes a move.
 * 5. Determine the outcome, update the matrix.
 */
int gamePlay::playPair(player* first, player* second, int detail, ofstream& printer, int teamPlay){


    int firstID = first->getID();
    int secondID = second->getID();

    int firstLastMove = getLastPlay(secondID, firstID);
    int secondLastMove = getLastPlay(firstID, secondID);

    bool firstPlayerChoice;
    bool secondPlayerChoice;
    //Determine the teams
    if(teamPlay){
        determineTeam(first, firstLastMove, second, secondLastMove, detail, printer);

        // If two players are in the same team, by 90% probability one collaborate
        if(first->getTeam() == second->getTeam() && first->getTeam() != 0){
            int firstRand = rand() % 101;
            int secondRand = rand() % 101;
            if(firstRand >= 90){
                firstPlayerChoice = 0;
            }else{
                firstPlayerChoice = 1;
            }
            if(secondRand >= 90){
                secondPlayerChoice = 0;
            }else{
                secondPlayerChoice = 1;
            }
        }else{ // If they are in different teams, just play normally
            firstPlayerChoice = first->play(*second, secondLastMove);
            secondPlayerChoice = second->play(*first, firstLastMove);
        }
    }else{ // If there is no teamplay, compute normally
        firstPlayerChoice = first->play(*second, secondLastMove);
        secondPlayerChoice = second->play(*first, firstLastMove);
    }
    //Compute outcome for each player
    int firstPlayerResult = gameMatrix[firstPlayerChoice][secondPlayerChoice];
    int secondPlayerResult = gameMatrix[secondPlayerChoice][firstPlayerChoice];

//UNCOMMENT THIS TO GET MORE INFO

if(detail){
printer << "Player " << firstID << " and Player " << secondID << " are now playing." << endl;
printer << "Last time, Player " << secondID;
if(secondLastMove){printer << " cooperated";}else{printer << " defected";}
printer << " and Player " << firstID;
if(firstLastMove){printer << " cooperated.\n";}else{printer << " defected.\n";}
printer << "Player " << firstID << " chose to ";
if(firstPlayerChoice){printer << "cooperate";}else{printer << "defect";}
printer << " and Player " << secondID << " chose to ";
if(secondPlayerChoice){printer << "cooperate.\n";}else{printer << "defect.\n";}
printer << "Player " << firstID << " earned " << firstPlayerResult;
printer << " and Player " << secondID << " earned " << secondPlayerResult << endl<<endl;
}

    //update the result of the gameplay for both players in the matrix
    state[firstID][secondID][0]=secondPlayerChoice;
    state[firstID][secondID][1]=firstPlayerResult;
    state[firstID][secondID][2]=(state[firstID][secondID][2])
        +firstPlayerResult;

    state[secondID][firstID][0]=firstPlayerChoice;
    state[secondID][firstID][1]=secondPlayerResult;
    state[secondID][firstID][2]=(state[secondID][firstID][2])
        +secondPlayerResult;

    /*update the row totals of the matrix
     *Note : [i][0][0] means all the cooperations user i received this round
     *       [i][0][1] means all the outcome user i received in this round
     *       [i][0][2] means all the outcome user i received until now
     */
    state[firstID][0][0] += secondPlayerChoice;
    state[firstID][0][1] += firstPlayerResult;
    state[firstID][0][2] += firstPlayerResult;
    state[secondID][0][0] += firstPlayerChoice;
    state[secondID][0][1] += secondPlayerResult;
    state[secondID][0][2] += secondPlayerResult;

    /*update the column totals of the matrix
     *Note : [0][i][0] means number of cooperations user i did (# of cooperations others received) this round
     *       [0][i][1] means all the outcome others (those that user i played) received in this round
     *       [0][i][2] means all the outcome others (those that user i played) received until now
     */
    state[0][firstID][0] += firstPlayerChoice;
    state[0][firstID][1] += secondPlayerResult;
    state[0][firstID][2] += secondPlayerResult;

    state[0][secondID][0] += secondPlayerChoice;
    state[0][secondID][1] += firstPlayerResult;
    state[0][secondID][2] += firstPlayerResult;
    return 0;
}

//Get the last action that other player has performed when playing thisPlayer
//Assumption : thisPlayer and otherPlayer had been declared and still exist
int gamePlay::getLastPlay(int thisPlayer, int otherPlayer){
    return state[thisPlayer][otherPlayer][0];
}


/*
 * Determines whether two players form a team or not
 * 1. Neither of them are in a team
 *    - prob : 0 -
 *           => Difference of Reputation(10%): the smaller the better
 *           => Past outcome (10% for each)
 *           => Distance (10%)
 *           => Random Probability (50%) : random probability between 0 and 10
 * 2. If one is in a team but other is not (at maximum there is 50% probability to "support" him)
 *    - Prob : 0 -
 *           => Reputation (10%) : the higher the other's reputation, higher probability to form a team
 *           => Past Outcome (10%) : if other defected last time, lower probability
 *           => Distance (10%) : closer one is, higher probability to form a team
 *           => Random Probability (70%) : random probability between 0 and 20
 * 3. Checks if they are in same team
*     - Prob : 0 -
 *          =>With some low probability, break the team depending on :
 *           => Distance (5%) : the closer, the higher probability to stay on same team
 *           => Past outcome (30%) : if the one defected last time
 *           => Reputation (5%) : the higher the sum of reputation, the lower the probability one breaks
 *           => Random probability (60%) : always set as 0 => if we have a team, rather not break it
 * 4. Both are in a team, but different teams
  *   - Prob : 0 -
 *           => By low probability, both break out of respective teams. (5%) = > who knows, might be doing startup
 *
 */
void gamePlay::determineTeam(player *A, int lastAMove, player *B, int lastBMove, int detail, ofstream& printer){
    int determinant;
    int rand_prob = rand() % 100 + 1;
    // Case 1 :both are not in a team
    if(A->getTeam() == 0 && B->getTeam() == 0){
        int dif_of_rep = 100-abs(A->getReput() - B->getReput()); //between 0 and 100
        int past_outcome = lastAMove * 100 + lastBMove * 100; // between 0 and 200
        int dif_of_dist = A->measureDist(*B); // between 0 and 100
        int rand_num = rand() % 101; // between 0 and 10
        determinant = dif_of_rep + past_outcome + dif_of_dist + rand_num; // between 500 and 0;
        determinant = determinant / 50; // Max prob = 10%
        if(determinant >= rand_prob){ // form a team
            A->setTeam(teamNumberCounter);
            B->setTeam(teamNumberCounter);
            if(detail){
                printer << "Player " << A->getID() << " and Player " << B->getID() << " joined to create team " << teamNumberCounter << endl;
                cout << "Player " << A->getID() << " and Player " << B->getID() << " joined to create team " << teamNumberCounter << endl;
            }
            teamNumberCounter++;
            if(teamNumberCounter == playerCount){ // Reset the number depending on number of players
                teamNumberCounter = 0;
            }
        }
    //Case 2 :  If one is in a team but other is not (at maximum there is 50% probability to "support" him)
    }else if(A->getTeam() ==0){
        int rep = B->getReput(); // between 0 and 100
        int outcome = lastBMove * 100; // between 0 and 100
        int dif_of_dist = A->measureDist(*B); // between 0 and 100
        int rand_num = rand() % 201; // between 0 and 200
        determinant = rep + outcome + dif_of_dist + rand_num; // between 0 and 500
        determinant = determinant / 100; // max prob = 5%
        if(determinant >= rand_prob){
            A->setTeam(B->getTeam());
            if(detail){
                printer << "Player " << A->getID() << " joined Player B's team " << B->getTeam() << endl;
                cout <<"Player " << A->getID() << " joined Player B's team " << B->getTeam() << endl;
            }
        }
    }else if(B->getTeam()==0){
        int rep = A->getReput(); // between 0 and 100
        int outcome = lastAMove * 100; // between 0 and 100
        int dif_of_dist = B->measureDist(*A); // between 0 and 100
        int rand_num = rand() % 201; // between 0 and 200
        determinant = rep + outcome + dif_of_dist + rand_num; // between 0 and 500
        determinant = determinant / 100; // max prob = 5%
        if(determinant >= rand_prob){
            B->setTeam(A->getTeam());
            if(detail){
                printer << "Player " << B->getID() << " joined Player A's team " << A->getTeam() << endl;
                cout << "Player " << B->getID() << " joined Player A's team " << A->getTeam() << endl;
            }
        }
    // Case 3 : In same team
    }else if (A->getTeam() == B->getTeam()){
        int dif_of_dist = B->measureDist(*A)/2; // between 0 and 50
        int rep_sum = 50 - ((A->getReput() + B->getReput())/4); // between 0 and 50
        int outcome = (2-lastAMove-lastBMove)*150; // between 0 and 300
        determinant = dif_of_dist + rep_sum +outcome; // between 0 and 400
        determinant = determinant / 100; // max prob = 4%
        if(determinant >= rand_prob){ // break up the team
            if(detail){
                printer << "Player " << A->getID() << " and Player " << B->getID() << " decided to break out of team " << A->getTeam() << endl;
                cout <<"Player " << A->getID() << " and Player " << B->getID() << " decided to break out of team " << A->getTeam() << endl;
            }
            A->setTeam(0);
            B->setTeam(0);
        }
    }else{
        determinant = rand() % 5 +1;
        if(determinant >= rand_prob){ // max prob = 5%
            if(detail){
                printer << "Player " << A->getID() << " left team " << A->getTeam() << " and Player " << B->getID() << " left team " << B->getTeam() << endl;
                cout <<"Player " << A->getID() << " left team " << A->getTeam() << " and Player " << B->getID() << " left team " << B->getTeam() << endl;
            }
            A->setTeam(0);
            B->setTeam(0);
        }
    }
}

//Prints the current matrix
void gamePlay::printMatrix(ofstream& printer){
    for (int i=0; i<=playerCount; i++){

        for (int j=0; j<=playerCount; j++){
            if(i == 0 && j==0){
                printer << ',' <<"Total" <<',';
                for (int k=1; k<=playerCount; k++){
                 printer << "Player " << k << ',';
                }
                printer << "\n";
            }

            if(j == 0){
                if(i==0)
                    printer << "Total" << ',';
                else
                    printer << "Player" << i << ',';
            }
            //printf("(%5d:%5d:%5d)",state[i][j][0],state[i][j][1],state[i][j][2]);
             printf("(%5d)",state[i][j][2]);
             char first[10],second[10],third[10];
            sprintf(first,"(%5d:",state[i][j][0]);
            sprintf(second,"%5d:",state[i][j][1]);
            sprintf(third,"%5d)",state[i][j][2]);
            printer << first << second << third << ',';
        }
        printer << "\n";
        printf("\n");
    }
}

//Prints the traits of each player
/*
void gamePlay::printTraits(int printToCout, ofstream &printer, int teamPlay){

    if(printToCout)
        cout << "Status of Players in generation " << players[1]->getGen() <<endl;
    printer << "Player ID, basefac,memfac,distfac,repfac,base,rep,x,y";

    if(teamPlay)
        printer <<",team\n";
    else
        printer << "\n";

    //print the traits to both console and log

    for(int i=1; i<=playerCount; i++){
            //Print this only if we are playing in rounds

        if(printToCout){
        cout << "Player " << i << " : base = " << players[i]->getTrait().getBase() <<
        ", basefac = " <<  players[i]->getTrait().getFac(1) <<
        ", memfac = " <<  players[i]->getTrait().getFac(2) <<
        ", distfac = " <<  players[i]->getTrait().getFac(3) <<
        ", repfac = " <<  players[i]->getTrait().getFac(4) <<
        ", rep = " <<  players[i]->getReput() <<
        ", x = " <<  players[i]->getTrait().getX() <<
        ", y = " <<  players[i]->getTrait().getY();

        if(teamPlay)
            cout << ", team = " << players[i]->getTeam() << endl;
        else
            cout << endl;
        }

        printer << i << "," <<

        players[i]->getTrait().getFac(1) << "," <<
        players[i]->getTrait().getFac(2) << "," <<
        players[i]->getTrait().getFac(3) << "," <<
        players[i]->getTrait().getFac(4) << "," <<
        players[i]->getTrait().getBase() << "," <<
        players[i]->getReput() << "," <<
        players[i]->getTrait().getX() << "," <<
        players[i]->getTrait().getY();

        if(teamPlay)
            printer <<","<<  players[i]->getTeam() << "\n";
        else
            printer << "\n";

    }
}
*/


//Prints the traits of each player =>for log and data
void gamePlay::printTraits(int printToCout, ofstream &printer, int teamPlay){
    if(printToCout)
        cout << "Status of Players in generation " << players[1]->getGen() <<endl;
    printer << "Status of Players in generation " << players[1]->getGen() <<endl;

    //print the traits to both console and log
    for(int i=1; i<=playerCount; i++){
            //Print this only if we are playing in rounds
        if(printToCout){
        cout << "Player " << i << " : base = " << players[i]->getTrait().getBase() <<
        ", basefac = " <<  players[i]->getTrait().getFac(1) <<
        ", memfac = " <<  players[i]->getTrait().getFac(2) <<
        ", distfac = " <<  players[i]->getTrait().getFac(3) <<
        ", repfac = " <<  players[i]->getTrait().getFac(4) <<
        ", rep = " <<  players[i]->getReput() <<
        ", x = " <<  players[i]->getTrait().getX() <<
        ", y = " <<  players[i]->getTrait().getY();
        if(teamPlay)
            cout << ", team = " << players[i]->getTeam() << endl;
        else
            cout << endl;
        }

        printer << "Player " << i << " : base = " << players[i]->getTrait().getBase() <<
        ", basefac = " <<  players[i]->getTrait().getFac(1) <<
        ", memfac = " <<  players[i]->getTrait().getFac(2) <<
        ", distfac = " <<  players[i]->getTrait().getFac(3) <<
        ", repfac = " <<  players[i]->getTrait().getFac(4) <<
        ", rep = " <<  players[i]->getReput() <<
        ", x = " <<  players[i]->getTrait().getX() <<
        ", y = " <<  players[i]->getTrait().getY();

        if(teamPlay)
            printer << ", team = " << players[i]->getTeam() <<endl;
        else
            printer << endl;
    }

}


// Create players that always perform Tit for tat
int gamePlay::addTFTPlayer(int num,ofstream& printer){
    for(int ctr=0; ctr< num; ctr++){
        playerCount++;
        if(playerCount > PLAYERMAXNUM) //additional check to prevent error
            return 1;
        trait* tftTrait = new trait;
        tftTrait->makeTitForTat();
        player* newPlayer = new player(tftTrait, 0, playerCount);
        players[playerCount] = newPlayer;
cout << "Player " << playerCount << " is a Tit-for-tat player" <<endl;
printer << "Player " << playerCount << " is a cooperative player" <<endl;
        chgMatrixForNewPlayer();
    }

    return 0;
}

// Create players that cooperate all the time
int gamePlay::addCOOPlayer(int num,ofstream& printer){
    for(int ctr=0; ctr< num; ctr++){
        playerCount++;
        if(playerCount > PLAYERMAXNUM) //additional check to prevent error
            return 1;
        trait* cooTrait = new trait;
        cooTrait->makeCooperate();
        player* newPlayer = new player(cooTrait, 0, playerCount);
        players[playerCount] = newPlayer;
cout << "Player " << playerCount << " is a cooperative player" <<endl;
printer << "Player " << playerCount << " is a cooperative player" <<endl;
        chgMatrixForNewPlayer();
    }

    return 0;
}

// Create players that always defect
int gamePlay::addDEFPlayer(int num,ofstream& printer){
    for(int ctr=0; ctr< num; ctr++){
        playerCount++;
        if(playerCount > PLAYERMAXNUM) //additional check to prevent error
            return 1;
        trait defTrait;
        defTrait.makeDefect();
        player* newPlayer = new player(&defTrait, 0, playerCount);
        players[playerCount] = newPlayer;
cout << "Player " << playerCount << " is a defective player" <<endl;
printer << "Player " << playerCount << " is a defective player" <<endl;
        chgMatrixForNewPlayer();
    }
    return 0;
}

// Create players that have random values
int gamePlay::addREGPlayer(int num,ofstream& printer){
    for(int ctr=0; ctr< num; ctr++){
        playerCount++;
        if(playerCount > PLAYERMAXNUM) //additional check to prevent error
            return 1;
        trait* regTrait = new trait;
        player* newPlayer = new player(regTrait, 0, playerCount);
        players[playerCount] = newPlayer;
cout << "Player " << playerCount << " is a regular player" <<endl;
printer << "Player " << playerCount << " is a regular player" <<endl;
        chgMatrixForNewPlayer();
    }
    return 0;
}

// Create players that have random values
int gamePlay::addCUSPlayer(int bVal, int bf, int mf, int df, int rf, ofstream& printer){
    playerCount++;
    trait* cusTrait = new trait(bVal, bf, mf, df, rf);
    player* newPlayer = new player(cusTrait, 0, playerCount);
    players[playerCount] = newPlayer;
cout << "Player " << playerCount << " is a customized player" <<endl;
printer << "Player " << playerCount << " is a customized player" <<endl;
    chgMatrixForNewPlayer();

    return 0;
}

// Adds the given player to the game
int gamePlay::addPlayer(player* newPlayer){
    playerCount++;
    player* copyPlayer = new player(*newPlayer);
    players[playerCount] = copyPlayer;
    chgMatrixForNewPlayer();

    return 0;
}

// Creates this generations' children and add the children to the next generation.
/* Implements the Fitness proportionate selection (roulette wheel selection)
 * =>Each player has a probability of being a parent that is proportionate to
 * his results in the game compared to others'.
 * For example, if A has earned 40 points, B 30, C 20, and D 10
 * A has 40% of being selected, b has 30%, etc.
 * Note : (A&A) not allowed (one cannot be father and mother of a player)
 *        (A&B), (A&C), (A&B), .... is allowed (same player can be parent to many)
 * If previous generation had n players, for next generation, n pairs of parents are selected.
 *    for each pair, there is ZEROCHILD (defined in gamePlay.h) probability of getting 0 child,
 *    ONECHILD probability of getting 1 child, and TWOCHILD probability of getting 2 children.
 * Return Values : 0 if generation was successfully changed
 *                 1 if there was only 1 in current generation -> next generation extinct
 *                 2 if after simulation, there was no one in next generation
 */
int gamePlay::genChange(gamePlay& other){
    int num=0;

    //If there is only one player, there is no next generation
    if(playerCount == 1){
        return 1;
    }

    /* Create an array of values that will represent the probability wheel.
     * The array will have cumulative values (cdf) of outcomes.
     * e.g.If A has earned 40, B 30, C 20, and D 10, array will look like following.
     * wheel : [0][40][70][90][100]
     *        NULL A   B   C   D
    */
    int wheel[playerCount+1]; // array of values that will create the probability wheel
    int sum = 0; // keep track of sum of results
    wheel[0] = 0;
    for(int i=1; i<=playerCount; i++){
        int res=state[i][0][2];
        if(res<=0)
            res =1;
        sum += res;

        wheel[i]=sum;
    }

    //Keep running and select n number of parents.
    while((num < playerCount) && (other.getPlayerCount() < PLAYERMAXNUM)){

        int father = rand() % sum + 1;
        //Choose the first Parent
        for(int i=1; i<=playerCount; i++){
            if(father > wheel[i-1] && father <= wheel[i]){
                father = i;
                break;
            }
        }

        //choose the second parent that is different from the first parent
        int mother;
        do{
            mother = rand()%sum +1;
            for(int i=1; i<=playerCount; i++){
                if(mother > wheel[i-1] && mother <= wheel[i]){
                    mother = i;
                    break;
                }
            }
        } while(father == mother); // keep doing until we have a father different from mother.

        // calculate the number of children depending on the chosen probability
        int numChild = rand()%100+1;

        if(numChild <= ZEROCHILD){
            numChild = 0;
        }else if (numChild > ZEROCHILD && numChild <= ZEROCHILD + ONECHILD){
            numChild = 1;
        }else{
            numChild = 2;
        }

        //Add the child player to the other list
        for(int i=0;i<numChild;i++){
            if(other.getPlayerCount()<PLAYERMAXNUM){
                player* childPlayer=(players[father]->reproduce(*(players[mother]),other.getPlayerCount()+1));
                other.addPlayer(childPlayer);
            }
        }

        num++;
    }

    //If we have fewer or equal to 0, return 2
    // This will indicate that next generation is extinct
    if(other.getPlayerCount() == 0){
        return 2;
    }

    return 0;
}



//Initialize the matrix's new portion for the new player
void gamePlay::chgMatrixForNewPlayer(){
        //Set the initial action values for this player as 1
        for (int i=1; i<=playerCount; i++){
            state[i][playerCount][0]=1;
            state[i][playerCount][1]=0;
            state[i][playerCount][2]=0;
        }

        // Set initial action values of other players as 1
        for (int j=1; j<=playerCount; j++){
            state[playerCount][j][0]=1;
            state[playerCount][j][1]=0;
            state[playerCount][j][2]=0;
        }
}

// Returns the current number of players
int gamePlay::getPlayerCount(){
    return playerCount;
}

// Returns the current generation of this game
int gamePlay::getGen(){
    return players[1]->getGen();
}

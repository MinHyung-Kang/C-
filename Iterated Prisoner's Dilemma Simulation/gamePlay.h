#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iosfwd>
#include <fstream>
#include "player.h"
#include "trait.h"
/*
 * gamePlay.h
 *
 *  Created on: Feb 15, 2015
 *  Author: Daniel Kang & Yachen Sun
 *  Description: header file for gamePlay, which will handle all the actions of the game
 *
 */

/*Data structure : 3 dimensional array.
 *         ---------------------
 *         |   |   |   |   |   |  2 : total amount of benefit/damage i received
 *         ---------------------
 *         |   |   |   |   |   |  1 : amount of benefit/damage i received in this round
 *         ---------------------
 *         |   |   |   |   |   |  0 : Past action of player j when i and j played (1 = cooperate, 0=defect)
 *         ---------------------
 *      0 /   /   /   /   /   /
 *       ---------------------       *Note that i coordinate of (i,j) is the subject.
 *    1 /   /   /   /   /   /         That is, while (2,3) and (3,2) both save gameplay between player 2 and 3
 *     ---------------------         (2,3) focuses more on player 2. It will store what player 3 did in last game
 *  2 /   /   /   /2,3 /   /          (since player 2 cares about player 3's past play) and sum of all outcomes for player 2.
 *   ----------------------          * 0 row and column represents the sum of values for each row and column
 *     0  1   2   3   4    <-> j      => (0,1) will show sum of all the outcomes for players who played player 1
 *                                    => (1,0) will show sum of all the outcomes for player 1
 *
 *
 *  Reference : gamePlayMatrix
 *  --------------------------------
 *  |         | Defect | Cooperate |  <= Second Player
 *  ---------------------------------
 *  |  Defect |  (0,0) |  (2,-1)   |       *Note, one can always get more by defecting, the defecting is the dominant strategy.
 *  --------------------------------
 *  |Cooperate| (-1,2) |   (1,1)   |
 *  --------------------------------
 *    First Player
 */

const int defectCoop=2; // The result if you defect but other cooperates
const int coopDefect=-1; // The result if you cooperate but other defects
const int defectDefect=0; // The result if both defects
const int coopCoop=1; // The result if both cooperate
const int DEFAULTPLAYERNUM = 5; // By default, we start out with 10 players
const int PLAYERMAXNUM = 100; // the maximum number of players in a game
const int ROUNDSPERGEN = 50; // Number of rounds per generation
const int ZEROCHILD = 2; // The probability (out of 100) to get no child
const int ONECHILD = 95; // The probability (out of 100)to get one child
const int TWOCHILD = 3; // The probability (out of 100) to get two children
const int gameMatrix[2][2]={{defectDefect, defectCoop},{coopDefect,coopCoop}}; //Determine outcomes => [firstPlayer play][secondPlayer play] returns outcome for first player
                                                  //gameMatrix[0][1] : returns outcome for first player if firstplayer defects and other cooperates

class gamePlay
{
    public:
        gamePlay(); // default constructor
        gamePlay(int num); // constructor for which one can set the number of players
        ~gamePlay(); //default destructor
        gamePlay(const gamePlay& other); //copy constructor
        int playRound(int detail, ofstream& printer, int teamPlay); // Plays a round of game
        int playPair(player* first, player* second, int detail, ofstream& printer, int teamPlay); //Plays game between two players
        int getLastPlay(int thisPlayer, int otherPlayer); //Return the last move of the other player
        void printMatrix(ofstream &printer); //prints out the current statematrix
        void printTraits(int printToCout, ofstream &printer, int teamPlay); //prints out the current traits of the players
        int addTFTPlayer(int num,ofstream &printer); // Add desired amount of Tit for tat players
        int addCOOPlayer(int num,ofstream &printer); // Add desired amount of cooperative players
        int addDEFPlayer(int num,ofstream &printer); // Add desired amount of defective players
        int addREGPlayer(int num,ofstream &printer); // Add desired amount of regular players
        int addCUSPlayer(int bVal, int bf, int mf, int df, int rf,ofstream &printer); // Add one customized player
        int addPlayer(player* newPlayer); // Add one given player
        int genChange(gamePlay& other); // perform generation change
        int getPlayerCount(); // returns the current Player count
        int getGen(); // returns the current generation

    protected:
    private:
        int playerCount; // Keeps track of total number of players
        int teamNumberCounter; // Keeps track of team numbers
        player* players[PLAYERMAXNUM+1]; // List of all the players in the game (starts from 1)
        int state[PLAYERMAXNUM+1][PLAYERMAXNUM+1][3] ;  //matrix that will save all the players the game
        void chgMatrixForNewPlayer(); // change the matrix's new values for newly created player
        void determineTeam(player* A,int lastAMove,player* B, int lastBMove, int teamPlay, ofstream& printer); // determines whether two form a team or not
};

#endif // GAMEPLAY_H

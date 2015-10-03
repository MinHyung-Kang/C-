#ifndef PLAYER_H
#define PLAYER_H


/*
 * player.h
 *
 *  Created on: Feb 14, 2015
 *      Author: Daniel Kang & Yachen Sun
 */

#include <iostream>
#include "trait.h"
#include <math.h>
using namespace std;

const int reputIncrement = 5;   // controls how fast reputation values change, can be adjusted
const int maxChildParentSep = 10;// controls the maximum distance that a child can be located from one of
								// its parents (in terms of x and y, not Euclidean distance)
const int maxEvalDist = 500;
const int mut = 20; 			// percentage mutation rate
const int crossover = 20;		// percentage crossover rate

class player {
	trait mytrait;
	int reput;  		// keep track of reputation of player, between 1 and 100
	int id; 			// unique id for identifying player within current generation
	int gen;			// keep track of the generation of the player (index from 0)
    int team;           // defines the team of the player : 0 by default
	int increaseReput();		// -|
	int decreaseReput();		// -|==> these two methods can only be called by the play method

public:
	player();
	player(trait*, int, int);	// trait, generation, ID
	player(player&);
	~player();

	trait getTrait();
	int getReput();
	int getID();
	int getGen();
    int getTeam();
    void setTeam(int newTeam);

	int measureDist(player);	// generate a metric to evaluate the Euclidean distance to another player

	bool play(player, int);   	// inputs: the other player, the last action from the other player
								// result: 1 for cooperate, 0 for defect

	player* reproduce(player, int); 	// generate a new player from two parent players;
										// the ID of the new player should be given to the method

};

#endif // PLAYER_H

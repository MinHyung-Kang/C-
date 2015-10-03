/*
 * player.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: Daniel Kang & Yachen Sun
 */

#include <stdio.h>
#include "player.h"
#include "trait.h"

// default constructor
// reputation is always 50 at start
player::player(){
	trait mytrait;
	reput = 50;
	gen = 0;
	id = 0;
	team = 0;
}

// constructor
// reputation is always 50 at start
player::player(trait* t, int GEN, int ID){
	mytrait = trait(*t);
	reput = 50;
	gen = GEN;
	id = ID;
	team = 0;
}

// copy constructor
player::player(player& p){
    mytrait = trait(p.getTrait());
	reput = p.reput;
	gen = p.gen;
	id = p.id;
	team = p.team;
}

// destructor
player::~player(){}

// increase reputation by a constant increment
// this method is declared private to ensure that the reputation value is not changed
// outside game play conditions
int player::increaseReput(){
	reput = reput + reputIncrement;
	if (reput >= 100) {
		reput = 100;
	}

	return reput;
}

// increase reputation by a constant increment
// this method is declared private to ensure that the reputation value is not changed
// outside game play conditions
int player::decreaseReput(){
	if (reput > reputIncrement) {
		reput = reput - reputIncrement;
	}
	else {
		reput = 1;
	}
	return reput;
}

trait player::getTrait() {
	trait tmp(mytrait);
	return tmp;
}

int player::getReput(){
	int tmp = reput;
	return tmp;
}

int player::getID(){
	int tmp = id;
	return tmp;
}

int player::getGen(){
    return gen;
}

int player::getTeam(){
    return team;
}

void player::setTeam(int newTeam){
    team = newTeam;
}

// Use this method to calculate the Euclidean distance between two players
// and map the distance onto a metric on a scale of 0 to 100
// Mapping: if the Euclidean distance between two players is smaller than 500,
// the larger the distance, the smaller the metric;
// if the Euclidean distance between two players is larger than 500,
// the metric is 0
int player::measureDist(player B){
	double eudist = sqrt(pow(double(mytrait.getX()-B.mytrait.getX()), 2)+ pow(double(mytrait.getY()-B.mytrait.getY()), 2));
	if (eudist > maxEvalDist) {
		return 0;
	}
	else {
		return 100 - eudist/maxEvalDist;
	}
}

// Used to calculate outcome of a game play
// inputs: the other player, other player's last move
// result: 1 for cooperate, 0 for defect
bool player::play(player B, int move){
	int prob; // probability of cooperation
	int dist = measureDist(B);
	prob = mytrait.getFac(1)*mytrait.getBase()+mytrait.getFac(2)*move*100+mytrait.getFac(3)*dist+mytrait.getFac(4)*B.reput;
	prob = prob / (mytrait.getFac(1)+mytrait.getFac(2)+mytrait.getFac(3)+mytrait.getFac(4));
	int x = rand() % 100 + 1;

	//printf("Player %d against player %d, player %d's prob=%d, x=%d\n", this->id,B.id,this->id,prob,x);
	if (x <= prob) {
		this->increaseReput();
		return 1;
	}
	else {
		this->decreaseReput();
		return 0;
	}
}


// generate new player from two parent players
// the four trait factors of the new player will be the average of the two parents' trait factors
// the x, y coordinate of the new player will be within a 10*10 (+-5 each coordinate) square of a random parent
// inputs: the other parent player, the ID assigned to the new player
player* player::reproduce(player B, int ID) {
	// randomly generate the x, y coordinate within +-5 each coordinate of one of the parents
	int x,y;
	int parent = rand()%2; 	// decide which parent's vicinity the child will be based on
	int signs[2] = {rand()%2,rand()%2};	// this decides whether to add to or subtract from the parent's coordinates
										// there are two possible values for each element: 0, 1
										// thus, 0*2-1 = -1; 1*2-1 = 1; we can use signs[i]*2-1 to randomize the sign
	int delta[2] = {rand()%maxChildParentSep, rand()%maxChildParentSep};
	if (parent) {  			// generate coordinates in the vicinity of this instance
		x = mytrait.getX() + (signs[0] * 2 - 1) * delta[0];
		y = mytrait.getY() + (signs[1] * 2 - 1) * delta[1];
	}

        else {
			x = B.mytrait.getX() + (signs[0] * 2 - 1) * delta[0];
			y = B.mytrait.getY() + (signs[1] * 2 - 1) * delta[1];
        }
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	// decide factors based on crossover or not
	// not crossover: take player A's factor
	// crossover: take player B's factor
	int baseprob = mytrait.getBase();
	int tmp = rand()%100;
	if (tmp < crossover) {
		baseprob = B.mytrait.getBase();
	}
	int factors[4];
	for (int i = 0; i < 4; i ++) {
		factors[i] = mytrait.getFac(i+1);
		tmp = rand()%100;
		if (tmp < crossover) {
			factors[i] = B.mytrait.getFac(i+1);
			if (factors[i]==0){factors[i]=1;}
		}
	}
	int GEN = gen + 1;		// generation number of child is generation number of parent + 1
	trait* newtrait= new trait(baseprob, factors[0],factors[1],factors[2],factors[3],x,y);
	// perform random mutation
	newtrait->mutate(mut);
	player* newplayer = new player(newtrait, GEN, ID);
	return newplayer;
}



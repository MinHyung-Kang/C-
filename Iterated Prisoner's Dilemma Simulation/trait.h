#ifndef TRAIT_H
#define TRAIT_H


/*
 * trait.h
 *
 *  Created on: Feb 14, 2015
 *  Author: Daniel Kang & Yachen Sun
 *  Description: Contains characteristics that explain the behavior of the player.
 *
 *  Trait--------basefac (1~100): describes the "innate probability" that one cooperates
 *        |
 *        -------memfac  (1~100): describes how much past gameplays affect the decision of the player
 *        |
 *        -------distfac (1~100): describes how important the distance between the players is
 *        |                       in making decision to cooperate
 *        -------repfac  (1~100): describes how much others' reputation (good or bad) affect the decision
 *        |                of the player
 *        ------- x,y :  (1~1000): describes the location of the player
 *        |
 *        ------- gen : (1 ~ ) : describes the generation of this player. Default value is 1.
 *
 *
 */

#include <ctime>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

const int FACTORDIVISOR = 100; // Constant used to randomly generate numbers for factors
const int LOCDIVISOR = 1000;  // Constant used to randomly generate numbers for x and y
const int MAXMUT = 20;

class trait{
private:
    // inborn probability of cooperation
    // between 0 and 100
    int base;
	// weights associated with each characteristic
	// between 0 and 100
	int basefac, memfac, distfac, repfac;
	// coordinates
	// between 1 and 1000
	int x, y;

public:
	trait();          // default constructor
	trait(int, int, int, int, int);//constructor that assigns all values except location
	trait(int, int, int, int, int, int, int);//constructor that assigns values
    trait(const trait&);//copy constructor
	~trait();         // default destructor

    int getBase();    // returns the base cooperation probability of the player
	int getX();       // returns the xcoordinate of the player
	int getY();       // returns the ycoordinate of the player
	int getFac(int);  // returns basefac if param=1, memfac if param=2, distfac if param=3, repfac if param=4
                      // returns -1 for any other param
    int setFac(int, int); //first int is the choice which specifies the factor to modify
                          //second int is the new value
                          // 1: basefac 2: memfac 3: distfac 4: repfac
                          // returns 0 on success, -1 if no value was changed

    //Functions that might be used
    void move(int, int); // move the player by the amount
    void makeTitForTat(); // make the player perform 100% tit for tat
    void makeCooperate(); // make the player cooperate 100%
    void makeDefect(); // make the player defect 100%
    void mutate(int);  // mutate the five factors according to the mutation rate
};
#endif // trait

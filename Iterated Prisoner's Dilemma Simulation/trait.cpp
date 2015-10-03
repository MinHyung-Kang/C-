/*
 * trait.cpp
 *
 *  Created on: Feb 14, 2015
 *  Author: Daniel Kang & Yachen Sun
 *  Description: Implements trait class
 *      Includes variables that explain the behavior of the players
 */

#include "trait.h"

//default constructor that randomly assigns the values of factors
trait::trait()
{
    //the innate probability that one cooperates is between 0 and 100
    base = rand()%(FACTORDIVISOR+1);

    //randomly assign values for the importance of each factor
    basefac = rand()%FACTORDIVISOR + 1;
    memfac = rand()%FACTORDIVISOR + 1;
    distfac = rand()%FACTORDIVISOR + 1;
    repfac = rand()%FACTORDIVISOR + 1;

    //x and y values are between 1 and 1000
    x = rand()%LOCDIVISOR + 1;
    y = rand()%LOCDIVISOR + 1;
}

//Constructor that takes in 5 parameters and assigns each value
trait::trait(int bVal, int bf, int mf, int df, int rf){
    base = bVal;
    basefac = bf;
    memfac = mf;
    distfac = df;
    repfac = rf;

    //x and y values are between 1 and 1000
    x = rand()%LOCDIVISOR + 1;
    y = rand()%LOCDIVISOR + 1;
}

//Constructor that takes in 7 parameters and assigns each value
trait::trait(int bVal, int bf, int mf, int df, int rf, int xVal, int yVal){
    base = bVal;
    basefac = bf;
    memfac = mf;
    distfac = df;
    repfac = rf;
    x = xVal;
    y = yVal;
}

//destructor : no action necessary at the moment
trait::~trait(){}

//copy constructor : copies the value of other trait
trait::trait(const trait& other)
{
    base = other.base;
    basefac = other.basefac;
    memfac = other.memfac;
    distfac = other.distfac;
    repfac = other.repfac;
    x = other.x;
    y = other.y;
}

// returns the base cooperative probability of the player
int trait::getBase(){
    return base;
}

// returns the xcoordinate of the player
int trait::getX(){
    return x;
}

// returns the ycoordinate of the player
int trait::getY(){
    return y;
}

/* depending on the parameter, returns a different factor
 * 1 : basefac
 * 2 : memfac
 * 3 : distfac
 * 4 : repfac
 * others : -1
*/
int trait::getFac(int choice){
    //since we always return, no need for break
    switch(choice){
    case 1:
        return basefac;
    case 2:
        return memfac;
    case 3:
        return distfac;
    case 4:
        return repfac;
    default:
        return -1;
    }
}

/* depending on the first parameter, changes different factor
 * 1 : basefac = newVal  --|
 * 2 : memfac  = newVal    |--- returns 0;
 * 3 : distfac = newVal    |
 * 4 : repfac  = newVal  --|
 * others : do nothing --- returns -1;
*/
int trait::setFac(int choice, int newVal){
    //since we always return, no need for break
    switch(choice){
    case 1:
        basefac = newVal;
        return 0;
    case 2:
        memfac = newVal;
        return 0;
    case 3:
        distfac=newVal;
        return 0;
    case 4:
        repfac=newVal;
        return 0;
    default:
        return -1;
    }
}

//Moves the player by the given amount.
//If the player goes out of the boundary, make the player stay within the boundary
void trait::move(int deltaX, int deltaY){
    x = x + deltaX;
    if(x >= LOCDIVISOR)
        x = LOCDIVISOR;
    else if(x <= 1)
        x = 1;

    y+=deltaY;
    if(y >= LOCDIVISOR)
        y = LOCDIVISOR;
    else if(y <= 1)
        y = 1;
}

//Makes the player 100% TitForTat
void trait::makeTitForTat(){
    base = 50;
    basefac = 0;
    memfac = 100;
    distfac = 0;
    repfac = 0;
}

//Makes the player cooperate all the time
void trait::makeCooperate(){
    base = 100;
    basefac = 100;
    memfac = 0;
    distfac = 0;
    repfac = 0;
}

//Makes the player defect all the time
void trait::makeDefect(){
    base = 0;
    basefac = 100;
    memfac = 0;
    distfac = 0;
    repfac = 0;
}

//Mutate five factors according to the mutation rate
void trait::mutate(int rate) {
    // deal with the four factors first
    for (int i = 1; i < 5; i++) {
        // decide whether or not to mutate the current factor
        int tmp = rand()%100;
        if (tmp < rate) { 
            // perform the mutation
            int oldVal = this->getFac(i);
            int newVal;
            int deviation = rand()%MAXMUT;
            int sign = rand()%2;
            if (sign) {
                newVal = oldVal+deviation;
                if (newVal > 100) {
                    newVal = 100;
                }
            }
            else {
                newVal = oldVal-deviation;
                if (newVal < 1) {
                    newVal = 1;
                }
            }
            this->setFac(i,newVal);
        }
    }
    // do the same thing for base
    int tmp = rand()%100;
    if (tmp < rate) { 
        // perform the mutation
        int deviation = rand()%MAXMUT;
        int sign = rand()%2;
        if (sign) {
            base = base+deviation;
            if (base > 100) {
               base = 100;
            }
        }
        else {
            base = base-deviation;
            if (base < 1) {
                base = 1;
            }
        }
    }
}



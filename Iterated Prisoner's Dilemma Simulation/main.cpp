#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iosfwd>
#include <time.h>

#include "trait.h"
#include "player.h"
#include "gamePlay.h"

#define DATADIRECTORY "outData.csv"
#define LOGDIRECTORY "outLog.txt"
#define GENLOG "GEN.csv"




/*
 *  main.cpp
 *
 *  Created on: March, 2015
 *  Author: Daniel Kang & Yachen Sun
 *  Description: Main console which user can run the program
 *
 *  Note : Before you compile the program, adjust the definitions above to change the desired file directory
 */

using namespace std;

string divider = "-------------------------------------------------";


// Prints out the current time when the log was created
const string getTime(){
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    char fullDate[60];

    strftime(fullDate,sizeof(fullDate),"%Y-%m-%d %X", &tstruct);

    return fullDate;
}


int main()
{
    //Create a logfile
    ofstream outData,outLog,outGen;
    outData.open(DATADIRECTORY);
    outLog.open(LOGDIRECTORY);
    outGen.open(GENLOG);

    outLog << "Log generated at " << getTime() << endl;


    cout << "How many players do you want in the simulation?" << endl;

    int usrPlayerNum;
    while (1){
        cout << "Input values between : 2 and " << PLAYERMAXNUM << " : ";
        cin >> usrPlayerNum;
        if(usrPlayerNum >= 2 && usrPlayerNum <= PLAYERMAXNUM)
            break;
    }
    outLog << divider << endl;
    outLog << "Total of " << usrPlayerNum << " players" << endl;
    gamePlay mainboard(usrPlayerNum);
    int tft=0, coo=0, def=0, reg=0, cus=0;

    cout << "----------------------------------------------------------------------" << endl;
    cout << "Specify your desired number of each type of players." << endl;
    cout << "Tit-for-Tat players will always perform Tit-for-tat" << endl;
    cout << "Cooperative players will always cooperate" <<endl;
    cout << "Defective players will always defect" << endl;
    cout << "Regular players will have randomized characteristics" << endl;
    cout << "Customized players will behave according to characteristics you define" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    do{
        if(tft+coo+def+reg+cus !=0){
            cout << "Please check your values. They do not add up to " << usrPlayerNum << endl;
        }
        cout << "Tit-for-tat Players : ";
        cin >> tft;
        cout << "Cooperative Players : ";
        cin >> coo;
        cout << "Defective Players : ";
        cin >> def;
        cout << "Regular Players : ";
        cin  >> reg;
        cout << "Custom Players : ";
        cin >> cus;
        cout << "---------------------------------------------------------------------" << endl;
    }while(tft+coo+def+reg+cus != usrPlayerNum);

    outLog << "Tit for Tat players : " << tft << endl;
    outLog << "Cooperative players : "  << coo << endl;
    outLog << "Defective players : " << def << endl;
    outLog << "Regular players : " << reg << endl;
    outLog << "Custom players : " << cus << endl;
    outLog << divider << endl;

    //Add the specified players
    mainboard.addTFTPlayer(tft, outLog);
    mainboard.addCOOPlayer(coo, outLog);
    mainboard.addDEFPlayer(def, outLog);
    mainboard.addREGPlayer(reg, outLog);

    int bVal, bf, mf, df, rf;

    for(int i=1; i<= cus; i++){
        while(1==1){
            cout << "Define your " <<i << " out of " << cus <<  " custom player(s)" <<endl;

            cout << "Base Value (Inborn probability of cooperation: 0-100) : ";
            cin >> bVal;
            if(bVal < 0 || bVal >100){
                cout << "Please input value between 0 and 100"<<endl;
                continue;
            }

            cout << "Base Factor (How important your inborn character is : 0-100): ";
            cin >> bf;
            if(bf < 0 || bf >100){
                cout << "Please input value between 0 and 100"<<endl;
                continue;
            }

            cout << "Memory Factor (How important others' past gameplay is : 0-100): ";
            cin >> mf;
            if(mf < 0 || mf >100){
                cout << "Please input value between 0 and 100"<<endl;
                continue;
            }

            cout << "Distance Factor (How important the distance with other is : 0-100) : ";
            cin  >> df;
            if(df < 0 || df >100){
                cout << "Please input value between 0 and 100"<<endl;
                continue;
            }
            cout << "Reputation Factor (How important the other's reputation is : 0-100) : ";
            cin >> rf;
            if(rf < 0 || rf >100){
                cout << "Please input value between 0 and 100"<<endl;
                continue;
            }

            if(bf+mf+df+rf ==0){
                cout << "At least one factor should not be 0!"<<endl;
                continue;
            }

            break;
        }
        mainboard.addCUSPlayer(bVal, bf, mf, df, rf, outLog);
    }
    outLog << divider << endl;
    mainboard.printTraits(1,outLog,0);
    outLog << divider << endl;

    int round=0;
    int choice=0;
    cout << divider <<endl;
    do{
        cout << "Do you want to simulate by Generation or Rounds?" <<endl;
        cout << "1. Generation 2. Rounds : ";
        cin  >> choice;
    }while(choice != 1 && choice !=2);

    cout << divider <<endl;


    int detail;
    do{
        cout << "Do you want to know the details of each game in your log? (0=No, 1=Yes) : ";
        cin     >> detail;
    }while(detail != 0 && detail != 1);

    int team;
    do{
        cout << "Do you want to allow collusion? (0=No, 1=Yes) : ";
        cin     >> team;
    }while(team != 0 && team != 1);

    if(choice == 1){ //Simulate by generation
        outLog << "Simulated by generation" << endl;
        int gen;
        mainboard.printMatrix(outData);
        do{
            cout << "How many generations do you want to simulate?(To quit, type in 0): ";
            cin >> gen;

            for(int i=0; i < gen; i++){


                //print out traits at 1, 20, 50, 80, 100th Generation
                if (i == 0 || i ==19 || i == 49 || i==79 || i == 99) {
                    outGen << "Generation " << i << "\n";
                    mainboard.printTraits(0,outGen,team);
                }

                cout << "\nProcessing " << mainboard.getGen() << "th generation" << endl;
                //Play set number of rounds per each generation
                for(int round=0; round < ROUNDSPERGEN; round++){
                    mainboard.playRound(0,outLog,team);
                }
                gamePlay newBoard;
                // Attempt to change new generation
                int result = mainboard.genChange(newBoard);

                outLog << "Generation change from : " << mainboard.getGen() << " to " << mainboard.getGen()+1 << endl;
                if(detail == 1){
                    mainboard.printTraits(0,outLog, team);
                    outLog << divider << endl;
                }

                //print out matrix and traits after last generation switch
                if(i == gen - 1){
                    cout << "Result of last game : " << endl;
                    outData << "\nGeneration " << mainboard.getGen() << " outcome"<<endl;
                    mainboard.printMatrix(outData);
                }

                if(result == 1 || result == 2){
                    cout << "The players are extinct at " << mainboard.getGen()+1 << "th generation." <<  endl;
                    gen = 0;
                    return 0;
                }

                mainboard = newBoard;
                if(i == gen - 1){
                  mainboard.printTraits(1,outLog,team);
                  outLog << divider << endl;
                }
            }
        }while(gen>0);

    }else{ // Simulate by rounds
    outLog << "Simulated by rounds" << endl;


        do{
            mainboard.printMatrix(outData);

            cout << "How many rounds should we play (to quit, type in 0) : ";
            cin >> round;
            if(round > 0)
                outLog << "Playing " << round << " rounds.\n" << divider << endl;
            for(int i=0;i<round;i++){
                mainboard.playRound(detail, outLog,team);

                //Uncomment this if you want the details to be printed out at cmd

                //mainboard.printTraits(1,outLog,team);
                mainboard.printTraits(0,outLog,team);
            }
            if(round > 0){
                outLog << "Played " << round << " rounds.\n" << divider << endl;
                outData << "\nPlayed " << round << " rounds." << endl;
            }

        }while(round >0);
    }

    //close the printed files
    outLog << "Log ended at " << getTime() << endl;
    outData.close();
    outLog.close();
    outGen.close();
    return 0;
}

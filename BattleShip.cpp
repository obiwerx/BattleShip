///////////////////////////////////////////////////////////////////////////
//  BattleShip.cpp                                                       //
//  BattleShip                                                           //
//                                                                       //
//  Created by Anarchy on 4/5/13.                                        //
//  Copyright (c) 2013 Tim O'Brien. All rights reserved.                 //
///////////////////////////////////////////////////////////////////////////
 
///////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify  //
//  it under the terms of the GNU General Public License as published by //
//  the Free Software Foundation, either version 3 of the License, or    //
//  (at your option) any later version.                                  //
//      								 //
//  This program is distributed in the hope that it will be useful,      //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of       //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
//  GNU General Public License for more details.                         //
//                                                                       //
//  You should have received a copy of the GNU General Public License    //
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.//
///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//              KNOWN BUGS IN THIS VERSION                  //
// 1) character validation is not working correctly - it is //
//    possible to enter a character when the program expects//
//    a int value and crash the game.  Still researching the//
//    best way to handle this.  (isdigit and isalpha)       //
//                                                          //
// 2) The #Defines's for the color codes only work in a UNIX//
//    Environment - they don't render correctly in Windows. //
//                                                          //
// 3) I have has problems with player records skipping over //
//    record 0 - I haven't been able to find a solution to  //
//    this problem when creating a new player with nothing  //
//    in the player.dat file.  I'll have to spend more debug//
//    time tracking down a solution.                        //
//                                                          //
// 4) Major trouble in the honor roll - it just appending   //
//    any records from players that complete the game rather//
//    than only publishing the top 10 scores.. etc..  I was //
//    planning something here that I just didn't have the   //
//    time to execute properly.  At least it functions some-//
//    what.  I'd like to have it format just the handle and //
//    high score here and have a comparing conditional loop //
//    that looked to see if the score belonged in the list. //
//////////////////////////////////////////////////////////////

//HEADER FILES//
#include <iostream>             //STL container for I/O
#include <string>               //Needed for String Manipulation
#include <fstream>              //Needed for File Manipulation
#include <cctype>               //Needed for Random Number Generation
#include <ctime>                //Needed for Random Number Generation


//NAMESPACES//
using namespace std;            //Standard Template Libary Namespace


//GLOBAL CONSTANTS//
const int seaRows = 10;         //Sets up the Rows for battlefield and target array
const int seaColumns = 10;      //Sets up the Columns for battlefield and target array
const int firstSize = 15;       //size of players first name
const int lastSize = 15;        //size of players last name
const int handleSize = 15;      //size of handle or GamerTag


//DEFINITIONS//                 //Utilizing define Macros to build colors for console (UNIX ONLY!!)
#define RESET   "\033[0m"       //*Reset to default*/
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */


//STRUCTURES//
struct Player {                         //Player Structure
    char firstName[firstSize];          //First name of player
    char lastName[lastSize];            //last name of player
    char handle[handleSize];            //handle or gamertag
    int score;                          //current high score
};

struct  Ship {                          //Ship Structure
    char name[firstSize];               //Char array to hold Ship name
    int size;                           //how many elements ship occupies in array
    int hitPoints;                      //damage ship can sustain before sinking
    int identifier;                     //number displayed on targeting grid to ID shiptype
    bool isSunk;                        //is the ship sunk or floating still (is HP > 0)
    int pointValue;                     //number of points earned for sinking ship
};



//-----------------------------------------------------------------------------------------------------------------------//
//FUNCTION PROTOTYPES                                                                        DEVELOPMENT STATUS          //
//-----------------------------------------------------------------------------------------------------------------------//
//THESE FUNCTIONS ARE FOR PLAYER RECORD SLECTION AND CREATION//
Player displayRoster(int&, int&);                                                                   //Complete
Player chooseOpponent(bool&, int&, int&);                                                           //Complete
Player playerMenu(int&, int&, int&);                                                                //Complete
Player opponentMenu(int, bool&, int&, int&);                                                        //Complete
Player selectPlayer(int&, int&);                                                                    //Complete
Player createPlayerAI(char[firstSize],  char[firstSize]);                                           //Complete
Player createPlayer(int &, int&);                                                                   //Complete

//-----------------------------------------------------------------------------------------------------------------------//
//THESE FUNCTIONS ARE THE INTIAL GAME MENUS AND OPTIONS//
void menuLoop(int&, int&, int&);                                                                    //Complete
void startGame(int&, int&, int&);                                                                   //Complete
void displayRules();                                                                                //Complete
void displaySplash();                                                                               //Complete
void displayRules();                                                                                //Complete
int showMenu();                                                                                     //Complete
void showHighScores();                                                                              //Complete

//-----------------------------------------------------------------------------------------------------------------------//
//THESE FUNCTIONS ARE FOR SHIP PLACEMENT AND GAME SETUP//
void placeShipsPlayer(Player,Ship, Ship, Ship, Ship, Ship, int[seaRows][seaColumns]);               //Complete
void placeShipsAI(int grid[seaRows][seaColumns], Player, Ship, Ship, Ship, Ship, Ship);             //Complete
void drawBattleField(int battleField[seaRows][seaColumns], Player);                                 //Complete
void drawTargetField(int battleField[seaRows][seaColumns], Player, int&);                           //Complete
Ship createShip(Ship, char name[firstSize], int, int, int, bool);                                   //Complete
void buildFleet(Ship&, Ship&, Ship&, Ship&, Ship&);                                                 //Complete
void deployShip(Ship, char, int, int, int[seaRows][seaColumns]);                                    //Complete

//-----------------------------------------------------------------------------------------------------------------------//
//THESE FUNCTIONS ARE FOR PRIMARY GAME LOGIC AND MAIN GAME LOOP//
void gameLoop(Player, int&, Player, int&, bool&);                                                   //Complete
bool assessGameState(Player, Ship, Ship, Ship, Ship, Ship);                                         //Complete
bool firingOrdersAI(Player, int&, int&, int [seaRows][seaColumns], int [seaRows][seaColumns],
                    Ship&, Ship&, Ship&, Ship&, Ship&, int&, int&);                                 //Complete
bool firingOrdersPlayer(Player, int&, int[seaRows][seaColumns],int[seaRows][seaColumns],
                        Ship&, Ship&, Ship&, Ship&, Ship&);                                         //Complete
bool determineHits(Player,int, int, int[seaRows][seaColumns],int[seaRows][seaColumns],
                   Ship&, Ship&, Ship&, Ship&, Ship&, int&);                                        //Complete
int calculateScoring(bool, Ship, Ship, Ship, Ship, Ship);                                           //Complete
void displayScores(int&, Player, int, int&, Player, int);                                           //Complete
int isShipSunk(Ship);                                                                               //Complete
void writeScores(int&, Player, int, int&, Player, int);                                             //Complete
void honorScores(int&, Player, int, int&, Player, int);                                             //Complete

//-----------------------------------------------------------------------------------------------------------------------//
//THESE FUNCTIONS ARE FOR INPUT VALIDATION AND CORRECTION//
char validateDirection(char);                                                                       //Complete             
int validateTargetCol(int);                                                                         //Complete             
char validateColumnChar(char);                                                                      //Complete              
int validateMainMenuChoice(int);                                                                    //Complete
int validatePlayerMenuChoice(int);                                                                  //Complete
bool validatePlacement(Ship, int, int, char, int[seaRows][seaColumns]);                             //Complete
int validateTargetRow(int);                                                                         //Complete

//-----------------------------------------------------------------------------------------------------------------------//
//THESE FUNCTIONS ARE GENERALIZED SUPPORT FUNCTIONS//
long byteNum(int);                                                                                  //Complete
int convertAlpha(char);                                                                             //Complete
void drawBuffer();                                                                                  //Complete

//-----------------------------------------------------------------------------------------------------------------------//


            /////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////PROGRAM SOURCE CODE STARTS HERE - MAIN FIRST FOLLOWED BY SUPPORT FUNCTIONS//////////////
            /////////////////////////////////////////////////////////////////////////////////////////////////////


//MAIN FUNCTION//

int main()
{
    int rec1 = 0;       //reference variable holder for p1's record.  Needed for scoring.
    int rec2 = 0;       //reference variable holder for p2's record.  Needed for scoring.
    int maxRec = 0;     //for new user creation
    
    srand (time_t(NULL));       //seed random numbers based on system time (psuedorandomess).
    
    displaySplash();            //show kick-butt ASCII art splash screen.
    
    menuLoop(rec1, rec2, maxRec);       //invoke menu
    
    return 0;                   //obligatory return.
}


                        ////////////////////////////////////////////////////////////////////
                        //// FUNCTIONS BELOW THIS LINE ARE CALLED BY MAIN OR BY ANOTHER ////
                        //// FUNCTION - THIS INCLUDES AI ROUTINES AND PLAYER FUNCTIONS  ////
                        ////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void displaySplash()        //just a little color and 'graphics' for showmanship
{
    cout << "\n\n\n";
    cout << RED << "***************************************************************************************************" << RESET << "\n";
    cout << CYAN << ".______       ___   .___________.___________. __       _______     _______. __    __   __  .______ \n";
    cout << "|   _  \\     /   \\  |           |           ||  |     |   ____|   /       ||  |  |  | |  | |   _  \\\n";
    cout << "|  |_)  |   /  ^  \\ `---|  |----`---|  |----`|  |     |  |__     |   (----`|  |__|  | |  | |  |_)  |\n";
    cout << "|   _  <   /  /_\\  \\    |  |        |  |     |  |     |   __|     \\   \\    |   __   | |  | |   ___/\n";
    cout << "|  |_)  | /  _____  \\   |  |        |  |     |  `----.|  |____.----)   |   |  |  |  | |  | |  |    \n";
    cout << "|______/ /__/     \\__\\  |__|        |__|     |_______||_______|_______/    |__|  |__| |__| | _|    \n";
    cout << RESET << "\n";
    cout << RED << "****************************************************************************************************" << RESET << "\n";
    cout << "  G A M E  O F  N A V A L  B A T T L E S  A N D  S U P R E M E C Y  O N  T H E  H I G H  S E A S\n";
    cout << RED << "***************************************************************************************************" << RESET << "\n";
    cout << CYAN << "°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸°º¤ø,¸¸,ø¤º°`°º¤ø\n";
    cout << BLUE << "°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸°º¤ø,¸¸,ø¤º°`°º¤ø\n" << RESET;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void displayRules()     //gives players to rules and scoring information
{
    cout << "\nTHESE ARE THE RULES OF BATTLESHIP:\n";
    cout << "\nBattleship is played across two 10 x 10 grids that are isolated from one another.\n";
    cout << "Each player places a number of ships (5) in secret in the pre-game setup.  Ships cannot\n";
    cout << "be moved during play. Player’s then alternate turns calling out “coordinates” on the\n";
    cout << "grid and recording hits and misses.  As a ship takes a predetermined number of hits,\n";
    cout << "it is sunk and removed from play. Once all ships on one side are sunk, the game is over.\n";
    cout << "\n";
    cout << "SCORING:\n";
    cout << RED << "Aircraft Carrier – 50 pts" << CYAN << "		Battleship – 40 pts" << GREEN << " 		Submarine – 30 pts\n" << RESET;
    cout << BLUE << "Cruiser – 30 pts" << MAGENTA << "		    	Detroyer – 20 pts" << YELLOW << "		Victory – 100 pts\n\n" << RESET;
   
    int rec1, rec2, maxRec;         //needed to invoke menu.
    menuLoop(rec1, rec2, maxRec);   //return to main menu loop.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void menuLoop(int &recNum, int &recNum2, int &maxRec)        //primary menu (players land here first)
{
    int choice = 0;             //variable to hold menu selection
    
    choice = showMenu();        //invoke menu and get choice
    
    while (choice != 99)        //as long as the choice wasn't 99
    {
        switch (choice)
        {
            case 1:
            {
                displayRules();                 //diplay rules
                break;
            }
            
            case 2:
            {
                startGame(recNum, recNum2, maxRec);     //invoke primary gameloop
                break;
            }
            
            case 3:
            {
                showHighScores();               //show honor roll.
                break;
            }
            
        }
    }
    
    if (choice == 99)           //if 99 is chosen...
    {
        exit(1);                //exit the game.
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int showMenu()          //shows ASCII art menu and asks for choice, which it verifies.
{
    int choice = 0;
    cout << "\n***************************************************************************************************\n";
    cout << "***************************************************************************************************\n";
    cout << "**                                                                                               **\n";
    cout << "**                                             GAME MENU                                         **\n";
    cout << "**                                                                                               **\n";
    cout << "**                                           (1) SHOW RULES                                      **\n";
    cout << "**                                           (2) START GAME                                      **\n";
    cout << "**                                           (3) HONOR ROLL                                      **\n";
    cout << "**                                                                                               **\n";
    cout << "***************************************************************************************************\n";
    cout << "***************************************************************************************************\n\n";
    cout << "ENTER CHOICE, OR " << CYAN << "99" << RESET << " TO EXIT GAME: ";
    cin >> choice;
    choice = validateMainMenuChoice(choice);        //make sure choice is 1 - 3, or 99.
    cout << "\n\n";
    return choice;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


Player displayRoster(int& playerChoice, int& maxRecords)
{
    fstream roster;                                         //create fstream
    roster.open("players.dat", ios::in | ios::binary);      //open file, set to binary input
    Player players;             //generic player container
    Player player1;             //player to be returned
    int recordNumber = 0;       //record number chosen for generic invocation of byteNum();
    
    playerChoice = 0;           //used to hold record choice from player (reference variable)
        
    if (!roster)  //verify file is available for reading
    {
        cout << "Error: unable to open file - returning to menu.";  //friendly error message.
    }

    else
    {
        cout << YELLOW <<"*******" << RESET << "COMMANDING OFFICERS LIST" << YELLOW << "*******" << RESET << "\n";
        roster.seekg(byteNum(recordNumber), ios::beg);                      //seek from beginning of roster stream
        roster.read(reinterpret_cast<char *>(&players), sizeof(players));   //find and reinterpret the records
        
        while (!roster.eof())                       //as long as there are valid records to be read...
        {
            cout << "Record Number: ";
            cout << recordNumber++ << endl;         //output record number
            cout << "First Name: ";
            cout << players.firstName <<  endl;     //output players first name
            cout << "Last Name: ";
            cout << players.lastName << endl;       //output players last name
            cout << "GamerTag: ";
            cout << players.handle << endl;         //output players gamertag/handle
            cout << "Current Score: ";
            cout << players.score << endl;          //output players score
            cout << endl;
            
            roster.read(reinterpret_cast<char *>(&players), sizeof(players));   //here to reinterpret each player pulled
            
        }
        maxRecords = recordNumber;                              //used during new account creation
        cout << "\nThat is all of the currently commissioned Officers.\n";
        
        roster.close();                                         //close and reset EOF bit
        roster.open("players.dat", ios::in | ios::binary);      //reopen players.dat
        
        cout << "Which Player Record do you want to use?: ";    //get record choice
        cin >> recordNumber;
        cout << endl;
        
        roster.seekg(byteNum(recordNumber), ios::beg);                          //find record chosen
        roster.read(reinterpret_cast<char *>(&players), sizeof(players));       //read it out

        for (int i = 0; i < firstSize; i++ )                //as you get data from read, pass it through the loop
        {
            player1.firstName[i] = players.firstName[i];    //set player 1's first name
            player1.lastName[i] = players.lastName[i];      //set player 1's last name
            player1.handle[i] = players.handle[i];          //set player 1's handle or gamertag
        }
        
        player1.score = players.score;                      //retrieve current high score for player 1.
        
        playerChoice = recordNumber;                        //
        
        roster.close();     //close file
        
    }
    
    return player1;     //return Player1 to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void startGame(int &recNum, int &recNum2, int &maxRec)  //set up game to be played
{
    
    Player player1;         //first player
    Player player2;         //second player
    bool selectAI = false;  //flag for AI player
    
    player1 = selectPlayer(recNum, maxRec);     //get player1
    cout << "OK.  Player one is " << CYAN << player1.handle << RESET << endl;
    
    player2 = chooseOpponent(selectAI, recNum2, maxRec);    //get player2
    cout << "OK.  Player two is " << MAGENTA << player2.handle << RESET << endl;
    
    gameLoop(player1, recNum, player2, recNum2, selectAI);      //start game loop
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void showHighScores()   //displays high scores
{
    
    int recNum = 0, recNum2 = 0, maxRec = 0;    //used to re-invoke menuLoop after display
    
    fstream honorRoll;                                      //open fstream
    honorRoll.open("Honor.dat", ios::in | ios::binary);     //open honor.dat in binary input mode
    Player players;     //generic players container
    
    if (!honorRoll)  //verify file is available for reading
    {
        cout << "Error: unable to open file - returning to menu.";  //friendly error message.
        return;
    }
    
    else
    {
        cout << YELLOW << "********HONOR ROLL********\n" << RESET;
        
        honorRoll.read(reinterpret_cast<char *>(&players), sizeof(players));
        
        while (!honorRoll.eof())        //until reaching end of honor.dat
        {
            cout << "First Name: ";
            cout << players.firstName <<  endl;     //output players first name
            cout << "Last Name: ";
            cout << players.lastName << endl;       //output players last name
            cout << "GamerTag: ";
            cout << players.handle << endl;         //output players gamertag/handle
            cout << "Current Score: ";
            cout << players.score << endl;          //output players score
            cout << endl;
            
            honorRoll.read(reinterpret_cast<char *>(&players), sizeof(players));   //here to reinterpret each player pulled
        }
        
        honorRoll.close();              //close honor.dat
        cout << YELLOW << "**REMEMBER THEIR VALOR!**\n" << RESET;
        menuLoop(recNum, recNum2, maxRec);      //when done, go back to menu
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Player selectPlayer(int &recNum, int &maxRec)    //obtains player 1 information - either from record or creation
{
    Player player1;     //generic p1 container
    
    int choice = 0;     //player's choice
    
    cout << "\n***************************************************************************************************\n";
    cout << "***************************************************************************************************\n";
    cout << "**                                                                                               **\n";
    cout << "**                                            PLAYER MENU                                        **\n";
    cout << "**                                                                                               **\n";
    cout << "**                                      (1) LOGIN EXISTING PLAYER                                **\n";
    cout << "**                                      (2) CREATE NEW PLAYER RECORD                             **\n";
    cout << "**                                                                                               **\n";
    cout << "***************************************************************************************************\n";
    cout << "***************************************************************************************************\n\n";
    cout << "ENTER CHOICE, OR " << CYAN << "99" << RESET << " TO EXIT MENU: ";
    cin >> choice;      //get choice
    choice = validatePlayerMenuChoice(choice);      //validate choice
    cout << "\n\n";
    
    player1 = playerMenu(choice, recNum, maxRec);       //invoke menu and pass choice to it
    
    
    return player1;   //return player1 to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Player playerMenu(int &choice, int &recNum, int &maxRec)     //executes choice from human player menu
{
    Player player1;     //generic p1 container
    int recNum2 = 0;    //used if menuloop has to be invoked
    
    switch (choice)     //choices for menu
    {
        case 1:
        {
            player1 = displayRoster(recNum, maxRec);    //diplay roster so player can pick his/her record
            break;  //break!
        }
                
        case 2:
        {
            player1 = createPlayer(recNum, maxRec);           //invoke player creation function and return value of
                                                              //create player back.
            break;  //break!
        }
                
    }
    
    if (choice == 99)                   //check for exit flag
    {
        menuLoop(recNum, recNum2, maxRec);      //if exit is chosen, go back to previous menu.
    }
    
    return player1;     //return p1 to calling function
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


Player chooseOpponent(bool &selectAI, int &recNum, int &maxRec)      //function to choose opponent (second player or AI)
{
    Player player2;     //generic container for p2
    int choice = 0;     //variable to hold player menu choice
        
    cout << "\n***************************************************************************************************\n";
    cout << "***************************************************************************************************\n";
    cout << "**                                                                                               **\n";
    cout << "**                                             OPPONENT MENU                                     **\n";
    cout << "**                                                                                               **\n";
    cout << "**                                      (1) LOGIN EXISTING PLAYER                                **\n";
    cout << "**                                      (2) CREATE NEW PLAYER RECORD                             **\n";
    cout << "**                                      (3) AI OPPONENT (COMPUTER)                               **\n";
    cout << "**                                                                                               **\n";
    cout << "***************************************************************************************************\n";
    cout << "***************************************************************************************************\n\n";
    cout << "ENTER CHOICE, OR " << CYAN << "99" << RESET << " TO EXIT MENU: ";
    cin >> choice;      //get choice from player
    
    choice = validateMainMenuChoice(choice);            //validate (must trust, but verify!)
    
    cout << "\n\n";
    
    player2 = opponentMenu(choice, selectAI, recNum, maxRec);   //call opponent menu and pass values to it
    
    
    return player2;     //return p2 to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Player opponentMenu(int choice, bool &selectAI, int &recNum, int &maxRec)        //function that offers P2 selections
{
    Player player2;     //generic container for p2
    int recNum2;        //generic container for referenced p2 if menuloop() must be called

    switch (choice)
    {
        case 1:
        {
            player2 = displayRoster(recNum, maxRec);        //allow player to choose record
            break;
        }
        
        case 2:
        {
            player2 = createPlayer(recNum, maxRec);               //allow player to create record
            break;
        }
            
        case 3:
        {
            char firstNameAI[15] = {'J','o','s','h','u','a'};       //build char array for AI Name
            char handleAI[15] = {'J','o','s','h','u','a'};          //build char array for AI Handle
            
            player2 = createPlayerAI(firstNameAI, handleAI);        //create AI player and return created entity
            recNum = maxRec + 1;
            cout << "Hello, my name is " << CYAN << "Joshua" << RESET << "\n";
            cout << "So you want to play a game? May I suggest " << RED << "Global Thermonuclear War?" << RESET << "\n";
            cout << "Just kidding...\n";
            selectAI = true;    //flag set to true to notify gameLoop() that an AI has been created.
        }
            
    }
            
    
    if (choice == 99)
    {
        menuLoop(recNum, recNum2, maxRec);      //call menuLoop if exit is requested.
    }

    return player2;     //return p2 to calling function.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

Player createPlayerAI(char name[firstSize], char handleAI[firstSize])       //Generates name and handle for AL
{
    Player AI;  //generic container for AI
    
    for (int i = 0; i < firstSize; i++ )        //iterate name array into AI construct
    {
        AI.firstName[i] = name[i];
    }
    
    for (int i = 0; i < firstSize; i++ )        //iterate handle array into AI construct
    {
        AI.handle[i] = handleAI[i];
    }
    return AI;      //return our creation to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


Player createPlayer(int &recNum, int &maxRec)       //walks player through creation of player account and opens players.dat
                                                    //and writes newly created player.
{
    fstream roster;                                         //create fstream
    
    roster.open("players.dat", ios::in | ios::binary);      //open file, set to binary input

    fstream playerFile("players.dat", ios::out | ios::binary | ios::app);       //calls fstream and opens player.dat in
                                                                                //appending input mode (binary)
    int recordNumber = 0;
    Player newPlayer;       //generic player container
    int newScore = 1;       //score variable

    roster.seekg(byteNum(recordNumber), ios::beg);                      //seek from beginning of roster stream
    roster.read(reinterpret_cast<char *>(&newPlayer), sizeof(newPlayer));   //find and reinterpret the records
    
    while (!roster.eof())                       //as long as there are valid records to be read...
    {
        cout << recNum++ << endl;         //output record number
                
        roster.read(reinterpret_cast<char *>(&newPlayer), sizeof(newPlayer));   //here to reinterpret each player pulled
    }
    
    if (recNum == 0)
    {
        maxRec = -1;
    }
    
    else
    {
       maxRec = recNum; 
    }
    
    roster.close();
    
    if(!playerFile) //always verify file is available before continuing!
    {
        cout << "Error: unable to open file - returning to menu.";  //friendly error message.
    }
        
        cin.ignore();  //need to consume the end of the menu line before reading name.
        cout << "What is your first Name?: ";
        cin.getline(newPlayer.firstName, firstSize);
        
        cout << "What is your Last Name?: ";
        cin.getline(newPlayer.lastName, lastSize);

        cout << "What is your Gamertag?: ";
        cin.getline(newPlayer.handle, handleSize);
    
        newPlayer.score = newScore;
    
        cout << "Saving your information....";  //inform user what is occuring
        playerFile.write(reinterpret_cast<char *>(&newPlayer),sizeof(newPlayer));
        cout << "done!\n\n";  //inform user
    
    
    playerFile.close();  //clean up and close file.
    
    maxRec = maxRec + 1;    //move to the next record past the ones available
    recNum = maxRec;        //set rec number to that number
    
    return newPlayer;    //return newly created player to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


Ship createShip(Ship type, char name[firstSize], int size, int HP, int identifier, int pointValue, bool floating)
                                                                                                            //builds ships
{
    
    for (int i = 0; i < firstSize; i++ )    //constructs name from array
    {
        name[i] = type.name[i];
    }
    
    type.size = size;                   //sets size
    type.hitPoints = HP;                //sets HP
    type.identifier = identifier;       //sets ID
    type.isSunk = false;                //sets boat to floating
    type.pointValue = pointValue;       //sets point value of sinking this ship
    
    return type;        //return newly commissioned vessel for fleet construction!
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void buildFleet(Ship &Carrier, Ship &Battleship, Ship &Submarine, Ship &Cruiser, Ship &Destroyer)   //builds player fleets
{
    char carrierName[15] = { 'C', 'a', 'r', 'r', 'i', 'e', 'r' };                       //generate carrier name
    char battleshipName[15] = { 'B', 'a', 't', 't', 'l', 'e', 's', 'h', 'i', 'p' };     //generate battleship name
    char submarineName[15] = { 'S', 'u', 'b', 'm', 'a', 'r', 'i', 'n', 'e' };           //generate submarine name
    char cruiserName[15] = { 'C', 'r', 'u', 'i', 's', 'e', 'r' };                       //generate cruiser name
    char destroyerName[15] = { 'D', 'e', 's', 't', 'r', 'o', 'y', 'e', 'r' };           //generate destroyer
    
    Carrier = createShip(Carrier, carrierName, 4, 5, 5, 50, true);              //create a carrier
    Battleship = createShip(Battleship, battleshipName, 3, 4, 4, 40, true);     //create a battleship
    Submarine = createShip(Submarine, submarineName, 3, 3, 6, 30, true);        //create a submarine
    Cruiser = createShip(Cruiser, cruiserName, 2, 3, 3, 30, true);              //create a cruiser
    Destroyer = createShip(Destroyer, destroyerName, 1, 2, 2, 20, true);        //create a destroyer
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////



long byteNum(int recNum)  //function to find size of Item
{
    return sizeof(Player) * recNum;  //return size of Item indicated by recNum.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void drawTargetField(int battleField[seaRows][seaColumns], Player playerID, int &pPoints)     //builds a targeting grid so
                                                                                //ship location remains obsfucated
{
    
    cout <<endl << endl << " PLAYER: " << MAGENTA << playerID.handle << RESET << "\t\t     POINTS: " << pPoints << endl;
    cout << YELLOW << "********************" << WHITE << "TARGET GRID" << YELLOW "********************" << RESET << endl;
    cout << endl << CYAN << "    A" << "    B" << "    C" << "    D" << "    E" << "    F" << "    G" << "    H" << "    I" << "    J"<< RESET << endl;
    
    for(int i = 0; i < seaRows; i++)            //these are the rows
    {
        cout<<  CYAN << i << " " << RESET;
        
        for(int j = 0; j < seaColumns; j++)        //these are the columns
        {
            if (battleField[i][j] == 0)
            {
                cout << BLUE << "| " << RESET;
                cout << CYAN << battleField[i][j] << RESET;     //this (i, j) location pulled from grid
                cout << BLUE << " |" << RESET;
            }
            
            else if (battleField[i][j] == 8)
            {
                cout << BLUE << "| " << RESET;
                cout << WHITE << battleField[i][j] << RESET;     //this (i, j) location is pulled from grid
                cout << BLUE << " |" << RESET;
                
            }
            
            else if (battleField[i][j] == 9)
            {
                cout << BLUE << "| " << RESET;
                cout << RED << battleField[i][j] << RESET;     //this (i, j) location is pulled from grid
                cout << BLUE << " |" << RESET;
                
            }
    
        }
        
        cout << endl;
        
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void drawBattleField(int battleField[seaRows][seaColumns], Player playerID)         //this is the function that builds the
                                                                                    //battlefield grid
{
    
    cout <<endl << endl << " PLAYER: " << MAGENTA << playerID.handle << RESET << endl;
    cout << YELLOW << "******************" << WHITE << "FIELD OF BATTLE" << YELLOW "******************" << RESET << endl;
    cout << endl << CYAN << "    A" << "    B" << "    C" << "    D" << "    E" << "    F" << "    G" << "    H" << "    I" << "    J"<< RESET << endl;
    
    for(int i = 0; i < seaRows; i++)            //these are the rows
    {
        cout<<  CYAN << i << " " << RESET;
        
        for(int j = 0; j < seaColumns; j++)        //these are the columns
        {
            if (battleField[i][j] == 0)
            {
                cout << BLUE << "| " << RESET;
                cout << CYAN << battleField[i][j] << RESET;     //this (i, j) location is pulled from file
                cout << BLUE << " |" << RESET;
            }
            
            else if (battleField[i][j] == 8)
            {
                cout << BLUE << "| " << RESET;
                cout << WHITE << battleField[i][j] << RESET;     //this (i, j) location is pulled from file
                cout << BLUE << " |" << RESET;
                
            }
            
            else if (battleField[i][j] == 9)
            {
                cout << BLUE << "| " << RESET;
                cout << RED << battleField[i][j] << RESET;     //this (i, j) location is pulled from file
                cout << BLUE << " |" << RESET;

            }
            
            else
            {
                cout << BLUE << "| " << RESET;
                cout << YELLOW << battleField[i][j] << RESET;     //this (i, j) location is pulled from file
                cout << BLUE << " |" << RESET;
            }
        }
    
        cout << endl;
        
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void gameLoop(Player p1, int &p1Rec, Player p2, int &p2Rec, bool &selectAI)     //the main gameloop for battleship (placement and firing)
{
    Ship p1Carrier, p1Battleship, p1Submarine, p1Cruiser, p1Destroyer;      //establish P1 ship structures
    Ship p2Carrier, p2Battleship, p2Submarine, p2Cruiser, p2Destroyer;      //estabilsh p2 ship structures
    
    bool allSunk = false;       //set allSunk flag to false
    
    buildFleet(p1Carrier, p1Battleship, p1Submarine, p1Cruiser, p1Destroyer);       //build P1's fleet
    buildFleet(p2Carrier, p2Battleship, p2Submarine, p2Cruiser, p2Destroyer);       //build P2's fleet
    
    int P1BattleField[seaRows][seaColumns] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};     //P1's battlefield
    int P2BattleField[seaRows][seaColumns] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};     //P2's battlefield
    int P1TargetField[seaRows][seaColumns] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};     //P1's Targeting Grid
    int P2TargetField[seaRows][seaColumns] = {{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};     //P2's Targeting Grid
    
    int p1Score = 0;        //initializing starting score P1
    int p2Score = 0;        //initializing starting score P2
    
    int lastRow = 0;        //holds last Row called for AI
    int lastColumn = 0;     //holds lass column called for AI
    
    if (selectAI == false)  //if 2 human players are chosen, run this game loop.
    {
        placeShipsPlayer(p1,p1Carrier, p1Battleship, p1Submarine, p1Cruiser, p1Destroyer, P1BattleField);       //P1 places
                                                                                                                //ships
        drawBuffer();   //this will hide P1's ships from P2 during deployment (unless they cheat as scroll up)
        
        placeShipsPlayer(p2,p2Carrier, p2Battleship, p2Submarine, p2Cruiser, p2Destroyer, P2BattleField);       //P2 places
                                                                                                                //ships
    
        while (allSunk == false)    //as long as at least one ship is floating in a player's fleet
        {
            allSunk = firingOrdersPlayer(p2, p2Score, P1BattleField, P2TargetField, p1Carrier,      //let P2 fire
                                         p1Battleship, p1Submarine, p1Cruiser, p1Destroyer);
            if (allSunk == true)        //then check and see if all the enemy ships are floating
            {
                displayScores(p1Rec, p1, p1Score, p2Rec, p2, p2Score);      //if they are all sunk, display the final score
                return;
            }
            
            allSunk = firingOrdersPlayer(p1, p1Score, P2BattleField, P1TargetField, p2Carrier,      //let P1 fire
                                         p2Battleship, p2Submarine, p2Cruiser, p2Destroyer);
            if (allSunk == true)        //then check and see if all the enemy ships are floating
            {
                displayScores(p1Rec, p1, p1Score, p2Rec, p2, p2Score);      //if they are all sunk, display the final score
                return;
            }
        
        }
    }
    
    else if (selectAI == true)  //if an AI match is chosen
    {
        placeShipsPlayer(p1, p1Carrier, p1Battleship, p1Submarine, p1Cruiser, p1Destroyer, P1BattleField);  //human goes
                                                                                                            //first placing
                                                                                                            //ships
        
        placeShipsAI(P2BattleField, p2, p2Carrier,p2Battleship, p2Submarine, p2Cruiser, p2Destroyer);       //AI runs
                                                                                                            //placement
                                                                                                            //algorithm
    
        while (allSunk == false)    //while at least one ship is floating in a given fleet.
        {

            allSunk = firingOrdersPlayer(p1, p1Score, P2BattleField, P1TargetField, p2Carrier,  //human fires first.
                                        p2Battleship, p2Submarine, p2Cruiser, p2Destroyer);
            if (allSunk == true)
            {
                displayScores(p1Rec, p1, p1Score, p2Rec, p2, p2Score);  //if they are all sunk, display the final score
                return;
            }
            
            allSunk = firingOrdersAI(p2, p2Rec, p2Score, P1BattleField, P2TargetField, p1Carrier,               //AI goes
                                    p1Battleship, p2Submarine, p2Cruiser, p2Destroyer, lastRow, lastColumn);    //second.
            if (allSunk == true)
            {
                displayScores(p1Rec, p1, p1Score, p2Rec, p2, p2Score);  //if they are all sunk, display the final score
                return;
            }
        }

    }
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void drawBuffer()  //for creating a buffer during a two player game (deprecated, but not deleted yet).
{
    for (int i = 0; i < 10; i++)
    {
    cout << "***************************************************************************************************\n";
    cout << "*********************************************FOG OF WAR********************************************\n";
    cout << "***************************************************************************************************\n";
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void placeShipsPlayer(Player player, Ship Carrier, Ship Battleship, Ship Submarine,         //function to place human 
                      Ship Cruiser, Ship Destroyer, int BattleField[seaRows][seaColumns])   //fleets
{
    int cycleCount = 0;             //for while loop
    int startRow;                   //Row for placement
    int startColumn;                //Column for placement
    char startColumnAlpha;          //Alpha variable for columns
    char direction;                 //direction deployment
    Ship currentVessel;             //container for which ship is being placed
    string currentVesselName;       //name of the current vessel (because iterating char arrays sucks)
    bool safeToDeploy = false;      //flag to see if placement is correct before writing to battlefield array
    
    Ship Fleet[5] = {Carrier, Battleship, Submarine, Cruiser, Destroyer};                   //fleet array
    string fleetNames[5] = {"Carrier","Battleship","Submarine","Cruiser","Destroyer"};      //names array
    
    cout << RED <<  "\n\nHave opponent look away during deployment!\n\n" << RESET;
    
    while (cycleCount < 5)          //loop to place ships (all 5)
    {
        currentVessel = Fleet[cycleCount];          //set current vessel to element at Fleet indexed by cyclecount
        currentVesselName = fleetNames[cycleCount]; //set current vessel name to element at fleetnames indexed by
                                                    //cyclecount
        
        while (safeToDeploy == false)   //As long as it isn't safe to deploy
        {
            
            cout << "Choose starting row location for " << CYAN << currentVesselName << RESET <<"(0-9): ";      //placement
                                                                                                                //row
            cin >> startRow;
            startRow = validateTargetRow(startRow); //validate it!
              
            cout << "Choose starting column location for " << CYAN << currentVesselName << RESET "(A-I): ";     //placement
                                                                                                                //column
            cin >> startColumnAlpha;
            startColumn = convertAlpha(startColumnAlpha);   //validate it!
            startColumn = validateTargetCol(startColumn);   //validate it!
            
            cout << "Choose direction of deployment - " << GREEN << "(h)" << RESET << "orizontal or "           //placement
                                                                                                                //direction
            << GREEN << "(v)" << RESET << "ertical: ";
            cin >> direction;
            direction = validateDirection(direction);   //validate it!
            
            safeToDeploy = validatePlacement(currentVessel, startRow, startColumn, direction, BattleField); //validate it!
            
            if (safeToDeploy == true)   //if everything checks out...
            {
                deployShip(currentVessel, direction, startRow, startColumn, BattleField);   //field the vessel
                drawBattleField(BattleField, player);   //show player how ship was deployed
            }
            
            else
            {
                safeToDeploy = false;   //if deployed incorrectly, do it again.
            }
            
        }
        
        safeToDeploy = false;   //reset flag
        cycleCount ++;          //increment cycleCount
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void deployShip(Ship sName, char direction, int sRow, int sCol, int grid[seaRows][seaColumns])      //Ship deployment
                                                                                                    //function
{
    if (direction == 'h' || direction == 'H')       //if horizontal deployment
    {

        for(int j = 0; j <= sName.size; j++)        //lock row and iterate along column
        {
            grid[sRow][sCol+j] = sName.identifier;  //pass identifier to each element location
        }

    }
    
    else if (direction == 'v' || direction == 'V')  //if vertical deployment
    {
        for (int i = 0; i <= sName.size; i++)       //lock column and iterate along row
        {
            grid[sRow+i][sCol] = sName.identifier;  //pass identifier to each element location
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int convertAlpha(char convertValue)     //this function converts char's to int's (there probably is a STL template to do it
                                        //better)
{
    int converted;
    
    if (convertValue == 'A' || convertValue == 'a')
    {
        converted = 0;
    }
    
    else if (convertValue == 'B' || convertValue == 'b')
    {
        converted = 1;
    }
    
    else if (convertValue == 'C' || convertValue == 'c')
    {
        converted = 2;
    }
    
    else if (convertValue == 'D' || convertValue == 'd')
    {
        converted = 3;
    }
    
    else if (convertValue == 'E' || convertValue == 'e')
    {
        converted = 4;
    }
    
    else if (convertValue == 'F' || convertValue == 'f')
    {
        converted = 5;
    }
    
    else if (convertValue == 'G' || convertValue == 'g')
    {
        converted = 6;
    }
    
    else if (convertValue == 'H' || convertValue == 'h')
    {
        converted = 7;
    }
    
    else if (convertValue == 'I' || convertValue == 'i')
    {
        converted = 8;
    }
    
    else if (convertValue == 'J' || convertValue == 'j')
    {
        converted = 9;
    }
    
    else
    {
        converted = 0;
    }
    
    return converted;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool determineHits(Player captain, int firedRow, int firedColumn,       //This function determines if a shot hits and the
    int grid[seaRows][seaColumns],int targetGrid[seaRows][seaColumns],  //results of the hit
    Ship &Carrier, Ship &Battleship, Ship &Submarine, Ship &Cruiser, Ship &Destroyer, int &score)

{
    bool shipDestroyed;     //holds flag for if ship is destroyed to pass back to gameLoop();
    
    
    if (grid[firedRow][firedColumn] == 8 || grid[firedRow][firedColumn] == 9)       //if a 8 or 9 is at that location, it was previously fired at
    {
        cout << "YOU ALREADY FIRED ON THAT LOCATION!!\n";
        shipDestroyed = false;
        drawTargetField(targetGrid, captain, score);
        return shipDestroyed;
    }
    
    else if (grid[firedRow][firedColumn] == 6)      //reports that the submarine has been hit
    {
        cout << RED << "A HIT ON THEIR SUBMARINE!! EXCELLENT SHOT CAPTAIN!!\n" << RESET;
        grid[firedRow][firedColumn] = 9;            //changes ID to 9 (a "RED" peg)
        targetGrid[firedRow][firedColumn] = 9;      //does the same for the target grid
        drawTargetField(targetGrid, captain, score);       //displays an updated target grid
        Submarine.hitPoints --;                     //reduces the targets HP
        shipDestroyed = false;                      //sets flag to false for now...
        
        if (Submarine.hitPoints == 0)               //if HP is reduced to 0
        {
            cout << "You sunk your Opponent's SUBMARINE!!\n";   //announce sinking
            Submarine.isSunk = true;                //set flag to sunk
            shipDestroyed = true;                   //set flag to destroyed
            return shipDestroyed;                   //let calling function know of destruction
        }
        
    }
    
    else if (grid[firedRow][firedColumn] == 5)
    {
        cout << RED << "A HIT ON THEIR CARRIER!! EXCELLENT SHOT CAPTAIN!!\n" << RESET;
        grid[firedRow][firedColumn] = 9;                //changes ID to 9 (a "RED" peg)
        targetGrid[firedRow][firedColumn] = 9;          //changes ID to 9 (a "RED" peg)
        drawTargetField(targetGrid, captain, score);   //displays an updated target grid
        Carrier.hitPoints --;                           //reduces the targets HP
        shipDestroyed = false;                          //sets flag to false for now...
        
        if (Carrier.hitPoints == 0)                     //if HP is reduced to 0
        {
            cout << "You sunk your Opponent's CARRIER!!\n"; //announce sinking
            Carrier.isSunk = true;                  //set flag to sunk
            shipDestroyed = true;                   //set flag to destroyed
            return shipDestroyed;                   //let calling function know of destruction
        }
    }
    
    else if (grid[firedRow][firedColumn] == 4)
    {
        cout << RED << "A HIT ON THEIR BATTLESHIP!! EXCELLENT SHOT CAPTAIN!!\n" << RESET;
        grid[firedRow][firedColumn] = 9;                //changes ID to 9 (a "RED" peg)
        targetGrid[firedRow][firedColumn] = 9;          //changes ID to 9 (a "RED" peg)
        drawTargetField(targetGrid, captain, score);    //displays an updated target grid
        Battleship.hitPoints --;                        //reduces the targets HP
        shipDestroyed = false;                          //sets flag to false for now...
        
        if (Battleship.hitPoints == 0)               //if HP is reduced to 0
        {
            cout << "You sunk your Opponent's BATTLESHIP!!\n";  //announce sinking
            Battleship.isSunk = true;                //set flag to sunk
            shipDestroyed = true;                    //set flag to destroyed
            return shipDestroyed;                    //let calling function know of destruction
        }
        
    }
    
    else if (grid[firedRow][firedColumn] == 3)
    {
        cout << RED << "A HIT ON THEIR CRUISER!! EXCELLENT SHOT CAPTAIN!!\n" << RESET;
        grid[firedRow][firedColumn] = 9;             //changes ID to 9 (a "RED" peg)
        targetGrid[firedRow][firedColumn] = 9;       //changes ID to 9 (a "RED" peg)
        drawTargetField(targetGrid, captain, score); //displays an updated target grid
        Cruiser.hitPoints --;                        //reduces the targets HP
        shipDestroyed = false;                       //sets flag to false for now...
        
        if (Cruiser.hitPoints == 0)                  //if HP is reduced to 0
        {
            cout << "You sunk your Opponent's CRUISER!!\n"; //announce sinking
            Cruiser.isSunk = true;                   //set flag to sunk
            shipDestroyed = true;                    //set flag to destroyed
            return shipDestroyed;                    //let calling function know of destruction
        }

    }
    
        
    else if (grid[firedRow][firedColumn] == 2)
    {
        cout << RED << "A HIT ON THEIR DESTROYER!! EXCELLENT SHOT CAPTAIN!!\n" << RESET;
        grid[firedRow][firedColumn] = 9;             //changes ID to 9 (a "RED" peg)
        targetGrid[firedRow][firedColumn] = 9;       //changes ID to 9 (a "RED" peg)
        drawTargetField(targetGrid, captain, score); //displays an updated target grid
        Destroyer.hitPoints --;                      //reduces the targets HP
        shipDestroyed = false;                       //sets flag to false for now...
        
        
        if (Destroyer.hitPoints == 0)                //if HP is reduced to 0
        {
            cout << "You sunk your Opponent's DESTROYER\n"; //announce sinking
            Destroyer.isSunk = true;                 //set flag to sunk
            shipDestroyed = true;                    //set flag to destroyed   
            return shipDestroyed;                    //let calling function know of destruction
        }

    }
    
    else 
    {
        cout << "YOU  MISSED!!\n";                      //if wasn't a hit or repeat fire
        grid[firedRow][firedColumn] = 8;                //set to 8 (a "White" peg)
        targetGrid[firedRow][firedColumn] = 8;          //do the same on the target grid
        drawTargetField(targetGrid, captain, score);    //show the miss
        shipDestroyed = false;                          //set flag to false
        return shipDestroyed;                           //return to the calling function
    }
   
    return shipDestroyed;   //just in case of a fall through, a generic return.
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool firingOrdersPlayer(Player captain, int &score, int grid[seaRows][seaColumns], int targetGrid[seaRows][seaColumns],
                        Ship &Carrier, Ship &Battleship, Ship &Submarine, Ship &Cruiser, Ship &Destroyer)  //function for
                                                                                                           //player firing
{
    
    int tRow = 0;                   //target row
    char colAlpha;                  //alpha column
    int tCol = 0;                   //target column
    bool destroyedTarget = false;   //flag for destroyed ship
    bool victory = false;           //flag for all ships destroyed
    
    drawTargetField(targetGrid, captain, score);   //show target field
    cout << "Captain " << MAGENTA << captain.handle << RESET << " what are your orders?:\n";
    cout << "Enter target " << CYAN << "row" << RESET " (0 - 9); ";
    cin >> tRow;                                   //get targeted row
    tRow = validateTargetRow(tRow);                //validated choice
    
    cout << "Enter target " << CYAN << "column" << RESET " (A - J): ";
    cin >> colAlpha;                               //get target column alpha
    tCol = convertAlpha(colAlpha);                 //convert to int
    tCol = validateTargetCol(tCol);                //validate int
    
    destroyedTarget = determineHits(captain, tRow, tCol, grid, targetGrid,
                                    Carrier, Battleship, Submarine, Cruiser, Destroyer, score);  //check and see what they
                                                                                                 //hit
    
    
    if (destroyedTarget == true)    //if something is destroyed
    {
        victory = assessGameState(captain, Carrier, Battleship, Submarine, Cruiser, Destroyer); //check for victory
                                                                                                //condition
        score = calculateScoring(victory, Carrier, Battleship, Submarine, Cruiser, Destroyer);  //calculate players score
        return victory; //return victory to calling function
    }
    
    else                            //otherwise..
    {
        victory = false;    //victory wasn't obtained
        cout << "\nCURRENT STRENGTH OF ENEMY FLEET\n\n";                        //read out current health of enemy
        cout << "Carrier" << "        " << Carrier.hitPoints << " HP\n";
        cout << "Battleship" << "     " << Battleship.hitPoints << " HP\n";
        cout << "Submarine" << "      " << Submarine.hitPoints << " HP\n";
        cout << "Cruiser" << "        " << Cruiser.hitPoints << " HP\n";
        cout << "Destroyer" << "      " << Destroyer.hitPoints << " HP\n\n";
        
        cout << "CURRENT SCORE IS: " << score << endl << endl;                  //read out current score
    }
    
    return victory;     //return victory to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


bool assessGameState(Player Captain, Ship Carrier, Ship Battleship, Ship Submarine, Ship Cruiser, Ship Destroyer)
                                                                                            //function to check for victory
{
    bool Victory;   //flag for victory
    
    if ((Carrier.isSunk == true) && (Battleship.isSunk == true) && (Submarine.isSunk == true) &&        //see if all ships are sunk
       (Cruiser.isSunk) == true && (Destroyer.isSunk == true))
        
    {
        cout << MAGENTA;
        cout << " _____   ___  ___  ___ _____   _____  _   _ ___________ _ _\n";
        cout << "|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | |  ___| ___ \\ | |\n";                   //inform players game is over.
        cout << "| |  \\// /_\\ \\| .  . || |__   | | | || | | | |__ | |_/ / | |\n";
        cout << "| | __ |  _  || |\\/| ||  __|  | | | || | | |  __||    /| | |\n";
        cout << "| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ / |___| |\\ \\|_|_|\n";
        cout << " \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/\\____/\\_| \\_(_|_)\n";
        cout << RESET;
        
        Victory = true;
        
    }
    
    else                //otherwise, game continues
    {
        Victory = false;
    }
 
    return Victory;     //return victory to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


int validateTargetRow(int value)        //validates target row given is within bounds
{
    while (isalpha(value) == 1)              //testing...
    {
        cout << YELLOW << "ROW MUST BE A NUMBER. PLEASE RE-ENTER: ";
        cin >> value;
        cout << RESET << endl;
    }
    
    
    while ((value < 0)||(value > 9))    //makes sure value is on grid
        
    {
        cout << YELLOW << "TARGET OUT OF BOUNDS. PLEASE RE-ENTER: ";
        cin >> value;
        cout << RESET << endl;

    }
   
    return value;                       //returns value to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int validateTargetCol(int value)        //validates target column is with bounds
{
    
    while ((value < 0)||(value > 9))    //makes sure value is on grid
        
    {
        cout << YELLOW << "TARGET OUT OF BOUNDS. PLEASE RE-ENTER: ";
        cin >> value;
        cout << RESET << endl;
        
    }
    
    return value;                       //returns value to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


bool validatePlacement(Ship vessel, int startRow, int startColumn, char direction,
                       int deploymentGrid[seaRows][seaColumns])     //function to validate placement of ships
{
    bool deploymentOK = false;                                      //flag for deployment validation
    
    if (direction == 'h' || direction == 'H')                       //if horizontally deployed
    {
        
        for(int j = 0; j <= vessel.hitPoints; j++)                  //move along column and...
        {
            if (deploymentGrid[startRow][startColumn+j] == 0)       //see if anything besides empty sea is there 
            {
                deploymentOK = true;                                //if it's OK, flag set to true.
            }
            
            else                                                    //otherwise
            {
                cout << YELLOW << "WARNING! COLLISION DETECTED! REDEPLOY VESSEL!\n" << RESET;  //inform player
                deploymentOK = false;                               //set to false.
                return deploymentOK;
                break;
            }
        }
    }
    
    else if ((direction == 'v') || (direction == 'V'))              //if vertically deployed
    {
        for (int i = 0; i <= vessel.hitPoints; i++)                 //move along row and..
        {
            if (deploymentGrid[startRow+i][startColumn] == 0)       //see if anything besides empty sea is there
            {
                deploymentOK = true;                                //if it's OK, set flag to true.
            }
            
            else                                                    //otherwise...
            {
                cout << YELLOW << "WARNING! COLLISION DETECTED! REDEPLOY VESSEL!\n" << RESET;   //inform player
                deploymentOK = false;                               //set to false.
                return deploymentOK;
                break;
            }
        }
    }
    
    return deploymentOK;                                            //return deployment flag to calling function
        
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


char validateDirection(char heading)                //function to validate direction of deployment
{
    bool headingCorrect = false;                    //flag for if heading is OK
    
    while (headingCorrect == false)                 //while that flag is false
    {
        if (heading == 'h' || heading == 'H' ||     //if H or h or V or v is entered
            heading == 'v' || heading == 'V')
        {
            headingCorrect = true;                  //it's OK to continue
        }
        
        else                                        //otherwise
        {
            cout << YELLOW << "Direction invalid - enter (H) or (V): ";     //tell player to re-enter
            cin >> heading;                         //get corrected heading
            cout << RESET << endl;
        }
    }
    return heading;                                 //return validated heading to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


char validateColumnChar(char alphaCol)      //attempt to validate char entries - not working currently
{                                           //probably should use a STL template, but ran out of time.
    bool columnCheck = false;
    
    while (isdigit(alphaCol) == 1)
    {
        cout << YELLOW << "COLUMN MUST BE A LETTER A-J. PLEASE RE-ENTER: ";
        cin >> alphaCol;
        cout << RESET << endl;
    }
    
    while (columnCheck == false)
    {
        
        if ((alphaCol == 'A') || (alphaCol == 'a'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'B') || (alphaCol == 'b'))
        {
            columnCheck = true;
        }
        
        else if  ((alphaCol == 'C') || (alphaCol == 'c'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'D') || (alphaCol == 'd'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'E') || (alphaCol == 'e'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'F') || (alphaCol == 'f'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'G') || (alphaCol == 'g'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'H') || (alphaCol == 'h'))
        {
            columnCheck = true;   
        }
        
        else if ((alphaCol == 'I') || (alphaCol == 'i'))
        {
            columnCheck = true;
        }
        
        else if ((alphaCol == 'J') || (alphaCol == 'j'))
        {
            columnCheck = true;
        }
       
        else;
        {
        
            cout << YELLOW << "TARGET OUT OF BOUNDS. PLEASE RE-ENTER: ";
            cin >> alphaCol;
            cout << RESET << endl;
            columnCheck = false;

        }
    
    }
    return alphaCol;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void placeShipsAI(int passedGrid [seaRows][seaColumns], Player AI,      //function to place AI ships
                  Ship Carrier, Ship Battleship, Ship Submarine, Ship Cruiser, Ship Destroyer)
{
    int dieRoll;                        //random number deployment
    
    int randomNumber = (rand()%10)+1;   //random number 1 to 10
    dieRoll = randomNumber;
    
    switch (dieRoll)                    //switch uses dieroll to choose deployment
    
    {
        case 1:
        {
            deployShip(Carrier, 'h', 2, 3, passedGrid);
            deployShip(Battleship, 'v', 6, 7, passedGrid);
            deployShip(Submarine, 'h', 4, 7, passedGrid);
            deployShip(Cruiser, 'h', 5, 3, passedGrid);
            deployShip(Destroyer, 'v', 8, 3, passedGrid);
            break;
        }
          
        case 2:
        {
            deployShip(Carrier, 'h', 2, 3, passedGrid);
            deployShip(Battleship, 'v', 5, 7, passedGrid);
            deployShip(Submarine, 'h', 3, 7, passedGrid);
            deployShip(Cruiser, 'h', 4, 3, passedGrid);
            deployShip(Destroyer, 'v', 7, 3, passedGrid);
            break;
        }
            
        case 3:
        {
            deployShip(Carrier, 'h', 1, 3, passedGrid);
            deployShip(Battleship, 'v', 4, 7, passedGrid);
            deployShip(Submarine, 'h', 2, 7, passedGrid);
            deployShip(Cruiser, 'h', 3, 3, passedGrid);
            deployShip(Destroyer, 'v', 6, 3, passedGrid);
            break;
        }
            
        case 4:
        {
            deployShip(Carrier, 'h', 2, 3, passedGrid);
            deployShip(Battleship, 'h', 3, 4, passedGrid);
            deployShip(Submarine, 'h', 5, 7, passedGrid);
            deployShip(Cruiser, 'h', 5, 3, passedGrid);
            deployShip(Destroyer, 'v', 3, 2, passedGrid);

            break;
        }
            
        case 5:
        {
            deployShip(Carrier, 'h', 2, 3, passedGrid);
            deployShip(Battleship, 'v', 6, 7, passedGrid);
            deployShip(Submarine, 'h', 4, 7, passedGrid);
            deployShip(Cruiser, 'h', 5, 3, passedGrid);
            deployShip(Destroyer, 'v', 8, 3, passedGrid);
            break;
        }
            
        case 6:
        {
            deployShip(Carrier, 'h', 5, 4, passedGrid);
            deployShip(Battleship, 'h', 3, 4, passedGrid);
            deployShip(Submarine, 'h', 7, 7, passedGrid);
            deployShip(Cruiser, 'h', 8, 4, passedGrid);
            deployShip(Destroyer, 'h', 6, 2, passedGrid);
            break;
        }
            
        case 7:
        {
            deployShip(Carrier, 'h', 5, 4, passedGrid);
            deployShip(Battleship, 'h', 3, 4, passedGrid);
            deployShip(Submarine, 'h', 4, 7, passedGrid);
            deployShip(Cruiser, 'h', 6, 5, passedGrid);
            deployShip(Destroyer, 'h', 4, 4, passedGrid);
            break;
        }
            
        case 8:
        {
            deployShip(Carrier, 'h', 2, 3, passedGrid);
            deployShip(Battleship, 'v', 6, 7, passedGrid);
            deployShip(Submarine, 'h', 4, 7, passedGrid);
            deployShip(Cruiser, 'h', 5, 3, passedGrid);
            deployShip(Destroyer, 'v', 8, 3, passedGrid);
            break;
        }
        
        case 9:
        {
            deployShip(Carrier, 'h', 3, 2, passedGrid);
            deployShip(Battleship, 'h', 2, 6, passedGrid);
            deployShip(Submarine, 'h', 4, 7, passedGrid);
            deployShip(Cruiser, 'h', 5, 3, passedGrid);
            deployShip(Destroyer, 'h', 5, 2, passedGrid);
            break;
        }
            
        case 10:
        {
            deployShip(Carrier, 'v', 3, 8, passedGrid);
            deployShip(Battleship, 'h', 7, 2, passedGrid);
            deployShip(Submarine, 'h', 6, 4, passedGrid);
            deployShip(Cruiser, 'h', 5, 2, passedGrid);
            deployShip(Destroyer, 'v', 3, 5, passedGrid);
            break;
        }
 
            
            
        default:
            break;
    }

    cout << YELLOW << "\n\nPreparing to deploy AI vessels...";      //taunt and let players know ships are deployed
    cout << "WOPR ONLINE...downloading battle plans....\n";
    cout << "\n\nPlans acquired.\n" << RESET;
    cout << RED << "\n\nFIELDING VESSELS...";
    cout << "WE ARE READY TO DOMINATE HUMANITY!" << RESET;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool firingOrdersAI(Player AI, int &recNum, int &score, int grid[seaRows][seaColumns],      //AI algorithm for firing
                    int targetGrid[seaRows][seaColumns],Ship &Carrier, Ship &Battleship,
                    Ship &Submarine, Ship &Cruiser, Ship &Destroyer, int& lastRow, int &lastColumn)
{
    int targetRow;                              //targeted row
    int targetColumn;                           //targeted column
    
    bool destroyedTarget = false;               //flag for ship sunk
    bool victory = false;                       //flag for victory (fleet sunk)
    
    int randomNumber1 = (rand()%10)+1;          //random number for rows
    int randomNumber2 = (rand()%10)+1;          //random number for columns
    
    targetRow = randomNumber1;                  //generate random row
    targetColumn = randomNumber2;               //generate random column
    
    if (grid[lastRow][lastColumn] == 9)         //if a hit was previously scored
    {
        cout << "attempting to lock onto target...\n";  //inform opponent his ships are threatened
        cout << "LAST FIRED LOCATION IS: " << lastRow << "," << lastColumn << endl;
        
        targetRow = lastRow;                    //set target to previous row
        targetColumn = lastColumn + 1;          //set target to one column up (horizontal hit)
        
        destroyedTarget = determineHits(AI,targetRow,targetColumn, grid, targetGrid,    //see what the resulst are
                                        Carrier, Battleship, Submarine, Cruiser, Destroyer, score);
    }
    
    else if (grid[lastRow][lastColumn] == 8)                            //if previous attack missed
    {
        
        while ((targetRow == lastRow) && (targetColumn == lastColumn))  //make sure you aren't firing at the same place!
            
        {
            targetRow = randomNumber1;      //get a different row
            targetColumn = randomNumber2;   //get a different column
            
        }
        
        cout << "LAST FIRED LOCATION IS: " << lastRow << "," << lastColumn << endl; //inform opponent
        cout << "INCOMING!! Joshua is firing shells at your location" << targetRow << "," << targetColumn << "!\n";
        
        destroyedTarget = determineHits(AI,targetRow,targetColumn, grid, targetGrid,    //see if you hit
                                        Carrier, Battleship, Submarine, Cruiser, Destroyer, score);
    }
    
    else                                    //if it wasn't a miss or hit previously...
    {
        
        cout << "LAST FIRED LOCATION IS: " << lastRow << "," << lastColumn << endl; //inform opponent
        cout << "INCOMING!! Joshua is firing shells at your location" << targetRow << "," << targetColumn << "!\n";
        
        destroyedTarget = determineHits(AI,targetRow,targetColumn, grid, targetGrid,                   //fire randomly and 
                                        Carrier, Battleship, Submarine, Cruiser, Destroyer, score);    //hope for the best.
    }
  
        
    
    
    if (destroyedTarget == true)                    //if you land a hit and sink a ship
    {
        victory = assessGameState(AI, Carrier, Battleship, Submarine, Cruiser, Destroyer);      //see if you won
        score = calculateScoring(victory, Carrier, Battleship, Submarine, Cruiser, Destroyer);  //recalculate scoring
        return victory;
        
    }
    
    else        //otherwise, you failed to achieve victory THIS round...
    {
        victory = false;    //so set flag to false
    }
    
    lastRow = targetRow;            //remember what row you just hit
    lastColumn = targetColumn;      //remember what column you just hit
    
    
    return victory;     //return victory status to calling function

    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


int validateMainMenuChoice(int choice)      //simply a validation routine for the main menu
{
    while (choice < 1 || choice > 3)    //see if the choice is on the menu
    {
        if (choice == 99)               //see if the choice is to exit menu
        {
            return choice;              //if so return the exit choice
            break;
        }
        
        else    //otherwise...
        {
            cout << "\nPlease Enter a valid selection or " << CYAN << " 99 " << RESET <<":\n";  //get valid data
            cin >> choice;
        }
        
    }
    
    return choice;      //return the validated choice back to calling function...
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


int validatePlayerMenuChoice(int choice)    //another simple validation routine for the player menu.
{
    while (choice < 1 || choice > 2)        //see if choice is valid
    {
        if (choice == 99)                   //see if exit is flagged
        {
            return choice;
            break;
        }
        
        else    //otherwise...
        {
            cout << "\nPlease Enter a valid selection or " << CYAN << " 99 " << RESET <<":\n";  //get valid data
            cin >> choice;
        }
    }
    
    return choice;  //return validated data back to calling function.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int calculateScoring(bool fleetSunk, Ship Carrier, Ship Battleship, Ship Submarine, Ship Cruiser, Ship Destroyer)
//                                                                      //function to calculate score
{
    int totalScore = 0, carrierScore = 0, battleshipScore = 0,          //intialize all scores at 0.
        submarineScore = 0, cruiserScore = 0, destroyerScore = 0;

    carrierScore = isShipSunk(Carrier);                 //see if carrier is sunk and add to score.
    battleshipScore = isShipSunk(Battleship);           //see if battleship is sunk and add to score.
    submarineScore = isShipSunk(Submarine);             //see if submarine is sunk and add to score.
    cruiserScore = isShipSunk(Cruiser);                 //see if cruiser is sunk and add to score.
    destroyerScore = isShipSunk(Destroyer);             //see if destroyer is sunk and add to score.
    
    totalScore = carrierScore + battleshipScore + submarineScore + cruiserScore + destroyerScore;   //sum all scores
    
    if (fleetSunk == true)      //see if the player has achieved victory by checking fleekSunk flag
    {
        totalScore = totalScore + 100;                  //add 100 points if victory was achieved.
    }
    
    return totalScore;          //return totaled score to calling function.
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

int isShipSunk(Ship Vessel)     //function to see if ship is sunk and pass point value back to score calculator
{
    int pointValue = 0;         //make sure the point value we start with is 0.
    
    if (Vessel.isSunk == true)  //if ship has been sunk
    {
        pointValue = Vessel.pointValue; //get it's point value
        cout << "You earned " << pointValue << " pts. for sinking your opponent ship!" << endl; //notify player
    }
    
    return pointValue;          //return pointvalue to calling function
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void displayScores(int &recordNum, Player p1, int P1Score, int &recordNum2, Player p2, int P2Score)  //show scores on
                                                                                                     //console
{
    cout << endl << p1.handle << " score is " << CYAN << P1Score << RESET << endl;
    cout << endl << p2.handle << " score is " << CYAN << P2Score << RESET << endl;
    
    writeScores(recordNum, p1, P1Score, recordNum2, p2, P2Score);        //write Player scores to file
    honorScores(recordNum, p1, P1Score, recordNum2, p2, P2Score);        //send to honor roll
    
    int maxRec = 0;
    
    menuLoop(recordNum, recordNum2, maxRec);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void writeScores(int &recordNum, Player p, int pScore, int &recordNum2, Player p2, int p2Score)   //writes scores to file
{
        
    fstream inputFile("players.dat", ios::in | ios::binary);        //open files in binary mode.
    fstream outputFile("players.dat", ios::out | ios::binary);
    
    if (!inputFile)
    {
        cout << "\nError: unable to open file";                     //friendly error message.
        return;
    }
            
    inputFile.seekg(byteNum(recordNum), ios::beg);                  //find record
    inputFile.read(reinterpret_cast<char *>(&p), sizeof(p));        //read record
    
    p.score = p.score + pScore;
    
    outputFile.seekg(byteNum(recordNum), ios::beg);                 //find record
    outputFile.write(reinterpret_cast<char *>(&p), sizeof(p));      //write binary data to file.
    
    inputFile.seekg(byteNum(recordNum2), ios::beg);                 //find record
    inputFile.read(reinterpret_cast<char *>(&p2), sizeof(p2));      //read record
    
    p2.score = p2.score + p2Score;
    
    outputFile.seekg(byteNum(recordNum2), ios::beg);                //find record
    outputFile.write(reinterpret_cast<char *>(&p2), sizeof(p2));    //write binary data to file.
    
    
    inputFile.close();                                              //close files
    outputFile.close();
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


void honorScores(int &recordNum, Player p, int pScore, int &recNum2, Player p2, int pScore2)//writes scores to file
{
    
    fstream inputFile("honor.dat", ios::in | ios::binary);        //open files in binary mode.
    fstream outputFile("honor.dat", ios::out | ios::binary | ios::app);
    
    if (!inputFile)
    {
        cout << "\nError: unable to open file";                     //friendly error message.
        return;
    }
    
    inputFile.seekg(byteNum(recordNum), ios::beg);                  //find record
    inputFile.read(reinterpret_cast<char *>(&p), sizeof(p));        //read record
    
    p.score = pScore;
    
    outputFile.seekg(byteNum(recordNum), ios::beg);                 //find record
    outputFile.write(reinterpret_cast<char *>(&p), sizeof(p));      //write binary data to file.
    
    inputFile.seekg(byteNum(recNum2), ios::beg);                 //find record
    inputFile.read(reinterpret_cast<char *>(&p2), sizeof(p2));      //read record
    
    p2.score = pScore2;
    
    outputFile.seekg(byteNum(recNum2), ios::beg);                //find record
    outputFile.write(reinterpret_cast<char *>(&p2), sizeof(p2));    //write binary data to file.
    
    
    inputFile.close();                                              //close files
    outputFile.close();
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION BREAK FUNCTION/////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
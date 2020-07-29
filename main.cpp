/*
  	Monopoly 
  	Authors: Sam Williamson, Peter Wood
  	Date Started: 
  	Date Finished: 18/08/15
  	Version: 4.0
  	Build: 1073
*/

#include <iostream>	// input and output
#include <iomanip>	// table for start/end turn screens and auction
#include <string>	// manipulation of strings
#include <fstream>	// read files
#include <cstdlib>	// general purpose functions
#include <stdlib.h>	// general purpose functions
#include <ctime>	// used for random number generation
#include <vector>	// used to create dynamic arrays
#include "gameTile.h"	// the gameTile header file
#include "player.h"		// the player header file

using namespace std;

void mainMenu();														//function for main menu at the start of the program
void displayRules();													// functionthat displays the rules
void primeRead();														//function to check if the board is complete, eg: all cards present, all properties have all attributes necessary
int playerMenu();														// function for the main menu
void errorMenuScreen();													// error menu for the main menu
char endOfTurnScreen(int playerNo, string name, int wallet, int position, player* players);	// screen that appears at the end of a turn
char startOfTurnScreen(int playerNo, string name, int wallet, int position, player* players);	// screen that appears at the start of a turn
void purchase(int playerNo, string name, int wallet, player* players, string propertyName, int currentPos, int playerCount); // function to purchase unowned property
void lineCountCheck(int lineCount);										// function that checks all lines are in files
void cardCheck(int cards);												// function that checks all cards are in files
void assignPropAttr();													// assigns attributes to properties
player* assignPlayerAttr(int playerCount);								//assigns attributes to players
int* rollDice();														// rolls dice
bool isDoubles(int dice1, int dice2);									// checks if dice roll was doubles
void playerCanPay(int moneyOwed, player* players, int playerNo, bool bankPayment);		// checks if the play can afford to pay, if not it keeps running until they can
void auction(player* players, int currentPos, int playerCount);		// function to auction properties 
void setChanceCards();												// shuffle and assign chance cards
void setCommunityChestCards();										// shuffle and assign community chest cards
void drawChanceCard(player* players, int playerNo, int playerCount, int currentPos, int* properties);	// draw chance cards 
void drawCommunityChestCard(player* players, int playerNo, int playerCount, int* properties);			// draw community chest cards
void updateSetRent(player* players, string colour, int propertyCount, bool gettingProperty, int playerNo);	// updates the rent of properties when a set is completed and when a set becomes incomplete
void outputTitleDeed(int position);										// outputs the ascii for the property being landed on
void gameOver(int winner, player* players);								// outputs a game ending screen
void trade(player* players, int playerNo, int playerCount);				// function to trade assets
bool isStringValid(string test);										// checks if a string is valid
void manage(player* players, int playerNo);								// manage properties
void mortgage(player* players, int proeprtyPos, int playerNo);			// mortgage a selected property
void unmortgage(player* players, int propertyPos, int playerNo);		// unmortgage a selected property
void addHouse(player* players, int propertyPos, int playerNo);			// add a house to the selected property
void addHotel(player* players, int propertyPos, int playerNo);			// add a hotel to the selected property
void removeHouse(player* players, int propertyPos, int playerNo);		// remove a house from the selected property
void removeHotel(player* players, int propertyPos, int playerNo);		// removve a hotel from the selected property

gameTile board[40];														// array for the board, global variable to be accessed by every function
int turnCount = 0;														// global variable for the turn count

vector <string> chanceCards;											// vector of strings to hold the chance cards
vector <string> communityChestCards;									// vector of strings to hold the community chest cards
int bankruptCount = 0;													// global variable to keep track of amount of bankrupt players
int housesLeft = 32;													// global variable to store the houses the bank has left
int hotelsLeft = 12;													// global variable to store the hotels the bank has left

int main() 
{
	mainMenu();															// call the main menu function
	system("cls");														// clear screen
	char userInput;	 													// userInput on start of turn and end of turn screen
	int playerCount;													// amount of players
	bool playerRolledDoubles;											// boolean for when player rolls doubles
	int sumOfDice;														// is the sum of two dice
	int currentWallet;													// current player's wallet
	int currentPos;														// current player's position
	int newPos;															// current player's new position
	string currentName;													// current player's name
	int playerProperties[28];											// current player's property positions
	playerCount = playerMenu();											// calls the player menu function
	int doublesThrown;													// counts the doubles thrown										
	int rentOwed;														// the rent owed on a tile
	string playerToBePaid;												// player that is to be paid when paying rent
	int playerOwedTo;													// the player number of the above person
	char incomeTaxPayment;												// what the player decides to pay on income tax, 10% OR $200
	int ii;																// copy of i
	int currentTotalWorth;												// total worth of the current player
	
	player* players = assignPlayerAttr(playerCount);					// assigns attributes to the players
	setChanceCards();													// calls function to set and shuffle the chance cards
	setCommunityChestCards();											// calls function to set and shuffle community chest cards
	assignPropAttr();													// assign attributes to properties
									
	system("PAUSE");		
	system("cls");
	
	for ( ; ; )															// infinite loop that will be broken when all player's are bankrupt
	{	
		turnCount++;													// increments the turn count 
		
		for (int j = 0; j < playerCount; j++)							// resets each players doubles count
		{
		 	players[j].resetDoubles();
		}
									
		for (int i = 0; i < playerCount ; i++)							// for loop for the actual game
		{
			if(bankruptCount == (playerCount -1))						// game ending condition
			{
				for(int i = 0; i < playerCount; i++)					// checks which player is not bankrupt
				{
					if(players[i].getBankruptStatus() == false)
					{
						gameOver(i, players);							// sends the game over function the player not bankrupt and the array of players
					}
				}
				
			}
			while(players[i].getBankruptStatus() == true)				// checks if the player that is about to play is bankrupt
			{															// if they are then increment i until the player is not bankrupt
				i++;
			}
			ii = i;														// copies i to another variable
			system("CLS");												// clears screen
			currentName = players[ii].getName();						// sets the name of the current player
			currentWallet = players[ii].getWallet();					// sets the wallet of the current player
			currentPos = players[ii].getPosition();						// sets the position of the current player
			currentTotalWorth = players[ii].getTotalWorth();			// sets the total worth of the current player
			players[ii].updateProperties(board);
			for(int j = 0; j < 28; j++)									// populates the current players property array
			{
				playerProperties[j] = players[ii].getProperties(j);
			}
			doublesThrown = players[ii].getDoublesCount();				// assigns the doubles thrown by the current player
			
			do															// loop that runs until the player decides to roll the dice or declares bankruptcy
			{ 
				userInput = startOfTurnScreen(ii, currentName, currentWallet, currentPos, players); // calls the start of turn screen
				if (userInput == 'X')									// if the player decided to exit the game completely
				{
					cout << "Terminating Game..." << endl;
					exit(0);
				}
				
				else if(userInput == 'T')								// if the player decided to trade
				{
					trade(players, ii, playerCount);
				}
				
				else if(userInput == 'M')								// if the player decided to manage
				{
					manage(players, ii);
				}
				
				else if(userInput == 'D')								// if the player decided to declare themselves bankrupt
				{
					cout << currentName << " is now bankrupt!" << endl;
					players[ii].declareBankrupt(board, true);
					while(players[ii].getJailCards() > 0)
					{
						if(communityChestCards.size() == 15)
						{
							communityChestCards.push_back("GET OF JAIL FREE. This card may be kept until needed or traded.");
						}
						else if(chanceCards.size() == 15)
						{
							chanceCards.push_back("GET OF JAIL FREE. This card may be kept until needed or traded.");
						}
						players[ii].decrementJailCards();
					}
					bankruptCount++;
				}
				
				else if(userInput == 'C')								// if the player used a get out of jail free card, which is only possible if they are in jail
				{
					cout << "You used a Get Out Of Jail Free Card." << endl;
					players[ii].decrementJailCards();					// takes a get out of jail free card away from the player
					cout << "You have " << players[ii].getJailCards() << " left." << endl;	// outputs how many they have left
					players[ii].setInJail(false);						// sets the player to be out of jail
				}
				
				else if (userInput == 'P')								// if they decide to pay the fine, only possible in jail
				{
					playerCanPay(50, players, ii, false);				// checks if the player can afford the fine
					
					cout << "You paid the fine!" << endl;
					currentWallet = players[ii].getWallet();			// gets the players new wallet value
					currentTotalWorth = players[ii].getTotalWorth();	// gets the players new total worth
						
					players[ii].setInJail(false);						// sets the player to be out of jail
				}
				
			}while(userInput != 'R' && userInput != 'D');				// end of do while loop
			
			if(userInput == 'D')										// if they entered d to declare bankruptcy
			{
				continue;												// continue to next iteration of for loop
			}	
			int* dice = rollDice();										// rolls dice
			sumOfDice = dice[0] + dice[1];								// calculates the sum of the dice
						
			if (players[ii].isInJail() == true)							// if the player is in jail
			{
				playerRolledDoubles = isDoubles(dice[0], dice[1]);	// check if they rolled doubles
				
				if(playerRolledDoubles)									// if they rolled doubles
				{
					cout << "You got doubles!" << endl;
					cout << currentName << " is now out of Jail!" << endl;
					players[ii].setInJail(false);						// set them to be out of jail
				}
				
				else if(playerRolledDoubles == false && players[ii].getTurnsInJail() == 3)	// if they didnt roll doubles, and have been in jail for three turns
				{
					cout << "Sorry! You have to pay the $50 fine!" << endl;					// force to pay fine
					
					playerCanPay(50, players, ii, true);									// checks if they can afford the fine
					
					cout << "You paid the fine!" << endl;
					currentWallet = players[ii].getWallet();								// gets new wallet value
					currentTotalWorth = players[ii].getTotalWorth();						// gets new total worth
						
					players[ii].setInJail(false);											// sets them out of jail
				}			
			}
			
			if(players[ii].isInJail() == false)					// if they arent in jail
			{
				newPos = currentPos + sumOfDice;						// assign a new position
				/* Test ASCII
				for (int p = 0; p < 40; p++)
				{
					outputTitleDeed(p);
					system("PAUSE");
				}*/
				
				if (newPos > 39)										// if the player gets to 40 or greater
				{
					newPos -= 40;										// subtract 40 to ge the position on the board
					cout << "You passed GO! Collect $200." << endl;		// give player $200 for passing go
					players[ii].addToWallet(200);
					players[ii].addToTotalWorth(200);
					
					currentWallet = players[ii].getWallet();			// gets wallet and total worth
					currentTotalWorth = players[ii].getTotalWorth();
					
				}
				outputTitleDeed(newPos);								// output the new positions square
				players[ii].setPosition(newPos);						// set the players position to be the new position
				currentPos = players[ii].getPosition();					// sets the current position
				
				cout << currentName << " is now on " << board[currentPos].getName() << endl; // outputs what they are on now
				
				if(board[currentPos].getName() == "Go To Jail")			// if they are on go to jail
				{
					cout << "Sorry! You're now in Jail!" << endl;
					players[ii].setInJail(true);						// set in jail
					currentPos = players[ii].getPosition();				// get new position
					system("PAUSE");									// pause console
					//break;											// break the else if
				}
				
				else if(board[currentPos].getOwner() == "USER")			// if the property is owned by no one and can be bought
				{
					
					cout << "This property can be purchased" << endl;
					purchase(ii, currentName, currentWallet, players, board[currentPos].getName(), currentPos, playerCount); // call purchase function
				}
				
				else if(board[currentPos].getOwner() != "USER" && board[currentPos].getOwner() != "NULL" && board[currentPos].getOwner() != currentName && board[currentPos].isMortgaged() == false) // if someone else owns the property
				{
					playerToBePaid = board[currentPos].getOwner();		// get the player name of the owner
					
					for (int player = 0; player < playerCount; player++)// checks the player number of the owner
					{
						if (players[player].getName() == playerToBePaid)
						{
							playerOwedTo = player;						// assigns the player number of the owner
							break;										// breaks out of for loop
						}
					}
										
					cout << board[currentPos].getName() <<  " is owned by " << playerToBePaid << "." << endl; // outputs the owner
					if(board[currentPos].getColour() == "Utility")	// if the tile is a utility
					{
						if(players[playerOwedTo].utilities == 2)	// if they own both utilities
						{
							rentOwed = sumOfDice * 10;				// calculates rent
						}
						
						else										// if they only own 1
						{
							rentOwed = sumOfDice * 4;				// calculates rent
						}
					}
					else											// if not a utility
					{
						rentOwed = board[currentPos].getCurrentRent();	// get rent of property
					}
					//rentOwed = 1300;
					cout <<  "You must pay $" << rentOwed << " to " << playerToBePaid << "." << endl; // outputs how much is to be paid
					playerCanPay(rentOwed, players, ii, false);			   // checks if player can afford the rent
					if (players[ii].getBankruptStatus() == true)		// if the player decided to go bankrupt	
					{
						cout << players[playerOwedTo].getName() << " receives all of your assets!" << endl; // output that the player that bankrupted you gets your assets
						players[playerOwedTo].addToWallet(currentWallet);					// add to wallet and total worth
						players[playerOwedTo].addToTotalWorth(currentWallet);				
						int amountOfCards = players[ii].getJailCards();
						for(int j = 0; j < amountOfCards; j++)
						{
							players[playerOwedTo].incrementJailCards();
							players[ii].decrementJailCards();
						}
						for(int i = 0; i < 40; i++)					// for loop that runs for the size of the board
						{
							if(board[i].getOwner() == currentName)	// if the owner of the current position is the current player
							{
								cout << playerToBePaid << " receives " << board[i].getName() << endl; // output what is being received
								board[i].setOwner(playerToBePaid);		// set the new owner
								string currentColour = board[i].getColour();	// get the colour of the property
								players[playerOwedTo].updateProperties(board); 	// update the properties of the player that is owed to
								players[playerOwedTo].addToColourCount(currentColour, players, playerOwedTo, board);	// add to the colour count for each new property
								int propertyWorth = board[i].getPrice();		// get the price of the property
								players[playerOwedTo].addToTotalWorth(propertyWorth);	// add to the players total worth
							}
						}
						system("PAUSE");
					}
					else if(players[ii].getBankruptStatus() == false)		// otherwise
					{
						players[playerOwedTo].addToWallet(rentOwed);		// adds money to the owners wallet and total worth
						players[playerOwedTo].addToTotalWorth(rentOwed);   
					}

					currentWallet = players[ii].getWallet();			// gets the current wallet of the player			
				}
				
				else if (board[currentPos].getName() == "Income Tax" || board[currentPos].getName() == "Super Tax") // if the tile is income or super tax
				{
					rentOwed = board[currentPos].getBaseRent();		// get the rent owed
					if(board[currentPos].getName() == "Income Tax")	// if it is income tax
					{						
						do											// give the options
						{
							cout << "A = Pay 10% of your total worth." << endl;
							cout << "B = Pay $" << rentOwed << "." << endl;
							cin >> incomeTaxPayment;
						}while(incomeTaxPayment != 'A' && incomeTaxPayment != 'B');
						
						if(incomeTaxPayment == 'B')
						{
							playerCanPay(rentOwed, players, ii, true);	 // checks if they can afford the rent
						}	
						
						else if (incomeTaxPayment == 'A')		// they chose 10%	
						{
							rentOwed = currentTotalWorth / 10;					// determines 10% of total worth (rounded), and checks if they can afford that 
							playerCanPay(rentOwed, players, ii, true);							
						}				
					}
					
					else										// if it is super tax
					{
						playerCanPay(rentOwed, players, ii, true); // check if they can afford rent
					}
				}
				
				else if(board[currentPos].getName() == "Community Chest") // if tile is community chest
				{
					drawCommunityChestCard(players, ii, playerCount, playerProperties); // draw a card
					currentPos = players[ii].getPosition();			// get new position in case it was modified
					currentWallet = players[ii].getWallet();		// gets new wallet and total worth
					currentTotalWorth = players[ii].getTotalWorth();					
				}
				
				else if(board[currentPos].getName() == "Chance")	// if tile is chance
				{
					drawChanceCard(players, ii, playerCount, currentPos, playerProperties);	//draw a chance card
					currentPos = players[ii].getPosition();		// get new position in case it was modified
					currentWallet = players[ii].getWallet();	// get new wallet and total worth
					currentTotalWorth = players[ii].getTotalWorth();
				}
				
				if(players[ii].isInJail() == false)
				{
					playerRolledDoubles = isDoubles(dice[0], dice[1]); // checks if the player rolled doubles
					if (playerRolledDoubles)						// if yes
					{
						players[ii].incrementDoublesCount();		// add one to the doubles count for the current player
						doublesThrown = players[ii].getDoublesCount(); // gets the new alue of doubles thrown
						if (doublesThrown == 3)			// if the player has thrown 3 doubles in a row
						{
							cout << "Whoops! 3 Doubles In A Row!" << endl;
							cout << "GO TO JAIL!" << endl;
							players[ii].setInJail(true);		// send them to jail
							currentPos = players[ii].getPosition(); // gets new position
						}
						
						else if(doublesThrown != 3) // if it isnt their third set of doubles in a row
						{
							cout << "You got doubles!" << endl;
							cout << "ROLL AGAIN" << endl;				
							i--;			// subtract one from i, so the loop adds one back and gives the same player another turn.
						}						
					}
				}				
				else if(doublesThrown > 0 && playerRolledDoubles == false) // if they didint roll doubles, but their doubles count was greater than one
				{
					players[ii].resetDoubles();			// reset the doubles count
					doublesThrown = 0;
				}
			}		
			if(players[ii].getBankruptStatus() == true) // if the player declared bankruptcy in any part above when calling other functions
			{
				continue;	  							// break out of for loop and continue with next player
			}		
			currentWallet = players[ii].getWallet(); 	// gets the current wallet
			for(int j = 0; j < 28; j++)					// gets the properties of the player in case they bought property
			{
				playerProperties[j] = players[ii].getProperties(j);
			}			
			players[ii].updateProperties(board);
			system("PAUSE");	
			do					// while loop to repeat the end of turn screen until the player declares bankruptcy or ends turn
			{
				userInput = endOfTurnScreen(ii, currentName, currentWallet, currentPos, players);
				if (userInput == 'X')															// if they chose to exit, terminate game
				{
					cout << "Terminating Game..." << endl;
					exit(0);
				}
				
				else if(userInput == 'T')														// if they chose to trade, call trade function
				{
					trade(players, ii, playerCount);
				}
				
				else if(userInput == 'M') 														// if they chose to manage properties, call manage function
				{
					manage(players, ii);
				}
				
				else if(userInput == 'D')														// if they declared bankrupt
				{
					cout << currentName << " is now bankrupt!" << endl;
					players[ii].declareBankrupt(board, true);
					while(players[ii].getJailCards() > 0)
					{
						if(communityChestCards.size() == 15)
						{
							communityChestCards.push_back("GET OF JAIL FREE. This card may be kept until needed or traded.");
						}
						else if(chanceCards.size() == 15)
						{
							chanceCards.push_back("GET OF JAIL FREE. This card may be kept until needed or traded.");
						}
						players[ii].decrementJailCards();
					}
					bankruptCount++;															// add one to bankrupt count
				}
				
				else if(userInput == 'C')														// if they used a card and were in jail
				{
					cout << "You used a Get Out Of Jail Free Card." << endl;
					players[ii].decrementJailCards();
					cout << "You have " << players[ii].getJailCards() << " left." << endl;
					players[ii].setInJail(false);												// set out of jail
				}
				
				else if (userInput == 'P')														// if they paid the fine and were in jail
				{
					cout << "You paid the fine!" << endl;
					playerCanPay(50, players, ii, true);										// check if they can afford it
					players[ii].setInJail(false);												// set out of jail
				}
			}while(userInput != 'E');
		}					
	}
	system("PAUSE");	
	return 0;
}

void primeRead()																				// function to check each of the files used
{
	// Sam
	ifstream propertyNameFile;																	// reads in the file of property names
	propertyNameFile.open("txts/propertyNames.txt");
	ifstream propertyPriceFile;																	// reads in the file of property prices
	propertyPriceFile.open("txts/propertyPrice.txt");	
	ifstream propertyRentFile;																	// reads in the file of property rent
	propertyRentFile.open("txts/propertyBaseRent.txt");
	ifstream propertyOwnerFile;																	// reads in the file of property owners
	propertyOwnerFile.open("txts/propertyOwner.txt");
	ifstream propertyRentHotelFile;																// reads in the file of property rent with a hotel
	propertyRentHotelFile.open("txts/propertyRentHotel.txt");
	ifstream propertyColourFile;																// reads in the file of property colours
	propertyColourFile.open("txts/propertyColour.txt");
	ifstream propertyHousesCostFile;															// reads in the file of property house costs
	propertyHousesCostFile.open("txts/propertyHouseCost.txt");
	ifstream propertyMortgageCostFile;															// reads in the file of property mortgage costs
	propertyMortgageCostFile.open("txts/propertyMortgageValue.txt");
	ifstream propertyUnmortgageCostFile;														// reads in the file of property unmortgage costs
	propertyUnmortgageCostFile.open("txts/propertyUnmortgageValue.txt");
	ifstream propertyRentOneFile;																// reads in the file of property rent with one house
	propertyRentOneFile.open("txts/propertyRentOneHouse.txt");
	ifstream propertyRentTwoFile;																// reads in the file of property rent with two houses
	propertyRentTwoFile.open("txts/propertyRentTwoHouse.txt");
	ifstream propertyRentThreeFile;																// reads in the file of property rent with three houses
	propertyRentThreeFile.open("txts/propertyRentThreeHouse.txt");	
	ifstream propertyRentFourFile;																// reads in the file of property rent with four houses
	propertyRentFourFile.open("txts/propertyRentFourHouse.txt");
	ifstream chanceCards;																		// reads in the file of chance cards
	chanceCards.open("txts/chance.txt");
	ifstream commChestCards;																	// reads in the file of community chest cards
	commChestCards.open("txts/communityChest.txt");
	
					
	string primer;																				// string for the primer, just a temprary storage for the lines in files
	int lineCount = 0;																			// resets the line count
    while(getline(propertyNameFile, primer))													// reads in every line from the file and stores it in the primer
	{
		lineCount++;																			// increment the lineCount
	}
	lineCountCheck(lineCount);																	// check the line count
	
	lineCount = 0;																				// repeat the above for every file
	while (getline(propertyPriceFile,primer))
	{
		lineCount++;		
	}
	lineCountCheck(lineCount);

	lineCount = 0;
	while (getline(propertyRentFile,primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);

	lineCount = 0;
	while(getline(propertyOwnerFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);

	lineCount = 0;
	while(getline(propertyColourFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyHousesCostFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyMortgageCostFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);

	lineCount = 0;
	while(getline(propertyUnmortgageCostFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyRentOneFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyRentTwoFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyRentThreeFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyRentFourFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;
	while(getline(propertyRentHotelFile, primer))
	{
		lineCount++;
	}
	lineCountCheck(lineCount);
	
	lineCount = 0;																		// resets line count
	while(getline(chanceCards, primer))													// reads in every line of the file and stores it in the primer
	{
		lineCount++;																	// increments the linecount
	}
	cardCheck(lineCount);																// checks the amount of lines in the card files
																						// uses a different function because the count is different
	lineCount = 0;
	while(getline(commChestCards, primer))
	{
		lineCount++;
	}
	cardCheck(lineCount);	
	
	propertyNameFile.close();
	propertyPriceFile.close();	
	propertyRentFile.close();
	propertyOwnerFile.close();
	propertyColourFile.close();
	propertyHousesCostFile.close();
	propertyMortgageCostFile.close();
	propertyUnmortgageCostFile.close();
	propertyRentOneFile.close();
	propertyRentTwoFile.close();
	propertyRentThreeFile.close();
	propertyRentFourFile.close();
	propertyRentHotelFile.close();
	chanceCards.close();
	commChestCards.close();
}

void lineCountCheck(int lineCount)
{
	//Sam
	if (lineCount != 40)															// if the lineCount does not eual 40
	{
		cout << "ERROR: Board Incomplete" << endl;									// display an error
		cout << "Terminating Game.." << endl;										// terminate the game
		system("PAUSE");															// pause console and wait for input
		exit(0);																	// exit program
	}
}

void cardCheck(int cards)															
{
	if(cards != 16)																	// if linecount does not equal 16
	{
		cout << "ERROR: Cards Incomplete" << endl;									// display error
		cout << "Terminating Game.." << endl;										// terminate game
		system("PAUSE");															// pause console and wait for input
		exit(0); 																	// exit program
	}
}
void assignPropAttr()																// function to assign attributes to the properties
{
	//Sam
	primeRead();																	// calls the prime read function
	ifstream propertyNames;															// reads in every file needed for the proeprties
	propertyNames.open("txts/propertyNames.txt");
	
	ifstream propertyPrices;
	propertyPrices.open("txts/propertyPrice.txt");
	
	ifstream propertyRents;
	propertyRents.open("txts/propertyBaseRent.txt");
	
	ifstream propertyOwners;
	propertyOwners.open("txts/propertyOwner.txt");
	
	ifstream propertyColours;
	propertyColours.open("txts/propertyColour.txt");
	
	ifstream propertyHouseCost;
	propertyHouseCost.open("txts/propertyHouseCost.txt");
	
	ifstream propertyMortgage;
	propertyMortgage.open("txts/propertyMortgageValue.txt");
	
	ifstream propertyUnmortgage;
	propertyUnmortgage.open("txts/propertyUnmortgageValue.txt");
	
	ifstream propertyR1;
	propertyR1.open("txts/propertyRentOneHouse.txt");
	
	ifstream propertyR2;
	propertyR2.open("txts/propertyRentTwoHouse.txt");
	
	ifstream propertyR3;
	propertyR3.open("txts/propertyRentThreeHouse.txt");
	
	ifstream propertyR4;
	propertyR4.open("txts/propertyRentFourHouse.txt");
	
	ifstream propertyRH;
	propertyRH.open("txts/propertyRentHotel.txt");
		
	string line;
	int value;
	
	int count = 0;												// resets the count
	while (getline(propertyNames,line))							// gets the next kline in the file, stores it in the line string
	{
		board[count].setName(line);								// assigns line to the aprropriate variable for the board position count 
		count++;												// increment count
	}
	
	count = 0;
	while (getline(propertyPrices,line))						// repeats for other files
	{
		value = atoi(line.c_str());								// for integers, it converts the line to an int
		board[count].setPrice(value);
		count++;
	}

	count = 0;
	while (getline(propertyRents,line))
	{
		value = atoi(line.c_str());
		board[count].setBaseRent(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyOwners,line))
	{
		board[count].setOwner(line);
		count++;
	}
	
	count = 0;
	while (getline(propertyColours,line))
	{
		board[count].setColour(line);
		count++;
	}
	
	count = 0;
	while (getline(propertyHouseCost,line))
	{
		value = atoi(line.c_str());
		board[count].setHouseCost(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyMortgage,line))
	{
		value = atoi(line.c_str());
		board[count].setMortgageValue(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyUnmortgage,line))
	{
		value = atoi(line.c_str());
		board[count].setUnmortgageValue(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyR1,line))
	{
		value = atoi(line.c_str());
		board[count].setR1House(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyR2,line))
	{
		value = atoi(line.c_str());
		board[count].setR2House(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyR3,line))
	{
		value = atoi(line.c_str());
		board[count].setR3House(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyR4,line))
	{
		value = atoi(line.c_str());
		board[count].setR4House(value);
		count++;
	}
	
	count = 0;
	while (getline(propertyRH,line))
	{
		value = atoi(line.c_str());
		board[count].setRHotel(value);
		count++;
	}
	
	for (int i = 0; i < 40; i++)
	{
		board[i].setCurrentRent(board[i].getBaseRent());
	}
	
	propertyNames.close();									// close each file
	propertyPrices.close();
	propertyRents.close();
	propertyOwners.close();
	propertyColours.close();
	propertyHouseCost.close();
	propertyMortgage.close();
	propertyUnmortgage.close();
	propertyR1.close();
	propertyR2.close();
	propertyR3.close();
	propertyR4.close();
	propertyRH.close();
}

player* assignPlayerAttr(int playerCount)					// assign player attributes
{
	//Sam
	player* allPlayers = new player[playerCount];			// creates an array that will be returned by the function
	string playerName;										// string to hold the name of the player just entered
	bool isValid;											// boolean value that determines whethere the name entered is valid or not
	string names[playerCount];								// creates an array of names entered
	
	for (int i = 0; i < playerCount; i++)					// for loop to be executed for each player
	{
		do													// do while loop that runs while isValid is false
		{
			isValid = true;									// sets isValid to be TRUE initially
			cout << "What is Player " << i +1 << "'s name?" << endl;	// asks for name
			cin >> playerName;											// input name
			isValid = isStringValid(playerName);						// checks if the string contains valid characters
			if(isValid == false)										// if it is not valid
			{
                if(playerName.size() <= 8)
                {       
				   cout << "ERROR: Name contained invalid character(s)." << endl;	// output error
                }
                else if(playerName.size() > 8)
                {
                     cout << "ERROR: Name must be 8 characters or less." << endl;     
                }
			}
			
			else if(isValid == true)						// if it is valid
			{
				for(int j = 0; j < playerCount; j++)		// check agaionst the other names entered into the aray of names
				{
					if (playerName == names[j])				// if it does equal a name already entered
					{
						cout << "ERROR: Names must be unique." << endl; // output error
						isValid = false;								// sets the validity to FALSE
					}
				}
			}
			
		}while(isValid == false);
		
		names[i] = playerName;			// sets the val;ue of names[i] to be the name entered
		allPlayers[i].setName(playerName);	// sets the name of the current player in their class
	}	
	return allPlayers;					// returns the array of allPlayers
}

int* rollDice()												// function to roll and output dice roll
{
	//Sam
	int* diceValues = new int[2];
	int roll;
	srand(time(0));														// used to generate random number for dice													// array to store the value of the dice rolls

	for (int dice = 0; dice < 2 ; dice++)								// for loop that generates two random numbers, one for each dice
    {		
	    roll = (rand() % 6 ) + 1;
	    diceValues[dice] = roll;										// stores the roll in an array
	}
																								
	if((diceValues[0] == 1 && diceValues[1] == 1 )||(diceValues[1] == 1 && diceValues[0] == 1))					// the rest of this function is for ascii art
	{																												// I have done every combination of numbers but
		cout << " ---------- " << "    ---------- " << endl;														// order is not important for the ascii output
		cout << "|          |" << "   |          |" << endl;														// eg: if i roll a 4 and a 3, it will output the
		cout << "|          |" << "   |          |" << endl;														// same ascii art for a 3 and a 4
		cout << "|    ()    |" << "   |    ()    |" << endl;										
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[0] == 1 && diceValues[1] == 2)||(diceValues[1] == 1 && diceValues[0] == 2))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[0] == 1 && diceValues[1] == 3)||(diceValues[1] == 1 && diceValues[0] == 3))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[0] == 1 && diceValues[1] == 4)||(diceValues[1] == 1 && diceValues[0] == 4))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if((diceValues[0] == 1 && diceValues[1] == 5)||(diceValues[1] == 1 && diceValues[0] == 5))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[0] == 1 && diceValues[1] == 6)||(diceValues[1] == 1 && diceValues[0] == 6))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}	
	
	else if((diceValues[1] == 2 && diceValues[0] == 2)||(diceValues[0] == 2 && diceValues[1] == 2))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 2 && diceValues[0] == 3)||(diceValues[0] == 2 && diceValues[1] == 3))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 2 && diceValues[0] == 4)||(diceValues[0] == 2 && diceValues[1] == 4))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 2 && diceValues[0] == 5)||(diceValues[0] == 2 && diceValues[1] == 5))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}			
	
	else if((diceValues[1] == 2 && diceValues[0] == 6)||(diceValues[0] == 2 && diceValues[1] == 6))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 3 && diceValues[0] == 3)||(diceValues[0] == 3 && diceValues[1] == 3))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 3 && diceValues[0] == 4)||(diceValues[0] == 3 && diceValues[1] == 4))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if((diceValues[1] == 3 && diceValues[0] == 5)||(diceValues[0] == 3 && diceValues[1] == 5))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if((diceValues[1] == 3 && diceValues[0] == 6)||(diceValues[0] == 3 && diceValues[1] == 6))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 4 && diceValues[0] == 4)||(diceValues[0] == 4 && diceValues[1] == 4))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 4 && diceValues[0] == 5)||(diceValues[0] == 4 && diceValues[1] == 5))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if((diceValues[1] == 4 && diceValues[0] == 6)||(diceValues[0] == 4 && diceValues[1] == 6))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
		
	else if((diceValues[1] == 5 && diceValues[0] == 5)||(diceValues[0] == 5 && diceValues[1] == 5))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |"<< endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if((diceValues[1] == 5 && diceValues[0] == 6)||(diceValues[0] == 5 && diceValues[1] == 6))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   | ()    () |"<< endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if((diceValues[1] == 6 && diceValues[0] == 6)||(diceValues[0] == 6 && diceValues[1] == 6))
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |"<< endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	return diceValues;	// returns the sum to the function calling it
}

bool isDoubles(int dice1, int dice2)	// checks if the dice roll was doubles
{
	//Sam
	if (dice1 == dice2)					// if the dice are equal
	{
		return true;					// return true
	}
	
	else								// if they are different
	{
		return false;					// return false
	}
}

int playerMenu()							// main menu function/ startup screen
{
	//Sam
	 string playerCountStr;				// string to hold playerCount
	 int playerCount;					// int to hold the player count
	 bool isNotValidDigits = false;		// bool to determine if the player count is a valid number
	 int size;							// size of the string
	 do
	 {
	 	 cout << "#####################################################" << endl;
 	 	 cout << "#    __  __           HASBRO(c)            _        #" << endl;
	     cout << "#   |  \\/  |                             | |        #" << endl;
	     cout << "#   | \\  / | ___  _ __   ___  _ __   ___ | |_   _   #" << endl;
	     cout << "#   | |\\/| |/ _ \\| '_ \\ / _ \\| '_ \\ / _ \\| | | | |  #" << endl;
	     cout << "#   | |  | | (_) | | | | (_) | |_) | (_) | | |_| |  #" << endl;
	     cout << "#   |_|  |_|\\___/|_| |_|\\___/| .__/ \\___/|_|\\__, |  #" << endl;
	     cout << "#                            | |             __/ |  #" << endl;
	     cout << "#                            |_|            |___/   #" << endl;
	     cout << "#   2 - 6 Players                                   #" << endl;
	     cout << "#####################################################" << endl;
	     cout << "#                                                   #" << endl;
	     cout << "#            How Many Players Are There?            #" << endl;	
	     cout << "#                                                   #" << endl;
	     cout << "#####################################################" << endl;
	     cin >> playerCountStr;						// input the player count as a string
	     size = playerCountStr.size();				// determines the size of the string
	     
	     
	     for (int i = 0; i < size; i++)				// for loop that runs while i is less than the size of the string
	     {
	     	if (playerCountStr[i] != '2' && playerCountStr[i] != '3' && playerCountStr[i] != '4' && playerCountStr[i] != '5' && 
				playerCountStr[i] != '6')			// if the string entered is not one of the numbers listed 
			{
				isNotValidDigits = true;			// the string is invalid
				break;
			}
			else									// if it is any of those digits
			{
				isNotValidDigits = false;			// the string is valid
			}			
		 }
		 if(size > 1)
		 {
		 	isNotValidDigits = true;
		 }
		 if(isNotValidDigits == false)				// if the string is valid
	     {
			playerCount = atoi(playerCountStr.c_str());	// convert the string to an int
		 }
		 
		 if(isNotValidDigits == true)
		 {
		 	errorMenuScreen();						// if it isnt valid, output an error screen
		 }
		 	 	
	 }while(isNotValidDigits == true);				// loop executes until the string is invalid
	 return playerCount;     
}

void errorMenuScreen()							// Outputs the error screen for an invalid digit
{
	// Sam
	 system("cls");
	 cout << "#####################################################" << endl;
 	 cout << "#    __  __           HASBRO(c)            _        #" << endl;
     cout << "#   |  \\/  |                             | |        #" << endl;
     cout << "#   | \\  / | ___  _ __   ___  _ __   ___ | |_   _   #" << endl;
     cout << "#   | |\\/| |/ _ \\| '_ \\ / _ \\| '_ \\ / _ \\| | | | |  #" << endl;
     cout << "#   | |  | | (_) | | | | (_) | |_) | (_) | | |_| |  #" << endl;
     cout << "#   |_|  |_|\\___/|_| |_|\\___/| .__/ \\___/|_|\\__, |  #" << endl;
     cout << "#                            | |             __/ |  #" << endl;
     cout << "#                            |_|            |___/   #" << endl;
     cout << "#   2 - 6 Players                                   #" << endl;
     cout << "#####################################################" << endl;
     cout << "#                                                   #" << endl;
     cout << "#           Error: Must Be A Valid Digit            #" << endl;	
     cout << "#                                                   #" << endl;
     cout << "#####################################################" << endl;
     system("PAUSE");
     system("cls");
	
}

char startOfTurnScreen(int playerNo, string name, int wallet, int position, player* players) // screen to be displayed at the start of a turn
{
	//Peter and Sam
	system("cls");
	char userDecision;					// char to hold the user decision
	string posName = board[position].getName();
	wallet = players[playerNo].getWallet();
	if(players[playerNo].isInJail() == true)
	{
		posName = "Jail";
	}
	
	cout << "######################################################################" << endl;
	cout << "#    ";
	cout << left << setw(10) << "TURN";
	cout << left << setw(10) << "NAME";
	cout << left << setw(10) << "WALLET";
	cout << left << setw(30) << "POSITION";
	cout << "    #" << endl;
	cout << "#                                                                    #" << endl;
	cout << "#    ";
	cout << left << setw(10) << turnCount; 
	cout << left << setw(10) << name;
	cout << left << setw(1) << "$" << setw(9) << wallet;
	cout << left << setw(30) << posName;
	cout << "    #" << endl;
	cout << "######################################################################" << endl; 
	
	if (players[playerNo].isInJail() == false)	// if the player is not in jail
	{
		do		// to be done until the player enters a valid character
		{
			cout << "D = Declare Bankrupt" << endl;		// outputs the options for the turn
			cout << "R = Roll Dice" << endl;
			cout << "X = Exit Game" << endl;
			cout << "T = Trade" << endl;
			cout << "M = Manage" << endl;
			cin >> userDecision;
			
		}while(userDecision != 'D' && userDecision != 'R' && userDecision != 'X' && userDecision != 'T'  && userDecision != 'M');
	}
	
	else if(players[playerNo].isInJail() == true) // if the player is in jail
	{
		players[playerNo].incrementTurnsInJail();	// increment the turns they have had in jail
		do			// to be done until the player enters a valid character
		{
			cout << "D = Declare Bankrupt" << endl;		// outputs options for the turn
			cout << "R = Roll For Doubles" << endl;
			cout << "P = Pay $50 Bail" << endl;
			if(players[playerNo].hasJailFreeCards() == true) // if the player has get out of jail free cards, output this
			{
				cout << "C = Use Get Out Of Jail Free Card" << endl;
			}
			cout << "X = Exit Game" << endl;
			cout << "T = Trade" << endl;
			cout << "M = Manage" << endl;
			cin >> userDecision;
			
			if(players[playerNo].hasJailFreeCards() == true && userDecision == 'C') // if they have cards, and choose c
			{
				return userDecision;	// return C
			}
			
		}while(userDecision != 'D' && userDecision != 'R' && userDecision != 'X' && userDecision != 'T' && userDecision != 'P'&& userDecision != 'M');
	}	
	return userDecision; //returns the users decision to the fun ction calling it
}

char endOfTurnScreen(int playerNo, string name, int wallet, int position, player* players) // exactly the same as the above, some options changed however
{
	//Peter and Sam
	system("cls");
	string posName = board[position].getName();	
	wallet = players[playerNo].getWallet();
	if(players[playerNo].isInJail() == true)
	{
		posName = "Jail";
	}
		
	cout << "######################################################################" << endl;
	cout << "#    ";
	cout << left << setw(10) << "TURN";
	cout << left << setw(10) << "NAME";
	cout << left << setw(10) << "WALLET";
	cout << left << setw(30) << "POSITION";
	cout << "    #" << endl;
	cout << "#                                                                    #" << endl;
	cout << "#    ";
	cout << left << setw(10) << turnCount; 
	cout << left << setw(10) << name;
	cout << left << setw(1) << "$" << setw(9) << wallet;
	cout << left << setw(30) << posName;
	cout << "    #" << endl;
	cout << "######################################################################" << endl;
	
	char userDecision;
	if (players[playerNo].isInJail() == false)
	{
		do
		{
			cout << "D = Declare Bankrupt" << endl;
			cout << "E = End Turn" << endl;
			cout << "X = Exit Game" << endl;
			cout << "T = Trade" << endl;
			cout << "M = Manage" << endl;
			cin >> userDecision;
			
		}while(userDecision != 'D' && userDecision != 'E' && userDecision != 'X' && userDecision != 'T' && userDecision != 'M');
	}
	
	else if(players[playerNo].isInJail() == true)
	{
		do
		{
			cout << "D = Declare Bankrupt" << endl;
			cout << "E = End Turn" << endl;
			if(players[playerNo].hasJailFreeCards() == true && players[playerNo].getTurnsInJail() > 0)
			{
				cout << "C = Use Get Out Of Jail Free Card" << endl;
			}
			cout << "X = Exit Game" << endl;
			cout << "T = Trade" << endl;
			cout << "M = Manage" << endl;
			cin >> userDecision;
			
			if(players[playerNo].hasJailFreeCards() == true && userDecision == 'C')
			{
				return userDecision;
			}
			
		}while(userDecision != 'D' && userDecision != 'E' && userDecision != 'X' && userDecision != 'T'&& userDecision != 'M');
	}	
	
	return userDecision;
}

void purchase(int playerNo, string name, int wallet, player* players, string propertyName, int currentPos, int playerCount)	// function to purchase property
{
	//Sam
	char userDecision;		// char for the users decision
	int price = board[currentPos].getPrice();	//price of the property being bought
	bool canPay = (wallet >= price);	// bool to determine if they can afford the property
	string colour = board[currentPos].getColour(); // stores the colour of the property
				
	do	// asks for decision as to whethere they are buying it or not
	{
		cout << "Do you want to buy " << propertyName << " for $" << price << "? (Y/N)" << endl;
		cin >> userDecision;
	}while (userDecision != 'Y' && userDecision != 'N');
	
	if(userDecision == 'Y' && canPay == true)	// if they choose to buy it, and can afford it
	{
		playerCanPay(price, players, playerNo, true);	// calls the payment function, which subtracts the moeny owed from their wallet 
		cout << "CONGRATULATIONS! You bought " << propertyName << "!" << endl;	// outputs that they bought the property
		board[currentPos].setOwner(name);	// sets the new owner to be the player
		players[playerNo].addToColourCount(colour, players, playerNo, board); // adds to the appropriate colour count	
	}
	
	else if(userDecision == 'Y' && canPay == false) // if the user cannot afford to pay, but wishes to
	{	
		// informs the user that they cant afford it, and gives them the optuion to auction or manage
		cout << "You cant afford that!" << endl;
		do
		{
			cout << "A = Auction Property" << endl;
		    cout << "M = Manage Properties" << endl;
		    cin >> userDecision;
		}while(userDecision != 'A' && userDecision != 'M');
		
		if(userDecision == 'A') // if they chose A, call the auction function
		{
			auction(players, currentPos, playerCount);
		}
		else	// otherwise, call the paying function
		{
			playerCanPay(price, players, playerNo, true);	
		}
	}
	
	else if(userDecision == 'N')
	{
		auction(players, currentPos, playerCount);
	}
}

void playerCanPay(int moneyOwed, player* players, int playerNo, bool bankPayment) // checks if the player can afford a property
{
	bool hasPaid; // boolean to store whther the player has paid in full or not
	int originalMoneyOwed = moneyOwed;	// copies the money owed to another variable
	int moneyPaid = 0;	// sets the money that has been paid to 0
	int extraMoney = 0;	// sets the extra money from managing to 0
	char userDecision;	// char for the suer decision
	int startingWallet = players[playerNo].getWallet();
	int updatedWallet;
	
	if(players[playerNo].getWallet() < originalMoneyOwed) // if they cannot afford it
	{
		do	// do this while hasPaid is equal to false
		{
			if(moneyPaid < originalMoneyOwed)	// if money paid is less than the money that is owed
			{
				hasPaid = false;	// set has paid to false
				cout << "You owe $" << moneyOwed << "." << endl; // output how much they owe
				cout << "You can mortgage properties and sell houses to get money, using the manage option in the menu." << endl; // outputs their options
				
				do
				{
					cout << "M = Manage" << endl;
					cout << "D = Declare Bankruptcy" << endl;
					cin >> userDecision;
				}while(userDecision != 'M' && userDecision != 'D');
				
				if(userDecision == 'M')	// if they called the manage function
				{
					manage(players, playerNo);  
				}
				
				else if(userDecision == 'D') // if they declared bankruptcy
				{
					cout << players[playerNo].getName() << " is now bankrupt!" << endl; // declare the player bankrupt and output that they are
					players[playerNo].declareBankrupt(board, false);
					
					if(bankPayment == true)
					{
						while(players[playerNo].getJailCards() > 0)
						{
							if(communityChestCards.size() == 15)
							{
								communityChestCards.push_back("GET OF JAIL FREE. This card may be kept until needed or traded.");
							}
							else if(chanceCards.size() == 15)
							{
								chanceCards.push_back("GET OF JAIL FREE. This card may be kept until needed or traded.");
							}
							players[playerNo].decrementJailCards();
						}
					}
					
					bankruptCount++; // increment the bankrupt count
					break;
				}				
			}
			
			else if(moneyPaid >= originalMoneyOwed)		// if the money paid is more than what is owed
			{
				hasPaid = true;							// set hasPaid to true
				if(moneyPaid > originalMoneyOwed)		
				{
					extraMoney = (moneyPaid - originalMoneyOwed); // get the extra money paid
				}
			}
			
			updatedWallet = players[playerNo].getWallet();		// gets the players wallet
			moneyPaid = updatedWallet - startingWallet;			// determines how much money has been paid
			moneyOwed = originalMoneyOwed - moneyPaid;			// determine how much money is stoll owed
		}while(hasPaid == false);
	}
	
	
	
	if(players[playerNo].getBankruptStatus()== false)			// if they are not bankrupt
	{
		if(bankPayment == true) // if the transsaction is with the banka chance or community card
		{
			cout << "You paid $" << originalMoneyOwed << "." << endl; // output how much was paid
		}
		players[playerNo].subtractFromWallet(originalMoneyOwed); // subtrct money from the wallet
		players[playerNo].subtractFromTotalWorth(originalMoneyOwed); //subtract money from the total worth
		players[playerNo].addToWallet(extraMoney);	// add to wallet the extra money
		players[playerNo].addToTotalWorth(extraMoney);	// add to the total worth the extra money
	}	
}

void auction(player* players, int currentPos, int playerCount) // function for the auctioning of properties
{
	for(int k = 0; k < playerCount; k++)	// sets each player to have not passed or forfeited
	{
		players[k].forfeit(false);
		players[k].passed(false);
	}
	
	int highestBid = 0; // stores the highest bid
	string winner;		// stores the winner
	string strAmount;	// stores a bid ina  string
	bool isNotDigits;	// bool for if the string is valid or not
	int bid = 0;		// stores the current bid
	int forfeitCount = 0; // the players that have forfeited
	char userDecision;	// what the user decides to do 
	int count = 0;		// counter for the players
	bool canPay;		// determines whethere the player can afford it
	int passedThisTurn = 0; // sets the players passed this turn to 0
	int bidTurn = 0;	// cout for the bid turn
	int playersStillBidding = playerCount;	// sets the playersStillBidding to be equal to the player count
	bool keepAuctioning = true;	// bool to tell the while loop to keep going
	int wallet;	// stores the wallet of the current player
	string colour = board[currentPos].getColour(); // stores the colour of the proeprty being auctioned
	int size; // stores the size of the string entered as a bid
	
	while(keepAuctioning == true)	// while this bool is true
	{
		system("PAUSE");
		system("cls");
				
		while(players[count].hasForfeited() == true) // while the current player has forfeited
		{
			count++;	// increment the count
			if(count == playerCount) // if the count = playerCount, reset the count to 0
			{
				count = 0;	
			}
		}
				
		canPay = true;	// sets the value of canPay to true
		wallet = players[count].getWallet();
		cout << "######################################################################" << endl;
		cout << "#    ";
		cout << left << setw(13) << "HIGHEST BID";
		cout << left << setw(10) << "BIDDING";
		cout << left << setw(10) << "WALLET";
		cout << left << setw(27) << "PROPERTY";
		cout << "    #" << endl;
		cout << "#                                                                    #" << endl;
		cout << "#    ";
		cout << left << setw(1) << "$" << setw(12) << highestBid; 
		cout << left << setw(10) << players[count].getName();
		cout << left << setw(1) << "$" << setw(9) << wallet;
		cout << left << setw(27) << board[currentPos].getName();
		cout << "    #" << endl;
		cout << "######################################################################" << endl;
		
		/*		
		cout << "Bid Turn: " << bidTurn << endl; // outputs the bid turn, the proeprty being auctioned, the price originally, the highest bid, the player bidding, and their wallet 
		cout << "Now Auctioning: " << board[currentPos].getName() << endl;
		cout << "Original Price: $" << board[currentPos].getPrice() << endl;		
		cout << "Highest Bid: $" << highestBid << endl;		
		cout << "Now Bidding: " << players[count].getName() << endl;
		cout << "Wallet: $" << wallet << endl;
		*/ 
		if(highestBid > wallet)	// if the highest bid is greater than their wallet, forfeit them
		{
			cout << "Sorry, you cant afford to keep bidding!" << endl;
			userDecision = 'F';
		}
		
		else if (highestBid < wallet) // if highest bid is less than the wallet
		{
			do	// do this until a valid character is entered
			{
				cout << "F = Forfeit" << endl;
				cout << "P = Pass" << endl;
				cout << "B = Bid" << endl;
				cin >> userDecision;
			}while(userDecision != 'F' && userDecision != 'P' && userDecision != 'B');
		}		
		
		if(userDecision != 'P')	// if they didnt choose PASS
		{
			passedThisTurn = 0;	//reset the passed this turn count
			for(int i = 0; i < playerCount; i++)	//sets players that havent forfeited to also not have passed
			{
				if(players[i].hasForfeited() == false)
				{
					players[i].passed(false);
				}				
			}
		}
		if (userDecision == 'B') // if they chose to bid
		{
			do	// do this loop, while bid is less than highest bid, or the number is invalid, or the bid entered is 0, or the player cannot afford their bid
			{
				if (bid < highestBid) // throws an error if the bid is less than the highest bid
				{
					cout << "ERROR: Your bid must be greater than the highest bid." << endl;
				}
				
				else if (isNotDigits == true)		// throws an erro if it was invalid bid
				{
					cout << "ERROR: Your bid contained non digit characters." << endl;
				}
				
				if(canPay == false)		// throws error if they cannot afford their bid
				{
					cout << "ERROR: Bid exceeds wallet value." << endl;
				}			
				
				cout << "Enter Your Bid:" << endl; // asks for input
				cin >> strAmount;		// input string amount
				size = strAmount.size();	// get the size of the string
				
				for (int m = 0; m < size; m++)	// checks if the character in string is valid
		        {
		        	if (strAmount[m] != '1' && strAmount[m] != '2' && strAmount[m] != '3' && strAmount[m] != '4' && strAmount[m] != '5' && 
					    strAmount[m] != '6' && strAmount[m] != '7' && strAmount[m] != '8' && strAmount[m] != '9' && strAmount[m] != '0')
				    {
					   isNotDigits = true; // if it isnt one pf the above characters, break and set the invalidity to true
					   break;
				    }
				    
				    else // otherwise, set it the invalidity to false
				    {
					   isNotDigits = false;
				    }			
			    }
			 
			    if(isNotDigits == false) // if the string was valid
		        {
				   bid = atoi(strAmount.c_str()); // copnvert it to an int
			    }
			    
			    canPay = (wallet >= bid); // checks if they can pay
				
			}while(bid < highestBid || isNotDigits == true || bid == 0 || canPay == false);
			
			highestBid = bid; // sets the highest bid to be the new bid
		}
		
		else if (userDecision == 'F') // if they chose to forfeit
		{
			cout <<  players[count].getName() <<" forfeited!" << endl; // output that they forfeited
			players[count].forfeit(true); // set them to be forfeited
			playersStillBidding--; // decrement the players still bidding
			forfeitCount++; // increment the forfeit count
		}
		
		else if(userDecision == 'P') // if they passed
		{
			cout <<  players[count].getName() << " passed!" << endl; //output that they passed
			players[count].passed(true);		// set them to be passed
			passedThisTurn++; // incremement the players passed this turn
		}
		
		// IF ANY OF THE CONDITIONS ARE MET THAT CAUSE THE AUCTION TO END
		if((passedThisTurn == (playersStillBidding - 1) && highestBid > 0) || (passedThisTurn == playerCount && highestBid == 0) ||
		   (forfeitCount == (playerCount - 1) && highestBid > 0) || (forfeitCount == playerCount && highestBid == 0))
		{
			keepAuctioning = false; // set the keep auctioning bool to false
		}
		
		count++;	// increment the count
		if(count == playerCount && keepAuctioning == true) // if the count is equal to the player count and keep auctioning is equal to TRUE
		{
			count = 0;	
			bidTurn++;
		}		
	}
	//if no one bids
	if((passedThisTurn == playerCount && highestBid == 0 )||( forfeitCount == playerCount && highestBid == 0))
	{
		cout << "No Bids! Bank retains " << board[currentPos].getName() << "." << endl; // the bank keeps the property
	}
		
	else if((forfeitCount == (playerCount -1) && highestBid > 0) || (passedThisTurn == (playersStillBidding - 1) && highestBid > 0)) // if EITHER OF THESE CONDITIONS
	{
		for(int j = 0; j < playerCount; j++)
		{
			if(players[j].hasForfeited() == false && players[j].hasPassed() == false) // DETERMINE WHICH PLAYER IS THE WINNER
			{
			 	winner = players[j].getName();		// set the winners name
			 	cout << winner << " won the auction!" << endl; // output the winner
			 	players[j].subtractFromWallet(highestBid);	//take money from their wallet
			 	players[j].subtractFromTotalWorth(highestBid);	//take money from their total worth
			 	board[currentPos].setOwner(winner);	//set the owner of the auctioned property to be the winner
			 	players[j].addToColourCount(colour, players, j, board); // adds to the appropriate colour
			 	players[j].addToTotalWorth(board[currentPos].getPrice());
			 					
				players[j].updateProperties(board);	//add to the properties array
			 	break;
			}
		}
	}
		
	system("PAUSE");
	system("cls");
}

void setChanceCards() // sets the chance cards
{
	string card; // string to store the current card
	int count = 0;	// counter for each card
	string temp;	//temporary string to be output to the console
	int index;		// store the index, which is a random number between 1 and 16
	
	srand(time(0)); // seed for random number
	ifstream chanceCardsFile;	// open the chance file
	chanceCardsFile.open("txts/chance.txt");
	
	while(getline(chanceCardsFile, card))	// assigns the value in the array to the current line
	{
		chanceCards.push_back(card);
		//chanceCards[count] = card;
		count++;
	}
	
	for(unsigned int i = 0; i < 16; i++)	// shuffles the cards
	{
		index = rand() % 15;	// gets a random number
		temp = chanceCards[i];	// sets temp to be the current card
		chanceCards[i] = chanceCards[index];	// swaps the cards
		chanceCards[index] = temp;	// sets the random card to be the old card
	}
	chanceCardsFile.close();	// closes the file
}

void setCommunityChestCards() //sets the community cards
{							  // exactly the same as the function to set and shuffle the chance cards, but with the community chest cards
	string card;
	int count = 0;
	string temp;
	int index;
	
	srand(time(0));
	ifstream commChestCards;
	commChestCards.open("txts/communityChest.txt");
	
	while(getline(commChestCards, card))
	{
		communityChestCards.push_back(card);
		//communityChestCards[count] = card;
		count++;
	}
	
	for(unsigned int i = 0; i < 16; i++)
	{
		index = rand() % 15;
		temp = communityChestCards[i];
		communityChestCards[i] = communityChestCards[index];
		communityChestCards[index] = temp;
	}	
	
	commChestCards.close();
}

void drawChanceCard(player* players, int playerNo, int playerCount, int currentPos, int* properties) // function to draw chance cards
{	
	string temp;		// temprorary string for output of card
	string ownerName;	// owner of a property
	string buyerName;	//buyer of a property
	int buyerWallet;	// wallet of the buyer
	string propertyName;	// name of the property
	int owner;	//integer for the player number of the owner
	int housesOnProperty;	// the amount of houses on a property
	int houseCount = 0;	//counts the houses on all proeprties in colour group
	int hotelCount = 0;	// counts the hotels on all properties in colour group
	int totalCost = 0;	// total cost of repairs
	int rentOwed;	// the amount of rent that is owed
	int sumOfDice;	// uinteger to store the sum of two dice
	
	temp = chanceCards[0];
	for(unsigned int i = 0; i < chanceCards.size()-1; i++)
	{
		chanceCards[i] = chanceCards[i+1];	// moves each card along one
	}	
	chanceCards[chanceCards.size()-1] = temp;	// puts card that was drawn at the end of the array (bottom of deck)
	
	//cout << "CHANCE: " << temp << endl;		// outputs the chance card drawn
	if(temp == "ADVANCE TO PALL MALL. IF YOU PASS GO, COLLECT $200.")	// if this card
	{
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     ADVANCE TO PALL MALL.      |" << endl;
		cout << "|        IF YOU PASS GO,         |" << endl;
		cout << "|         COLLECT $200           |" << endl;
		cout << "|________________________________|" << endl;
		
		if(currentPos != 7)						// if the player is on any chance that isnt position number 7
		{
			cout << "You passed GO! Collect $200." << endl;	// they must pass go
			players[playerNo].addToTotalWorth(200);	// add $200 to wallet and total worth
			players[playerNo].addToWallet(200);
		}
		
		players[playerNo].setPosition(11);		// set position to be pall mall
		
	}
	
	else if(temp == "GET OF JAIL FREE. This card may be kept until needed or traded.")	// gives a get out of jail free card
	{
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     GET OUT OF JAIL FREE.      |" << endl;
		cout << "|     THIS CARD MAY BE KEPT      |" << endl;
		cout << "|     UNTIL NEEDED OR TRADED     |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].incrementJailCards();			// incerments players jail cards
		players[playerNo].setHasJailCards(true);		// assigns a true value to the hasJaiLCards variable
		//chanceCards[15] = "NULL";						// sets the last card in the array to now equal "NULL".
		chanceCards.pop_back();
	}
	
	else if(temp == "MAKE GENERAL REPAIRS ON ALL YOUR PROPERTY: FOR EACH HOUSE PAY $25, FOR EACH HOTEL PAY $100.")
	{
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|   MAKE GENERAL REPAIRS ON ALL  |" << endl;
		cout << "|  YOUR PROPERTY: FOR EACH HOUSE |" << endl;
		cout << "|PAY $25, FOR EACH HOTEL PAY $100|" << endl;
		cout << "|________________________________|" << endl;
		
		for (int i = 0; i < 28; i++)		// checks the properties of the player for houses or hotels
		{
			if(properties[i] != 99)			// if the property is not 99, which is the sentinel
			{
				housesOnProperty = board[properties[i]].getHouses();	// gets the houses on the property
				if(housesOnProperty == 5)	// if the houses is 5, then it is a hotel
				{
					hotelCount++;	//increment hotel count
				}
				
				else
				{
					houseCount += housesOnProperty;		//add to the house count
				}
			}			
		}
		
		totalCost = (houseCount* 25) + (hotelCount*100);		// calculates the total cost of repairs
		playerCanPay(totalCost, players, playerNo, true);		// checks if they can afford the repairs
	}
	
	else if(temp == "BANK PAYS YOU DIVIDEND OF $50.") 
	{
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|         BANK PAYS YOU          |" << endl;
		cout << "|        DIVIDEND OF $50         |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToTotalWorth(50);					// add $50 to wallet and total worth
		players[playerNo].addToWallet(50);
	}
	
	else if(temp == "ADVANCE TO GO. (COLLECT $200)")
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|         ADVANCE TO GO          |" << endl;
		cout << "|         COLLECT $200           |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].setPosition(0);						// set position to be GO
		players[playerNo].addToTotalWorth(200);					// add $200 to wallet and total worth
		players[playerNo].addToWallet(200);
	}
	
	else if(temp == "GO TO JAIL. GO DIRECTLY TO JAIL, DO NOT PASS GO, DO NOT COLLECT $200.")
	{
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|    GO TO JAIL. GO DIRECTLY     |" << endl;
		cout << "|    TO JAIL, DO NOT PASS GO,    |" << endl;
		cout << "|      DO NOT COLLECT $200       |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].setInJail(true);			// set the player to be in jail
	}
	else if(temp == "ADVANCE TO THE NEAREST UTILITY. IF UNOWNED, YOU MAY BUY IT FROM THE BANK. IF OWNED, THROW DICE AND PAY OWNER 10 TIMES THE AMOUNT THROWN.") 
	{
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|             CHANCE             |" << endl;
		cout << "| ADVANCE TO THE NEAREST UTILITY |" << endl;
		cout << "|IF UNOWNED, YOU MAY BUY IT FROM |" << endl;
		cout << "| THE BANK. IF OWNED, THROW DICE |" << endl;
		cout << "|   AND PAY OWNER 10 TIMES THE   |" << endl;
		cout << "|         AMOUNT THROWN          |" << endl;
		cout << "|________________________________|" << endl;
		
		if(currentPos == 7 || currentPos == 36)		// if the player is on either of these positions
		{
			if(currentPos == 36)					// if the player is on this specific positon
			{
				cout << "You passed GO! Collect $200." << endl;			// add $200 to wallet and total worth
				players[playerNo].addToTotalWorth(200);
				players[playerNo].addToWallet(200);
			}
			players[playerNo].setPosition(12);		// set the current position to be this utility
		}
		
		else if(currentPos == 22)				// if the position is this 
		{
			players[playerNo].setPosition(28);	// set the new position to this
		}
		currentPos = players[playerNo].getPosition();	// get the players new position
		ownerName = board[currentPos].getOwner();		// get the owner of the property
		
		if(ownerName != "NULL" && ownerName != players[playerNo].getName() && ownerName != "USER") // if the property is owned by someone
		{
			for (int i = 0; i < playerCount; i++)		// find the owner
			{
				if (players[i].getName() == ownerName)
				{
					owner = i;			// set the owner
					break;				// exit for loop
				}
			}
			
			cout << board[currentPos].getName() << " is owned by " << owner << endl;	// output who it is  owned by
			int* dice = rollDice();														// roll the dice
			sumOfDice = dice[0] + dice[1];												// calculate the sum of the dice
			rentOwed = sumOfDice * 10;													// mulitply the sum by 10
			
			
			cout << "You owe " << players[owner].getName() << " $" << rentOwed << "." << endl;				// output the rent that is owed
			playerCanPay(rentOwed, players, playerNo, false);							// call the paying function
			if (players[playerNo].getBankruptStatus() == true)							// IF THEY WENT BANKRUPT
			{
				cout << players[owner].getName() << " receives all of your assets!" << endl;	// ooutput who gets their stuff
				int currentWallet = players[playerNo].getWallet();			// get their current wallet
				players[owner].addToWallet(currentWallet);					// add to the owners wallet and total worth
				players[owner].addToTotalWorth(currentWallet);
				int amountOfCards = players[playerNo].getJailCards();
						
			    for(int j = 0; j < amountOfCards; j++)
			    {
			        players[owner].incrementJailCards();
			        players[playerNo].decrementJailCards();
			    }
				
				for(int i = 0; i < 40; i++)					// for loop that runs for the size of the board
				{
					if(board[i].getOwner() == players[playerNo].getName())		// if the owner of the position is the current player
					{
						cout << players[owner].getName() << " receives " << board[i].getName() << endl; // assign assets to the player that bankrupted them
						board[i].setOwner(players[owner].getName());
						string currentColour = board[i].getColour();
						players[owner].updateProperties(board);
						players[owner].addToColourCount(currentColour, players, owner, board);
					}
				}
				system("PAUSE");
			}
			players[owner].addToTotalWorth(rentOwed);									// add to the owners money and total worth
			players[owner].addToWallet(rentOwed);			
		}
		
		else if(ownerName == "USER")													// if it is unowned
		{
			buyerName = players[playerNo].getName();									// get the name of the player, thjeir wallet and the name of the property
			buyerWallet = players[playerNo].getWallet();
			propertyName = board[currentPos].getName();
			purchase(playerNo, buyerName, buyerWallet, players, propertyName, currentPos, playerCount); // call the purchase function
		}
	}
	else if(temp == "ADVANCE TO TRAFALGAR SQUARE, IF YOU PASS GO, COLLECT $200.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|  ADVANCE TO TRAFALGAR SQUARE,  |" << endl;
		cout << "|        IF YOU PASS GO,         |" << endl;
		cout << "|         COLLECT $200           |" << endl;
		cout << "|________________________________|" << endl;
		
		if(currentPos == 36)			// if this is the current position
		{
			cout << "You passed GO! Collect $200." << endl;		// add $200 to wallet and total worth
			players[playerNo].addToTotalWorth(200);
			players[playerNo].addToWallet(200);
		}
		
		players[playerNo].setPosition(24);		// set the new position
	
	}
	else if(temp == "YOUR BUILDING LOAN MATURES. COLLECT $150.") 	
	{
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|   YOUR BUILDING LOAN MATURES.  |" << endl;
		cout << "|         COLLECT $150           |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToTotalWorth(150);			// add $150 to wallet and total worth
		players[playerNo].addToWallet(150);
	}
	else if(temp == "TAKE A TRIP TO KINGS CROSS STATION. IF YOU PASS GO, COLLECT $200.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|   TAKE A TRIP TO KINGS CROSS   |" << endl;
		cout << "|    STATION, IF YOU PASS GO,    |" << endl;
		cout << "|          COLLET $200           |" << endl;
		cout << "|________________________________|" << endl;
		
		cout << "You passed GO! Collect $200." << endl;			
		players[playerNo].addToTotalWorth(200);				// add $200 to wallet and total worth
		players[playerNo].addToWallet(200);
		
		players[playerNo].setPosition(5);			// set new position
	}
	else if(temp == "ADVANCE TO MAYFAIR.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________" << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|       ADVANCE TO MAYFAIR       |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].setPosition(39);			// set new position
	}
	else if(temp == "YOU HAVE BEEN ELECTED CHAIRMAN OF THE BOARD. PAY EACH PLAYER $50.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "| YOU HAVE BEEN ELECTED CHAIRMAN |" << endl;
		cout << "|         OF THE BOARD.          |" << endl;
		cout << "|      PAY EACH PLAYER $50.      |" << endl;
		cout << "|________________________________|" << endl;
		
		for(int i = 0; i < playerCount; i++)	// add mopnmey to each players wallet
		{
			if (i == playerNo)					// if the current iteration is equal to the player number, continue and increment i
			{
				continue;
			}
			totalCost += 50;					// add to the total cost
			players[i].addToWallet(50);			// add $50 to the other players wallets
			players[i].addToTotalWorth(50);
		}		
		playerCanPay(totalCost, players, playerNo, true);	// check if the player can afford to pay each player $50
	}
	
	else if(temp == "SPEEDING FINE $15.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|       SPEEDING FINE $15        |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		playerCanPay(15, players, playerNo, true);			// call paying function
	}
	
	else if(temp == "GO BACK THREE SPACES.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     GO BACK THREE SPACES.      |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		currentPos -= 3;									// take 3 off the current position
		players[playerNo].setPosition(currentPos);			// set the new position
	}
	
	else if(temp == "ADVANCE TO THE NEAREST STATION. IF UNOWNED, YOU MAY BUY IT FROM THE BANK. IF OWNED, PAY OWNER TWICE THE RENTAL TO WHICH THEY ARE OTHERWISE ENTITLED.") 
	{
		
		//CHANCE - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|            CHANCE              |" << endl;
		cout << "| ADVANCE TO THE NEAREST STATION |" << endl;
		cout << "|IF UNOWNED, YOU MAY BUY IT FROM |" << endl;
		cout << "| THE BANK. IF OWNED, PAY OWNER  |" << endl;
		cout << "| TWICE THE RENTAL TO WHICH THEY |" << endl;
		cout << "|     ARE OTHERWISE ENTITLED     |" << endl;
		cout << "|________________________________|" << endl;
		
		if(currentPos == 7)					// if the player is on this posititon
		{
			players[playerNo].setPosition(15);		// set new position
		}
		
		else if(currentPos == 22)			// if this position
		{
			players[playerNo].setPosition(25);		// set this position
		}
		
		else if(currentPos == 36)					// if this position
		{
			cout << "You passed GO! Collect $200." << endl;		// add $200 to wallet and total worth
			players[playerNo].addToTotalWorth(200);
			players[playerNo].addToWallet(200);
			
			players[playerNo].setPosition(5);		// set this position
		}
		
		currentPos = players[playerNo].getPosition();		// get the current position
		ownerName = board[currentPos].getOwner();			// get the owner of the current property
		if(ownerName != "NULL" && ownerName != players[playerNo].getName() && ownerName != "USER")	// if it is owned
		{
			for (int i = 0; i < playerCount; i++)			// get the owner's player number
			{
				if (players[i].getName() == ownerName)
				{
					owner = i;		// set the owner number
					break;			// break out of for loop
				}
			}
			
			cout << board[currentPos].getName() << " is owned by " << players[owner].getName() << endl;			// output owner
			rentOwed = (board[currentPos].getCurrentRent() * 2);				// double the rent that is usually owed
			
			cout << "You owe " << players[owner].getName() << " $" << rentOwed << endl;	// output rent owed
			playerCanPay(rentOwed, players, playerNo, false);			// pay the rent owed withthis function
			players[owner].addToTotalWorth(rentOwed);					// // add the rent owed to wallet and total worth of the owner
			players[owner].addToWallet(rentOwed);			
		}
		
		else if(ownerName == "USER")						// if it is unowned
		{
			buyerName = players[playerNo].getName();		// get the player name, walelt and property name
			buyerWallet = players[playerNo].getWallet();
			propertyName = board[currentPos].getName();
			purchase(playerNo, buyerName, buyerWallet, players, propertyName, currentPos, playerCount); // call purchase function
		}
	}		
}

void drawCommunityChestCard(player* players, int playerNo, int playerCount, int* properties)
{
	string temp;
	int housesOnProperty;
	int houseCount = 0;
	int hotelCount = 0;
	int totalCost;	
	
	temp = communityChestCards[0];
	for(unsigned int i = 0; i < communityChestCards.size()-1; i++)
	{
		communityChestCards[i] = communityChestCards[i+1];	// moves each card along one
	}	
	communityChestCards[communityChestCards.size()-1] = temp;	// puts card that was drawn at the end of the array (bottom of deck)
	
	if(temp == "BANK ERROR IN YOUR FAVOUR COLLECT $200.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|      BANK ERROR IN YOUR        |" << endl;
		cout << "|     FAVOUR. COLLECT $200       |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;		
		
		players[playerNo].addToTotalWorth(200);		// add to wallet and total worth
		players[playerNo].addToWallet(200);
	}
	
	else if(temp == "ADVANCE TO GO (COLLECT $200).")
	{
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|         ADVANCE TO GO          |" << endl;
		cout << "|         COLLECT $200           |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].setPosition(0);		// set position to go
		players[playerNo].addToTotalWorth(200);	// add to wallet and total worth
		players[playerNo].addToWallet(200);
	}
	else if(temp == "YOU ARE ASSESSED FOR STREET REPAIRS: PAY $40 PER HOUSE AND $115 PER HOTEL YOU OWN.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|  YOU ARE ASSESSED FOR STREET   |" << endl;
		cout << "|  REPAIRS: PAY $40 PER HOUSE    |" << endl;
		cout << "|  AND $115 PER HOTEL YOU OWN.   |" << endl;
		cout << "|________________________________|" << endl;
		
		for (int i = 0; i < 28; i++)		// get the players proprties
		{
			if(properties[i] != 99)			// if the proeprty isnt 99 (sentinel)
			{
				housesOnProperty = board[properties[i]].getHouses(); // get the amount of houses on the property
				if(housesOnProperty == 5)
				{
					hotelCount++;		// if it is five increment the hotel count
				}
				
				else
				{
					houseCount += housesOnProperty; // otherwise add to the house count
				}
			}			
		}
		
		totalCost = (houseCount* 40) + (hotelCount*115); // get the total cost
		playerCanPay(totalCost, players, playerNo, true);	// check if they can pay
	}
	else if(temp == "YOU HAVE WON SECOND PRIZE IN A BEAUTY CONTEST. COLLECT $10.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     YOU HAVE WON SECOND        |" << endl;
		cout << "|   PRIZE IN A BEAUTY CONTEST.   |" << endl;
		cout << "|        COLLECT $10             |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToTotalWorth(10);  // add to wallet and total worth
		players[playerNo].addToWallet(10);		
	}
	else if(temp == "FROM SALE OF STOCK YOU GET $40.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|       FROM SALE OF STOCK       |" << endl;
		cout << "|          YOU GET $50           |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToTotalWorth(40); 	// add to wallet and total worth
		players[playerNo].addToWallet(40);	
	}
	else if(temp == "YOU INHERIT $100.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|       YOU INHERIT $100.        |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToTotalWorth(100);		// add to wallet and total worth
		players[playerNo].addToWallet(100);
	}
	else if(temp == "IT IS YOUR BIRTHDAY. COLLECT $10 FROM EVERY PLAYER.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|      IT IS YOUR BIRTHDAY.      |" << endl;
		cout << "|         COLLECT $10            |" << endl;
		cout << "|      FROM EVERY PLAYER.        |" << endl;
		cout << "|________________________________|" << endl;
		
		for(int i = 0; i < playerCount; i++)
		{
			if (i == playerNo)
			{
				continue;
			}
			players[i].subtractFromTotalWorth(10); // subtract from the other players wallet and total worth
			players[i].subtractFromWallet(10);
			players[playerNo].addToWallet(10);			// add to wallet and total worth
			players[playerNo].addToTotalWorth(10);
		}
	}
	else if(temp == "RECEIVE $25 CONSULTANCY FEE.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|          RECEIVE $25           |" << endl;
		cout << "|        CONSULTANCY FEE.        |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToWallet(25);			// add to wallet and total worth
		players[playerNo].addToTotalWorth(25);
	}
	else if(temp == "GO TO JAIL. GO DIRECTLY TO JAIL, DO NOT PASS GO, DO NOT COLLECT $200.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|    GO TO JAIL. GO DIRECTLY     |" << endl;
		cout << "|    TO JAIL, DO NOT PASS GO,    |" << endl;
		cout << "|      DO NOT COLLECT $200       |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].setInJail(true);			// set the player to be in jail
	}
	else if(temp == "HOSPITAL FEES. PAY $10.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|         HOSPITAL FEES.         |" << endl;
		cout << "|           PAY $100             |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		playerCanPay(100, players, playerNo, true);	// check if they can afford the price
	}
	else if(temp == "INCOME TAX REFUND. COLLECT $20.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|       INCOME TAX REFUNE.       |" << endl;
		cout << "|          COLLECT $20           |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;	
		
		players[playerNo].addToWallet(20);				// add to wallet and total worth
		players[playerNo].addToTotalWorth(20);
	}
	else if(temp == "SCHOOL FEES. PAY $50.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|         SCHOOL FEES.           |" << endl;
		cout << "|           PAY $50              |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		playerCanPay(50, players, playerNo, true);		// check if they can afford the price
	}
	else if(temp == "LIFE INSURANCE MATURES. COLLECT $100.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     LIFE INSURANCE MATURES.    |" << endl;
		cout << "|          COLLECT $100          |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToWallet(100);				// add to wallet and total worth
		players[playerNo].addToTotalWorth(100);		
	}
	else if(temp == "HOLIDAY FUND MATURES. RECEIVE $100.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     HOLIDAY FUND MATURES.      |" << endl;
		cout << "|         RECEIVE $100           |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].addToWallet(100);				// add to wallet and total worth
		players[playerNo].addToTotalWorth(100);			
	}
	else if(temp == "DOCTOR'S FEES. PAY $50.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|        DOCTOR'S FEES.          |" << endl;
		cout << "|          PAY $50.              |" << endl;
		cout << "|                                |" << endl;
		cout << "|________________________________|" << endl;
		
		playerCanPay(50, players, playerNo, true);			// check if they can afford the price
	}
	else if(temp == "GET OF JAIL FREE. This card may be kept until needed or traded.")
	{
		
		//COMMUNITY CHEST - PETER
		cout << " ________________________________ " << endl;
		cout << "|                                |" << endl;
		cout << "|        COMMUNITY CHEST         |" << endl;
		cout << "|                                |" << endl;
		cout << "|                                |" << endl;
		cout << "|     GET OUT OF JAIL FREE.      |" << endl;
		cout << "|     THIS CARD MAY BE KEPT      |" << endl;
		cout << "|     UNTIL NEEDED OR TRADED     |" << endl;
		cout << "|________________________________|" << endl;
		
		players[playerNo].incrementJailCards();		// increment the players jail cards
		players[playerNo].setHasJailCards(true);	// set the player to have jail cards
		//communityChestCards[15] = "NULL";
		communityChestCards.pop_back();				// take one out of the vector
	}
}

void outputTitleDeed(int position) // ASCII output function for title deeds and positions on the board
{
	// all completed by Peter
	system("PAUSE");
	// OLD KENT ROAD - Peter
	string nameOfPosition = board[position].getName();
	
	if(nameOfPosition == "Old Kent Road")
	{
		cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|      Old Kent Road     |" << endl;
	    cout << "|       Price: $60       |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $2    |" << endl;
	    cout << "| With Set:        $4    |" << endl;
	    cout << "| With(1)house     $10   |" << endl;
	    cout << "| With(2)house     $30   |" << endl;
	    cout << "| With(3)house     $90   |" << endl;
	    cout << "| With(4)house     $160  |" << endl;
	    cout << "| With(1)hotel     $250  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $50   |" << endl;
	    cout << "|Hotels cost       $50   |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}	
    
    else if(nameOfPosition == "Whitechapel Road")
	{
	    //WHITCHAPEL ROAD - Peter
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|    Whitechapel Road    |" << endl;
	    cout << "|       Price: $60       |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $4    |" << endl;
	    cout << "| With Set:        $8    |" << endl;
	    cout << "| With(1)house     $20   |" << endl;
	    cout << "| With(2)house     $60   |" << endl;
	    cout << "| With(3)house     $180  |" << endl;
	    cout << "| With(4)house     $320  |" << endl;
	    cout << "| With(1)hotel     $450  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $50   |" << endl;
	    cout << "|Hotels cost       $50   |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "The Angel Islington")
	{
	    //THE ANGEL, ISLINGTION - Peter
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|  The Angel Islingtion  |" << endl;
	    cout << "|       Price: $100      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $6    |" << endl;
	    cout << "| With Set:        $12   |" << endl;
	    cout << "| With(1)house     $30   |" << endl;
	    cout << "| With(2)house     $90   |" << endl;
	    cout << "| With(3)house     $270  |" << endl;
	    cout << "| With(4)house     $400  |" << endl;
	    cout << "| With(1)hotel     $550  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $50   |" << endl;
	    cout << "|Hotels cost       $50   |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Euston Road")
	{    
	    //EUSTON ROAD - Peter
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|      Euston Road       |" << endl;
	    cout << "|       Price: $100      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $6    |" << endl;
	    cout << "| With Set:        $12   |" << endl;
	    cout << "| With(1)house     $30   |" << endl;
	    cout << "| With(2)house     $90   |" << endl;
	    cout << "| With(3)house     $270  |" << endl;
	    cout << "| With(4)house     $400  |" << endl;
	    cout << "| With(1)hotel     $550  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $50   |" << endl;
	    cout << "|Hotels cost       $50   |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	    //PENTONVILLE ROAD - Peter
	else if(nameOfPosition == "Pentonville Road")
	{    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|    Pentonville Road    |" << endl;
	    cout << "|       Price: $120      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $8    |" << endl;
	    cout << "| With Set:        $16   |" << endl;
	    cout << "| With(1)house     $40   |" << endl;
	    cout << "| With(2)house     $100  |" << endl;
	    cout << "| With(3)house     $300  |" << endl;
	    cout << "| With(4)house     $450  |" << endl;
	    cout << "| With(1)hotel     $600  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $50   |" << endl;
	    cout << "|Hotels cost       $50   |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Pall Mall")
	{    
	    //PALL MALL - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|        Pall Mall       |" << endl;
	    cout << "|       Price: $140      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $10   |" << endl;
	    cout << "| With Set:        $20   |" << endl;
	    cout << "| With(1)house     $50   |" << endl;
	    cout << "| With(2)house     $150  |" << endl;
	    cout << "| With(3)house     $450  |" << endl;
	    cout << "| With(4)house     $625  |" << endl;
	    cout << "| With(1)hotel     $750  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $100  |" << endl;
	    cout << "|Hotels cost       $100  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Whitehall")
	{    
	    //WHITEHALL - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|        Whitehall       |" << endl;
	    cout << "|       Price: $140      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $10   |" << endl;
	    cout << "| With Set:        $20   |" << endl;
	    cout << "| With(1)house     $50   |" << endl;
	    cout << "| With(2)house     $150  |" << endl;
	    cout << "| With(3)house     $450  |" << endl;
	    cout << "| With(4)house     $625  |" << endl;
	    cout << "| With(1)hotel     $750  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $100  |" << endl;
	    cout << "|Hotels cost       $100  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Northumberland Avenue")
	{    
	    //NORTHUMBERLAND AVENUE - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|  Northumberland Avenue |" << endl;
	    cout << "|       Price: $160      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $12   |" << endl;
	    cout << "| With Set:        $24   |" << endl;
	    cout << "| With(1)house     $60   |" << endl;
	    cout << "| With(2)house     $180  |" << endl;
	    cout << "| With(3)house     $500  |" << endl;
	    cout << "| With(4)house     $700  |" << endl;
	    cout << "| With(1)hotel     $900  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $100  |" << endl;
	    cout << "|Hotels cost       $100  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Bow Street")
	{    
	    //BOW STREET - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|       Bow Street       |" << endl;
	    cout << "|       Price: $180      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $14   |" << endl;
	    cout << "| With Set:        $28   |" << endl;
	    cout << "| With(1)house     $70   |" << endl;
	    cout << "| With(2)house     $200  |" << endl;
	    cout << "| With(3)house     $550  |" << endl;
	    cout << "| With(4)house     $750  |" << endl;
	    cout << "| With(1)hotel     $950  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $100  |" << endl;
	    cout << "|Hotels cost       $100  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Marlborough Street")
	{    
	    //MARLBOROUGH STREET - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|   Marlborough Street   |" << endl;
	    cout << "|       Price: $180      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $14   |" << endl;
	    cout << "| With Set:        $28   |" << endl;
	    cout << "| With(1)house     $70   |" << endl;
	    cout << "| With(2)house     $200  |" << endl;
	    cout << "| With(3)house     $550  |" << endl;
	    cout << "| With(4)house     $750  |" << endl;
	    cout << "| With(1)hotel     $950  |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $100  |" << endl;
	    cout << "|Hotels cost       $100  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Vine Street")
	{    
	    //VINE STREET - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|       Vine Street      |" << endl;
	    cout << "|       Price: $200      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $16   |" << endl;
	    cout << "| With Set:        $32   |" << endl;
	    cout << "| With(1)house     $80   |" << endl;
	    cout << "| With(2)house     $220  |" << endl;
	    cout << "| With(3)house     $600  |" << endl;
	    cout << "| With(4)house     $800  |" << endl;
	    cout << "| With(1)hotel     $1000 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $100  |" << endl;
	    cout << "|Hotels cost       $100  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Strand")
	{    
	    //STRAND - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|         Strand         |" << endl;
	    cout << "|       Price: $220      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $18   |" << endl;
	    cout << "| With Set:        $36   |" << endl;
	    cout << "| With(1)house     $90   |" << endl;
	    cout << "| With(2)house     $250  |" << endl;
	    cout << "| With(3)house     $700  |" << endl;
	    cout << "| With(4)house     $875  |" << endl;
	    cout << "| With(1)hotel     $1050 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $150  |" << endl;
	    cout << "|Hotels cost       $150  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Fleet Street")
	{    
	    //FLEET STREET - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|       Fleet Street     |" << endl;
	    cout << "|       Price: $220      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $18   |" << endl;
	    cout << "| With Set:        $36   |" << endl;
	    cout << "| With(1)house     $90   |" << endl;
	    cout << "| With(2)house     $250  |" << endl;
	    cout << "| With(3)house     $700  |" << endl;
	    cout << "| With(4)house     $875  |" << endl;
	    cout << "| With(1)hotel     $1050 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $150  |" << endl;
	    cout << "|Hotels cost       $150  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Trafalgar Square")
	{    
	    //TRAFALGAR SQUARE - Peter
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|    Trafalgar Square    |" << endl;
	    cout << "|       Price: $240      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $20   |" << endl;
	    cout << "| With Set:        $40   |" << endl;
	    cout << "| With(1)house     $100  |" << endl;
	    cout << "| With(2)house     $300  |" << endl;
	    cout << "| With(3)house     $750  |" << endl;
	    cout << "| With(4)house     $925  |" << endl;
	    cout << "| With(1)hotel     $1100 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $150  |" << endl;
	    cout << "|Hotels cost       $150  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Leicester Square")
	{    
	    //LEICESTER SQUARE
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|    Leicester Square    |" << endl;
	    cout << "|       Price: $260      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $22   |" << endl;
	    cout << "| With Set:        $44   |" << endl;
	    cout << "| With(1)house     $110  |" << endl;
	    cout << "| With(2)house     $330  |" << endl;
	    cout << "| With(3)house     $800  |" << endl;
	    cout << "| With(4)house     $975  |" << endl;
	    cout << "| With(1)hotel     $1150 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $150  |" << endl;
	    cout << "|Hotels cost       $150  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Coventry Street")
	{    
	     //COVENTRY STREET
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|    Coventry Street     |" << endl;
	    cout << "|       Price: $260      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $22   |" << endl;
	    cout << "| With Set:        $44   |" << endl;
	    cout << "| With(1)house     $110  |" << endl;
	    cout << "| With(2)house     $330  |" << endl;
	    cout << "| With(3)house     $800  |" << endl;
	    cout << "| With(4)house     $975  |" << endl;
	    cout << "| With(1)hotel     $1150 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $150  |" << endl;
	    cout << "|Hotels cost       $150  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Piccadilly")
	{    
	     //PICCADILLY SQUARE
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|       Piccadilly       |" << endl;
	    cout << "|       Price: $280      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $24   |" << endl;
	    cout << "| With Set:        $48   |" << endl;
	    cout << "| With(1)house     $120  |" << endl;
	    cout << "| With(2)house     $360  |" << endl;
	    cout << "| With(3)house     $850  |" << endl;
	    cout << "| With(4)house     $1025 |" << endl;
	    cout << "| With(1)hotel     $1200 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $150  |" << endl;
	    cout << "|Hotels cost       $150  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Regent Street")
	{    
	     //REGENT STREET
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|      Regent Street     |" << endl;
	    cout << "|       Price: $300      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $26   |" << endl;
	    cout << "| With Set:        $52   |" << endl;
	    cout << "| With(1)house     $130  |" << endl;
	    cout << "| With(2)house     $390  |" << endl;
	    cout << "| With(3)house     $900  |" << endl;
	    cout << "| With(4)house     $1100 |" << endl;
	    cout << "| With(1)hotel     $1275 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $200  |" << endl;
	    cout << "|Hotels cost       $200  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Oxford Street")
	{    
	    
	    //REGENT STREET
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|      Oxford Street     |" << endl;
	    cout << "|       Price: $300      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $26   |" << endl;
	    cout << "| With Set:        $52   |" << endl;
	    cout << "| With(1)house     $130  |" << endl;
	    cout << "| With(2)house     $390  |" << endl;
	    cout << "| With(3)house     $900  |" << endl;
	    cout << "| With(4)house     $1100 |" << endl;
	    cout << "| With(1)hotel     $1275 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $200  |" << endl;
	    cout << "|Hotels cost       $200  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	else if(nameOfPosition == "Bond Street")
	{    
	    
	    //BOND STREET
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|       Bond Street      |" << endl;
	    cout << "|       Price: $320      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $28   |" << endl;
	    cout << "| With Set:        $56   |" << endl;
	    cout << "| With(1)house     $150  |" << endl;
	    cout << "| With(2)house     $450  |" << endl;
	    cout << "| With(3)house     $1000 |" << endl;
	    cout << "| With(4)house     $1200 |" << endl;
	    cout << "| With(1)hotel     $1400 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $200  |" << endl;
	    cout << "|Hotels cost       $200  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	
	else if(nameOfPosition == "Park Lane")
	{    
	    //PARK LANE
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|        Park Lane       |" << endl;
	    cout << "|       Price: $350      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $35   |" << endl;
	    cout << "| With Set:        $70   |" << endl;
	    cout << "| With(1)house     $175  |" << endl;
	    cout << "| With(2)house     $500  |" << endl;
	    cout << "| With(3)house     $1100 |" << endl;
	    cout << "| With(2)house     $1300 |" << endl;
	    cout << "| With(1)hotel     $1500 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $200  |" << endl;
	    cout << "|Hotels cost       $200  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	
	else if(nameOfPosition == "Mayfair")
	{    
	    
	    //MAYFAIR
	    
	    cout << " ________________________" << endl;
	    cout << "|       Title Deed       |" << endl;
	    cout << "|                        |" << endl;
	    cout << "|         Mayfair        |" << endl;
	    cout << "|       Price: $400      |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "| Rent:            $50   |" << endl;
	    cout << "| With Set:        $100  |" << endl;
	    cout << "| With(1)house     $200  |" << endl;
	    cout << "| With(2)house     $600  |" << endl;
	    cout << "| With(3)house     $1400 |" << endl;
	    cout << "| With(2)house     $1700 |" << endl;
	    cout << "| With(1)hotel     $2000 |" << endl;
	    cout << "|________________________|" << endl;
	    cout << "|Houses cost       $200  |" << endl;
	    cout << "|Hotels cost       $200  |" << endl;
	    cout << "|(plus 4 houses)         |" << endl;
	    cout << "|________________________|" << endl;
	}
	
	else if(nameOfPosition == "GO")
	{    
	    //GO sign - Peter
	    
	    cout << " _________________________  "   << endl;
	    cout << "|                         |"    << endl;
	    cout << "|       COLLECT $200      |"    << endl;
	    cout << "|       AS YOU PASS       |"    << endl;
	    cout << "|       _____  ____       |"    << endl;
	    cout << "|      / ____|/ __ \\      |"   << endl;
	    cout << "|     | |  __| |  | |     |"    << endl;
	    cout << "|     | | |_ | |  | |     |"    << endl;
	    cout << "|     | |__| | |__| |     |"    << endl;
	    cout << "|      \\_____|\\____/      |"  << endl;
	    cout << "|                         |"    << endl;
	    cout << "| /|________________////  |"    << endl;
	    cout << "| \\|                \\\\\\\\  |"    << endl;
	    cout << "|_________________________|"    << endl;
	}
	
	
	else if(nameOfPosition == "Community Chest")
	{    
	    
	    // COMMUNITY CHEST - Peter
	    
	    
	    cout << " ________________________  "   << endl;
	    cout << "|                        |"    << endl;
	    cout << "|    COMMUNITY CHEST     |"    << endl;
	    cout << "|                        |"    << endl;
	    cout << "|       __________       |" << endl;
	    cout << "|      /\\____;;___\\      |" << endl;
	    cout << "|     | /         /      |" << endl;
	    cout << "|     |__________\\       |" << endl;
	    cout << "|     |\\          \\      |" << endl;
	    cout << "|     | |---------|      |" << endl;
	    cout << "|     \\ |    ))   |      |" << endl;
	    cout << "|      \\| ________|      |"    << endl;
	    cout << "|                        |"    << endl;
	    cout << "|________________________|"    << endl;
	}
	else if(nameOfPosition == "Chance")
	{    
	    
	     // CHANCE - Peter
	    cout << " ________________________"    << endl;
	    cout << "|                        |"    << endl;
	    cout << "|         CHANCE         |"    << endl;
	    cout << "|         _____          |"    << endl;
	    cout << "|        //---\\\\         |"    << endl;
	    cout << "|       //     ||        |"    << endl;
	    cout << "|             ||         |"    << endl;
	    cout << "|            //          |"    << endl;
	    cout << "|           //           |"    << endl;
	    cout << "|          ||            |"    << endl; 
	    cout << "|                        |"    << endl;
	    cout << "|          ##            |"    << endl;
	    cout << "|________________________|"    << endl;
	}
	else if (nameOfPosition == "Electric Company")
    {
        // ELECTRIC COMPANY - Peter
        cout << " ________________________"     << endl;
        cout << "|                        |"    << endl;
        cout << "|         $150           |"    << endl;
        cout << "|                        |"    << endl;
        cout << "|      .    |    ,       | "   << endl;
        cout << "|       \\ _---_ /        |"    << endl;
        cout << "|    -_ .'     `. _- '   |"     << endl;
        cout << "|   __ :  .---.  : __    |"    << endl;
        cout << "|      :  \\   /  :       |"    << endl;
        cout << "|    -' `. | | .' '-     |"    << endl;
        cout << "|       / |`-'| \\        |"    << endl;
        cout << "|      '  ]'-_[  `       |"    << endl;
        cout << "|         ]-'_[          |"    << endl;
        cout << "|                        |"    << endl;
        cout << "|    ELECTRIC COMPANY    |"    << endl;
        cout << "|                        |"    << endl;
        cout << "|IF ONE UTILITY IS OWNED |"    << endl;
        cout << "| RENT IS 4 TIMES AMOUNT |"    << endl;
        cout << "|    SHOWN ON DICE       |"    << endl;
        cout << "|                        |"    << endl;
        cout << "| IF BOTH UTILITIES ARE  |"    << endl;
        cout << "| OWNED RENT IS 10 TIMES |"    << endl; 
        cout << "|  AMOUNT SHOWN ON DICE  |"    << endl;
        cout << "|________________________|"    << endl;   		
	}
	
	else if (nameOfPosition == "Water Works")
    {
         // WATER WORKS - Peter
        cout << " ________________________"     << endl;
        cout << "|                        |"    << endl;
        cout << "|         $150           |"    << endl;
        cout << "|         _____          |"    << endl;
        cout << "|        /#___#\\         | "   << endl;
        cout << "|    /|    | |           |"    << endl;
        cout << "|   / |----| |-----\\     |"     << endl;
        cout << "|   \\ |----| |---\\ |     |"    << endl;
        cout << "|    \\|          | |     |"    << endl;
        cout << "|                | |     |"    << endl;
        cout << "|                |_|     |"    << endl;
        cout << "|                        |"    << endl;
        cout << "|       WATER WORKS      |"    << endl;
        cout << "|                        |"    << endl;
        cout << "|IF ONE UTILITY IS OWNED |"    << endl;
        cout << "| RENT IS 4 TIMES AMOUNT |"    << endl;
        cout << "|    SHOWN ON DICE       |"    << endl;
        cout << "|                        |"    << endl;
        cout << "| IF BOTH UTILITIES ARE  |"    << endl;
        cout << "| OWNED RENT IS 10 TIMES |"    << endl; 
        cout << "|  AMOUNT SHOWN ON DICE  |"    << endl;
        cout << "|________________________|"    << endl;
	}
	
	else if (nameOfPosition == "Free Parking")
    {
         // FREE PARKING
      cout << " __________________________"    << endl;
      cout << "|                          |"    << endl;
      cout << "|                          |"    << endl;
      cout << "|           FREE           |" << endl;
      cout << "|        ___________       |" << endl;
      cout << "|       //   |||   \\\\      |" << endl;   
      cout << "|   __ //____|||____\\\\__   |" << endl; 
      cout << "|  | _|      |       _  |  |" << endl;
      cout << "|  |/ \\______|______/ \\_|  |" << endl;
      cout << "|   \\_/             \\_/    |" << endl;
      cout << "|                          |" << endl;
      cout << "|         PARKING          |"    << endl;
      cout << "|                          |"    << endl;
      cout << "|__________________________|"    << endl;
	}
	
	else if (nameOfPosition == "Go To Jail")
    {
         // GO TO JAIL - Peter
      cout << " __________________________"    << endl;
      cout << "|                          |"    << endl;
      cout << "|          GO TO           |" << endl;
      cout << "|                          |" << endl;
      cout << "|        ,   /\\   ,        |" << endl;
      cout << "|       / '-'  '-' \\       |" << endl;
      cout << "|      |   POLICE   |      |" << endl;
      cout << "|      \\    .--.    /      |" << endl;
      cout << "|       |  ( 10 )  |       |" << endl;
      cout << "|       \\   '--'   /       |" << endl;
      cout << "|        '--.  .--'        |" << endl;
      cout << "|            \\/            |" << endl;
      cout << "|                          |" << endl;
      cout << "|           JAIL           |"    << endl;
      cout << "|__________________________|"    << endl;
	}
	
	else if (nameOfPosition == "Just Visiting")
    {
         // CHANCE - Peter
      cout << " _________________________"    << endl;
      cout << "|                         |"    << endl;
      cout << "|                         |"    << endl;
      cout << "|   ||   ||, , ,||   ||   |"    << endl;
      cout << "|   ||  (||/|/(\\||/  ||   |"    << endl;
      cout << "|   ||  ||| _'_`|||  ||   |"    << endl;
      cout << "|   ||   || o o ||   ||   |"    << endl;
      cout << "|   ||  (||  - `||)  ||   |"    << endl;
      cout << "|   ||   ||  =  ||   ||   |"    << endl;
      cout << "|   ||   ||\\___/||   ||   |"    << endl;
      cout << "|   ||___||) , (||___||   |"    << endl;
      cout << "|  /||---||-\\_/-||---||\\  |"    << endl;
      cout << "| / ||--_||_____||_--|| \\ |"    << endl;
      cout << "|      JUST VISITING      |"    << endl;
      cout << "|_________________________|"    << endl;
	}
	
	else if (nameOfPosition == "Kings Cross Station")
    {
         // KINGS CROSS - Peter
		cout << " ________________________" << endl;
		cout << "|                        |" << endl;
		cout << "|  Kings Cross Station   |" << endl;
		cout << "|       Price: $200      |" << endl;
		cout << "|   _________            |" << endl;
		cout << "|    |  _  |    __       |" << endl;
		cout << "|    | | | |____\\/_      |" << endl;
		cout << "|    | |_| |       \\     |" << endl;
		cout << "|    | __  |  _  _  |    |" << endl;
		cout << "|    |/  \\_|_/ \\/ \\/     |" << endl;
		cout << "|     \\__/   \\_/\\_/      |" << endl;
		cout << "|                        |" << endl;                    
		cout << "|                        |" << endl;
		cout << "| Rent:             $25  |" << endl;
		cout << "| With(2)stations   $50  |" << endl;
		cout << "| With(3)stations   $100 |" << endl;
		cout << "| With(4)stations   $200 |" << endl;
		cout << "|________________________|" << endl;
  	}
	  
	else if (nameOfPosition == "Marylebone Station")
    {
         // MARYLEBONE STATION - Peter
		cout << " ________________________" << endl;
		cout << "|                        |" << endl;
		cout << "|   Marylebone Station   |" << endl;
		cout << "|       Price: $200      |" << endl;
		cout << "|   _________            |" << endl;
		cout << "|    |  _  |    __       |" << endl;
		cout << "|    | | | |____\\/_      |" << endl;
		cout << "|    | |_| |       \\     |" << endl;
		cout << "|    | __  |  _  _  |    |" << endl;
		cout << "|    |/  \\_|_/ \\/ \\/     |" << endl;
		cout << "|     \\__/   \\_/\\_/      |" << endl;
		cout << "|                        |" << endl;                    
		cout << "|                        |" << endl;
		cout << "| Rent:             $25  |" << endl;
		cout << "| With(2)stations   $50  |" << endl;
		cout << "| With(3)stations   $100 |" << endl;
		cout << "| With(4)stations   $200 |" << endl;
		cout << "|________________________|" << endl;
	}
	
	else if (nameOfPosition == "Frenchurch Street Station")
    {
         // LIVERPOOL STATION - Peter
		cout << " ________________________" << endl;
		cout << "|                        |" << endl;
		cout << "| Frenchurch St. Station |" << endl;
		cout << "|       Price: $200      |" << endl;
		cout << "|   _________            |" << endl;
		cout << "|    |  _  |    __       |" << endl;
		cout << "|    | | | |____\\/_      |" << endl;
		cout << "|    | |_| |       \\     |" << endl;
		cout << "|    | __  |  _  _  |    |" << endl;
		cout << "|    |/  \\_|_/ \\/ \\/     |" << endl;
		cout << "|     \\__/   \\_/\\_/      |" << endl;
		cout << "|                        |" << endl;                    
		cout << "|                        |" << endl;
		cout << "| Rent:             $25  |" << endl;
		cout << "| With(2)stations   $50  |" << endl;
		cout << "| With(3)stations   $100 |" << endl;
		cout << "| With(4)stations   $200 |" << endl;
		cout << "|________________________|" << endl;
	}
	
	else if (nameOfPosition == "Liverpool Street Station")
    {
         // FENCHURCH STATION - Peter
		cout << " ________________________" << endl;
		cout << "|                        |" << endl;
		cout << "| Liverpool St. Station  |" << endl;
		cout << "|       Price: $200      |" << endl;
		cout << "|   _________            |" << endl;
		cout << "|    |  _  |    __       |" << endl;
		cout << "|    | | | |____\\/_      |" << endl;
		cout << "|    | |_| |       \\     |" << endl;
		cout << "|    | __  |  _  _  |    |" << endl;
		cout << "|    |/  \\_|_/ \\/ \\/     |" << endl;
		cout << "|     \\__/   \\_/\\_/      |" << endl;
		cout << "|                        |" << endl;                    
		cout << "|                        |" << endl;
		cout << "| Rent:             $25  |" << endl;
		cout << "| With(2)stations   $50  |" << endl;
		cout << "| With(3)stations   $100 |" << endl;
		cout << "| With(4)stations   $200 |" << endl;
		cout << "|________________________|" << endl;
	}
	
	else if (nameOfPosition == "Income Tax")
    {
         // INCOME TAX - Peter
		cout << " ________________________"    << endl;
		cout << "|                        |"    << endl;
		cout << "|       INCOME TAX       |"    << endl;
		cout << "|                        |"    << endl;
		cout << "|                        |"    << endl;
		cout << "|                        |"    << endl;
		cout << "|          /\\            |"    << endl;
		cout << "|          \\/            |"    << endl;
		cout << "|                        |"    << endl;
		cout << "|        PAY $200        |"    << endl; 
		cout << "|           OR           |"    << endl;
		cout << "|   10% OF TOTAL WORTH   |"    << endl;
		cout << "|________________________|"    << endl;	
	}
	
	else if (nameOfPosition == "Super Tax")
    {
         // SUPER TAX - Peter
		cout << " ________________________"    << endl;
		cout << "|                        |"    << endl;
		cout << "|       SUPER TAX        |"    << endl;
		cout << "|                        |"    << endl;
		cout << "|         ____           |"    << endl;
		cout << "|         \\  /           |"    << endl;
		cout << "|        __\\/__          |"    << endl;
		cout << "|       //----\\\\         |"    << endl;
		cout << "|      ||      ||        |"    << endl;
		cout << "|      ||      ||        |"    << endl; 
		cout << "|       \\\\____//         |"    << endl;
		cout << "|         ----           |"    << endl;
		cout << "|                        |"    << endl;
		cout << "|        PAY $100        |"    << endl;
		cout << "|________________________|"    << endl;
	}	
}

void gameOver(int winner, player* players)      // function to end the game
{
	string winnerName = players[winner].getName();          // get the winner's name
	int winnerTotalWorth = players[winner].getTotalWorth(); // get the winner's total worth
	int winnerWallet = players[winner].getWallet();         // get the winner's wallet
	
	cout << "#####################################################" << endl;
 	cout << "#    __  __           HASBRO(c)            _        #" << endl;
    cout << "#   |  \\/  |                             | |        #" << endl;
    cout << "#   | \\  / | ___  _ __   ___  _ __   ___ | |_   _   #" << endl;
    cout << "#   | |\\/| |/ _ \\| '_ \\ / _ \\| '_ \\ / _ \\| | | | |  #" << endl;
    cout << "#   | |  | | (_) | | | | (_) | |_) | (_) | | |_| |  #" << endl;
    cout << "#   |_|  |_|\\___/|_| |_|\\___/| .__/ \\___/|_|\\__, |  #" << endl;
    cout << "#                            | |             __/ |  #" << endl;
    cout << "#                            |_|            |___/   #" << endl;
    cout << "#   2 - 6 Players                                   #" << endl;
    cout << "#####################################################" << endl;
    cout << "#                                                   #" << endl;
    cout << "#                     Game Over                     #" << endl;	
    cout << "#                                                   #" << endl;
    cout << "#####################################################" << endl;
    cout << "WINNER: " << winnerName << endl;                          // output the winners name, total worth and wallet
    cout << "WALLET: $" << winnerWallet << endl;
    cout << "TOTAL WORTH: $" << winnerTotalWorth << endl;
    system("PAUSE");
    exit(0);
}

void trade(player* players, int playerNo, int playerCount) // function to trade
{
	string userDec;         // string to hold userDecision
	int userDecision;		// int for user decision
	bool isValid;			// boolean to check if it is valid input
	string yourName	 = players[playerNo].getName();	// gets the players name
	int jailCards;		// int to hold the get out of jail free cards 
	bool notInVector;	// bool to check 
	string colour;		// string to hold colour of properties
	int otherPlayer;	// integer for the player you are trading with
	int size;			// integer to store size of vectors
	char accept;		// character for a user decision
	string temp;		// temporary string
	int temp2;			// temproary integer
	char moneyDecision;	// character for decision on whther money is being traded
	int yourMoneyOffer = 0;	// intger to store your money offer
	int theirMoneyOffer = 0; // integer to store their money offer
	string moneyOffer;
	
	vector<int> yourAssetsTradable;	// vectors for assets trading and tradable
	vector<int> yourAssetsTrading;
	vector<int> theirAssetsTradable;
	vector<int> theirAssetsTrading;
	system("cls");
	cout << "#####################################################" << endl;
 	cout << "#                                                   #" << endl;
    cout << "#                       Trade                       #" << endl;	
    cout << "#             Enter 0 to finish offering            #" << endl;
	cout << "#####################################################" << endl;
	cout << "PRESS:" << endl;
	do	// post repeat loop for user decision of player to be traded with
	{
		for(int i = 0; i < playerCount; i++)		// output players and their names
		{
			
		    if(i == playerNo)
		 	{
				continue;
	 		}
	 		else
	 		{
	 			cout << i+1 << " = " << players[i].getName() << endl;
	 		}	 	
		}
		cin >> userDecision;
		userDecision--;
	}while(userDecision == playerNo || userDecision >= playerCount);
	
	system("PAUSE");
	otherPlayer = userDecision; // sets the player that is being traded with to be the user input player
	string theirName = players[otherPlayer].getName(); // gets the other players name
	cout << "NOW TRADING WITH: " << theirName << endl;	// output who is being traded with
	
	do		// do while that is done while the other player counter offers
	{
		do	// do while user input is not a sentinel to indicate they are done
		{
			system("PAUSE");
			system("cls");
			cout << "#####################################################" << endl;
		 	cout << "#                                                   #" << endl;
		    cout << "#                       Trade                       #" << endl;	
    		cout << "#             Enter 0 to finish offering            #" << endl;
			cout << "#####################################################" << endl;
			theirAssetsTradable.clear(); // clear the tradable assets for both players
			yourAssetsTradable.clear();
						
			cout << theirName << "'s ASSETS: " << endl;
			for(int i = 0; i < 40; i++) // for loop for each position on board
			{
				colour = board[i].getColour(); // get the colour of the current property
				if(theirAssetsTrading.size() == 0) // if the size of their assets trading is 0
				{
					notInVector = true; 	// set not in vector to be true
				}
				else
				{
					for(unsigned int p = 0; p < theirAssetsTrading.size(); p++) // for loop to check if the property is already in the vector
					{
						if(theirAssetsTrading[p] == i)
						{
							notInVector = false;	// set not in vector to be false
							break;					// break out of loop
						}
						notInVector = true;			// set not in vector to be true
					}
				}
				if(board[i].getOwner() == theirName && board[i].hasHousesOnSet(colour, board) == false && notInVector == true) // if they are the owner, and it has no houses on it and it isnt alrady in the vector
				{
					theirAssetsTradable.push_back(i); // add the position to the vector
				}		
			}	 
			if(players[otherPlayer].hasJailFreeCards())	// if the other player has get out jail free cards
			{
				jailCards = players[otherPlayer].getJailCards();			// get the jail cards they have
				for(int i = 0; i < jailCards; i++)
				{
					theirAssetsTradable.push_back(40);			// add 40 to the end of the vector 
				}
			}
			for(unsigned int i = 0; i < theirAssetsTradable.size(); i++)	// output their assets tradable
			{
				if(theirAssetsTradable[i] < 40)
				{
					cout << i + 1 << " = " << board[theirAssetsTradable[i]].getName() << endl;
				}
				
				else if(theirAssetsTradable[i] == 40)
				{
					cout << i + 1 << " = Get Out Jail Free Card" << endl;
				}			
			}
			
			if(yourAssetsTrading.size() != 0)	// if the size of the assets you wish to trade is not 0
			{
				cout << "TRADING: " << endl;		// output what you are trading
				
				for(unsigned int i = 0; i < yourAssetsTrading.size(); i++)
				{
					if(yourAssetsTrading[i] < 40)
					{
						cout << "  >>" << board[yourAssetsTrading[i]].getName() << endl;
					}
					
					else if(yourAssetsTrading[i] == 40)
					{
						cout << "  >> Get Out Jail Free Card" << endl;
					} 
				}
			}
			
			cout << "WHAT WOULD YOU LIKE TO OFFER: " << endl;			// ask what they are offering
			for(int i = 0; i < 40; i++)
			{
				//system("PAUSE");
				colour = board[i].getColour();		// get the colour of the property
				//cout << "COLOUR  = " << colour << endl;
				if(yourAssetsTrading.size() == 0)	// if your assets trading is 0
				{
					notInVector = true;				// set not in vector to be true
				}
				else								//otherwise 
				{
					for(unsigned int p = 0; p < yourAssetsTrading.size(); p++)	// check if the assets are already being offered
					{
						if(yourAssetsTrading[p] == i)							
						{
							notInVector = false;		// if it is in the vector set it be false
							break;						// break out of loop
						}
						notInVector = true;
					}
				}
				if(board[i].getOwner() == yourName && board[i].hasHousesOnSet(colour, board) == false && notInVector == true)	// if you are the owner and their arent houses on the property and it isnt already being traded
				{
					yourAssetsTradable.push_back(i);	// add the position to the end of the vector
				}		
			}	 
			if(players[playerNo].hasJailFreeCards())	// if you have get out of jail free cards
			{
				jailCards = players[playerNo].getJailCards();			// get the amount of jail cards
				for(int i = 0; i < jailCards; i++)
				{
					yourAssetsTradable.push_back(40);		// append the vector with 40 to indicate get out jail free cards
				}
			}
			for(unsigned int i = 0; i < yourAssetsTradable.size(); i++)			// list the assets you can trade
			{
				if(yourAssetsTradable[i] < 40)
				{
					cout << i + 1 << " = " << board[yourAssetsTradable[i]].getName() << endl;
				}
				
				else if(yourAssetsTradable[i] == 40)
				{
					cout << i + 1 << " = Get Out Jail Free Card" << endl;
				}			
			}
			
			do		// input which asset you would like to trade
			{
				cin >> userDec;
				isValid = true;
				for(unsigned int l = 0; l < userDec.size(); l++)		// determine if the input was a number
				{
					if(userDec[l] != '0' && userDec[l] != '1' && userDec[l] != '2' && userDec[l] != '3' && userDec[l] != '4' && userDec[l] != '5' && 
					   userDec[l] != '6' && userDec[l] != '7' && userDec[l] != '8' && userDec[l] != '9')
					{
						cout << "ERROR: Input must be a number." << endl;
						isValid = false;
						break;
					}   
				}
			}while(isValid == false);
			
			userDecision = atoi(userDec.c_str());		// convert the character to a number
			
			size = yourAssetsTradable.size();			// get the size of the assets you can trade
			if(userDecision != 0 && userDecision <= size)		// if the input is not 0 or greater than the size of the vector
			{
				yourAssetsTrading.push_back(yourAssetsTradable[userDecision-1]);	// add the asset to the vector containing items that are going to be traded
			}
			
		}while(userDecision != 0);	
		
		do 
		{
			cout << "Would you like to offer any money? (Y/N)" << endl;	// ask for input determining whether or not they wish to offer money
			cin >> moneyDecision;
			
			if(moneyDecision == 'Y')			// if they do wish to trade money
			{
				do			// ask for input until their money offer is less than/equal to their wallet
				{
					do
					{
						cout << "You have $" << players[playerNo].getWallet() << endl;
						cout << "How much are you offering?" << endl;
						cin >> moneyOffer;
						isValid = true;
						for(unsigned int p = 0; p < moneyOffer.size(); p++)
						{
							if(moneyOffer[p] != '0' && moneyOffer[p] != '1' && moneyOffer[p] != '2' && moneyOffer[p] != '3' && moneyOffer[p] != '4' && moneyOffer[p] != '5' && 
							   moneyOffer[p] != '6' && moneyOffer[p] != '7' && moneyOffer[p] != '8' && moneyOffer[p] != '9')
							{
								cout << "ERROR: Input must be a number." << endl;
								isValid = false;
								break;
							}   
						}
					}while(isValid == false);
					
					yourMoneyOffer = atoi(moneyOffer.c_str());
				}while(yourMoneyOffer > players[playerNo].getWallet());				
			}
			else if(moneyDecision == 'N')
			{
				yourMoneyOffer = 0;
			}
		}while(moneyDecision != 'Y' && moneyDecision != 'N');
		
		do
		{
			// the same as the above, however this time it goes through what you would like to receive
			system("PAUSE");
			system("cls");
			cout << "#####################################################" << endl;
		 	cout << "#                                                   #" << endl;
		    cout << "#                       Trade                       #" << endl;	
    		cout << "#             Enter 0 to finish offering            #" << endl;
			cout << "#####################################################" << endl;
			cout << "What would you like to receive from "<< theirName << "'s ASSETS: " << endl;
			theirAssetsTradable.clear();
			for(int i = 0; i < 40; i++)
			{
				colour = board[i].getColour();
				if(theirAssetsTrading.size() == 0)
				{
					notInVector = true;
				}
				else
				{
					for(unsigned int p = 0; p < theirAssetsTrading.size(); p++)	// checks if it is alerady being traded
					{
						if(theirAssetsTrading[p] == i)
						{
							notInVector = false;
							break;
						}
						notInVector = true;
					}
				}
				if(board[i].getOwner() == theirName && board[i].hasHousesOnSet(colour, board) == false && notInVector == true)
				{
		
					theirAssetsTradable.push_back(i);
				}		
			}	 
			if(players[otherPlayer].hasJailFreeCards())
			{
				jailCards = players[otherPlayer].getJailCards();			
				for(int i = 0; i < jailCards; i++)
				{
					theirAssetsTradable.push_back(40);
				}
			}
			
			for(unsigned int i = 0; i < theirAssetsTradable.size(); i++)		// list assets
			{
				if(theirAssetsTradable[i] < 40)
				{
					cout << i + 1 << " = " << board[theirAssetsTradable[i]].getName() << endl;
				}
				
				else if(theirAssetsTradable[i] == 40)
				{
					cout << i + 1 << " = Get Out Jail Free Card" << endl;
				}			
			}	 
						
			cout << "TRADING YOUR: " << endl;
			
			if(yourAssetsTrading.size() != 0)	// list the assets you are trading
			{				
				for(unsigned int i = 0; i < yourAssetsTrading.size(); i++)
				{
					if(yourAssetsTrading[i] < 40)
					{
						cout << "  >>" << board[yourAssetsTrading[i]].getName() << endl;
					}
					
					else if(yourAssetsTrading[i] == 40)
					{
						cout << "  >> Get Out Jail Free Card" << endl;
					}
				}				
			}		
			
			if(yourMoneyOffer > 0)
			{
				cout << "  >> $" << yourMoneyOffer << endl;
			}
			
			if(yourAssetsTrading.size() == 0 && yourMoneyOffer == 0)		// if you are not offering anything
			{
				cout << "  >> Nothing" << endl;
			}
			cout << "TRADING THEIR: " << endl;	
			if(theirAssetsTrading.size() != 0)
			{
				for(unsigned int i = 0; i < theirAssetsTrading.size(); i++)
				{
					if(theirAssetsTrading[i] < 40)
					{
						cout << "  >>" << board[theirAssetsTrading[i]].getName() << endl;
					}
					
					else if(theirAssetsTrading[i] == 40)
					{
						cout << "  >> Get Out Jail Free Card" << endl;
					}
				}
			}
			if(theirMoneyOffer > 0)
			{
				cout << "  >> $" << theirMoneyOffer << endl;
			}
			
			if(theirAssetsTrading.size() == 0 && theirMoneyOffer == 0)		// if you do not wish to receive anything
			{
				cout << "  >> Nothing" << endl;
			}
			
			do
			{
				cin >> userDec;
				isValid = true;
				for(unsigned int l = 0; l < userDec.size(); l++)
				{
					if(userDec[l] != '0' && userDec[l] != '1' && userDec[l] != '2' && userDec[l] != '3' && userDec[l] != '4' && userDec[l] != '5' && 
					   userDec[l] != '6' && userDec[l] != '7' && userDec[l] != '8' && userDec[l] != '9')
					{
						cout << "ERROR: Input must be a number." << endl;
						isValid = false;
						break;
					}   
				}
			}while(isValid == false);
			
			userDecision = atoi(userDec.c_str());
			
			size = theirAssetsTradable.size();
			if(userDecision != 0 && userDecision <= size)
			{
				theirAssetsTrading.push_back(theirAssetsTradable[userDecision-1]);
			}
		}while(userDecision != 0);
		
		do 
		{
			cout << "Would you like to receive any money? (Y/N)" << endl;
			cin >> moneyDecision;
			if(moneyDecision == 'Y')			// if they do wish to trade money
			{
				do			// ask for input until their money offer is less than/equal to their wallet
				{
					do
					{
						cout << "They have $" << players[otherPlayer].getWallet() << endl;
						cout << "How much do you want?" << endl;
						cin >> moneyOffer;
						isValid = true;
						for(unsigned int p = 0; p < moneyOffer.size(); p++)
						{
							if(moneyOffer[p] != '0' && moneyOffer[p] != '1' && moneyOffer[p] != '2' && moneyOffer[p] != '3' && moneyOffer[p] != '4' && moneyOffer[p] != '5' && 
							   moneyOffer[p] != '6' && moneyOffer[p] != '7' && moneyOffer[p] != '8' && moneyOffer[p] != '9')
							{
								cout << "ERROR: Input must be a number." << endl;
								isValid = false;
								break;
							}   
						}
					}while(isValid == false);
					
					theirMoneyOffer = atoi(moneyOffer.c_str());
				}while(theirMoneyOffer > players[otherPlayer].getWallet());				
			}
			else if(moneyDecision == 'N')
			{
			theirMoneyOffer = 0;
			}
		}while(moneyDecision != 'Y' && moneyDecision != 'N');				
		
		do	// ask what the other player would like to do
		{
			cout << theirName << " what would you like to do?" << endl;
			cout << "D = Decline" << endl;
			cout << "C = Counter Offer" << endl;
			cout << "A = Accept" << endl;
			cin >> accept;
			if(accept == 'C')			// if they press C to counter offer
			{
				temp = yourName;		// assign temp to yourname
				yourName = theirName;	// swap yourname and theirname
				theirName = temp;		// assign theirname to temp (your name)
				temp2 = otherPlayer;	// assign temp2 to the other players id
				otherPlayer = playerNo;	// swap the player ids
				playerNo = temp2;		// assign playerNo to be the other players id
				yourAssetsTrading.clear();	// clear the assets being traded
				theirAssetsTrading.clear();	
				yourMoneyOffer = 0;			// reset the money being offered
				theirMoneyOffer = 0;
			}
		}while(accept != 'A' && accept != 'C' && accept != 'D');
	}while(accept == 'C');
	
	if(accept == 'A')		// if they accept
	{
		size = theirAssetsTrading.size();	// get the size of the vector containing items that they own
		
		if(size > 0)		// if the size is not 0
		{
			for (int i = 0; i < size; i++)
			{
				if(theirAssetsTrading[i] < 40)			// if the elemnt in the vector is not 40
				{
					board[theirAssetsTrading[i]].setOwner(yourName);	// set the owner of the corresponding property to be your name
					colour = board[theirAssetsTrading[i]].getColour();	// get the colour
					players[playerNo].addToColourCount(colour, players, playerNo, board);	// add to that player's colour count
					players[otherPlayer].removeFromColourCount(colour, players, otherPlayer, board);	// remove from that players colour count
				}
				
				else if(theirAssetsTrading[i] == 40)	// if it is 40
				{
					if(players[otherPlayer].getJailCards() > 0)		// if they still have jail cards
					{
						players[otherPlayer].decrementJailCards();		// decrement the amount of jail cards they have
					}
					
					players[playerNo].incrementJailCards();		// increment the amount of jail cards that you have
					players[playerNo].setHasJailCards(true);	// set you to have jail cards
				}			
			}
		}
		if(yourMoneyOffer > 0)						// if you offered money
		{
			players[playerNo].subtractFromWallet(yourMoneyOffer);	// subtract the amount from your wallet and total worth
			players[playerNo].subtractFromTotalWorth(yourMoneyOffer);
			players[otherPlayer].addToWallet(yourMoneyOffer);		// add the amoun to their wallet and total worth
			players[otherPlayer].addToTotalWorth(yourMoneyOffer);
		}
		
		if(theirMoneyOffer >0)						// if you receive any money
		{
			players[playerNo].addToWallet(theirMoneyOffer);			// add amount o your wallet and total worth
			players[playerNo].addToTotalWorth(theirMoneyOffer);
			players[otherPlayer].subtractFromWallet(theirMoneyOffer);	// subtract amount from their wallet and total worth
			players[otherPlayer].subtractFromTotalWorth(theirMoneyOffer);
		}
					
		size = yourAssetsTrading.size();		// get the size of your assets that are being traded
		
		if( size > 0)
		{
			for (int i = 0; i < size; i++)
			{
				if(yourAssetsTrading[i] < 40)			// if the elemt in the vector is less than 40
				{
					board[yourAssetsTrading[i]].setOwner(theirName);		// set the owner of the corresponding position to be their name
					colour = board[yourAssetsTrading[i]].getColour();		// get the colour
					players[otherPlayer].addToColourCount(colour, players, otherPlayer, board);		// add to the other players colour count
					players[playerNo].removeFromColourCount(colour, players, playerNo, board);		// remove from your colour count
				}
				
				else if(yourAssetsTrading[i] == 40)			// if it is 40
				{
					if(players[playerNo].getJailCards() > 0)		// if you still have get out jail free cards
					{
						players[playerNo].decrementJailCards();		// decrement the cards you have
					}
					
					players[otherPlayer].incrementJailCards();		// increment the cards they have
					players[otherPlayer].setHasJailCards(true);		// set them to have jail cards
				}			
			}
		}
			
	}
	
	else if(accept == 'D')		// if they chose D to decline the offer
	{
		cout << "Sorry " << yourName << "! " << theirName << " declined!" << endl; // output that they declined, return to start of turn or end of turn screen.
	}
	
	players[playerNo].updateProperties(board);		// update the properties for both players
	players[otherPlayer].updateProperties(board);
}

bool isStringValid(string test)			// tests the see if string contains invalid characters
{
	int length = test.length();			// gets the length of the string
	for(int i = 0; i < length; i++)		// converts each character to uppercase
	{
		test[i] = toupper(test[i]);
	}
	//A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
	for(int i = 0; i < length; i++)		// checks if eachcharacter is in the alphabet
	{
		if(test[i] != 'A' && test[i] != 'B' && test[i] != 'C' && test[i] != 'D' && test[i] != 'E' && test[i] != 'F' && test[i] != 'G' && 
		   test[i] != 'H' && test[i] != 'I' && test[i] != 'J' && test[i] != 'K' && test[i] != 'L' && test[i] != 'M' && test[i] != 'N' && 
		   test[i] != 'O' && test[i] != 'P' && test[i] != 'Q' && test[i] != 'R' && test[i] != 'S' && test[i] != 'T' && test[i] != 'U' && 
		   test[i] != 'V' && test[i] != 'W' && test[i] != 'X' && test[i] != 'Y' && test[i] != 'Z')
		   {
		   	return false;	// if the charcter is not any of the letters in the alphabet, return false
		   }
	}
	
	if(length > 8)
	{
        return false;          
    }
	return true;			// return true if it got through the whole string with valid characters
}

void manage(player* players, int playerNo)		// function to manage properties
{
	int propertyCount = players[playerNo].getPropertyCount();		// gets the players property count
	string userDecision;		// stores the users decision
	int userDec;				// stores the converted user decision
	int chosenProperty;			// stores the proeprty chosen by the user
	string colour;				// stores the colour of the proeprty
	int colourCount;			// stores the colour count of the player
	char userInput;				// stores a user input
	bool m;						// bool for corresponding input
	bool u;
	bool r;
	bool h;
	bool isValid;				// bool to tell if the input is valid
	int length;					// length of the input

	
	if( propertyCount == 0)		// if they have no properties
	{
		cout << "Sorry! You have no properties to manage!" << endl; // output that they have no properties and return to previous screen
		system("PAUSE");
	}
	
	else		// otherwise
	{
		
		do
		{
			m = false;		// set all bools to false
			u = false;
			r = false;
			h = false;
			system("PAUSE");
			system("cls");
			
			cout << "#####################################################" << endl;
		 	cout << "#                                                   #" << endl;
		    cout << "#                      Manage                       #" << endl;	
		    cout << "#                                                   #" << endl;
			cout << "#####################################################" << endl;
			cout << "Which property would you like to manage? (0 = Exit)" << endl;
			for(int i = 0; i < propertyCount; i++)			// output properties to be managed
			{
				chosenProperty = players[playerNo].getProperties(i);	// sets chosen property to be the current iteration
				cout << i + 1 << " = " << board[chosenProperty].getName();	// output proeprty
				if(board[chosenProperty].isMortgaged())
				{
					cout << "(MORTGAGED)" << endl;		// if it is mortgaged 
				}
				
				else
				{
					cout << endl;				// if it is not mortgaged
				}
			}
			cin >> userDecision;		// ask for input
			length = userDecision.size();		// get the size of the input
			isValid = true;			// set the validity to be true
			
			for(int j = 0; j < length; j++)	// checks if the input was valid
			{
			   if(userDecision[j] != '0' && userDecision[j] != '1' && userDecision[j] != '2' && userDecision[j] != '3' && userDecision[j] != '4' && userDecision[j] != '5' &&
			   userDecision[j] != '6' && userDecision[j] != '7' && userDecision[j] != '8' && userDecision[j] != '9')
			   {
			   		isValid = false;		// if it is not numeric, set validity to false
			   }
			}
			if(isValid == true)		// if the validity is true
			{
				userDec = atoi(userDecision.c_str());		// convert the string to an integer
				if(userDec > propertyCount)		// if the converted input is greater than the amount of properties to be managed
				{
					isValid = false;		// set validity to false
				}
			}
			
			if(isValid == true && userDec > 0)	// if the input is valid and greater than 0
			{
				chosenProperty = players[playerNo].getProperties(userDec -1);	// gets the position of the chosen property
				do		// Loop tha repeats until user input is valid
				{
					colour = board[chosenProperty].getColour();		// get the colour of the chosen proeprty
					colourCount = players[playerNo].getColourCount(colour);	// get the colour count for that colour
					
					cout << board[chosenProperty].getName() << endl;	// output the name of the property
					if(board[chosenProperty].isMortgaged())		// if the property is mortgaged output that they can unmortgage it
					{
						cout << "U = Unmortgage" << endl;
						u = true;// set u to be true
					}
					if(board[chosenProperty].isMortgaged() == false && board[chosenProperty].hasHousesOnSet(colour, board) == false) // if the property is not mortgaged and no properties in the colopur group have houses on them
					{
						cout << "M = Mortgage" << endl;
						m = true;	// set m to be true
					}
					if ((colourCount == 3 && colour != "Station") && board[chosenProperty].isMortgaged() == false && board[chosenProperty].hasMortgagedProperty(colour, board) == false) // if the colour count is 3 and not a station, and isnt mortgaged or has any mortgaged properties in the group
					{
						if(board[chosenProperty].getHouses() < 4) // if there are less than 4 houses
						{
							cout << "H = Add House" << endl;
							h = true;		// h set to true
						}
						
						else if(board[chosenProperty].getHouses() == 4) // if there are 4 houses on the proeprty, and hotels left in the bank
						{
							cout << "H = Add Hotel" << endl;
							h = true;				// set h to true
						}
					}
					
					if(colourCount == 2 && (colour == "Blue" || colour == "Brown") && board[chosenProperty].isMortgaged() == false && board[chosenProperty].hasMortgagedProperty(colour, board) == false)	// if the colour count is 2, and the colour is blue or brown
					{																																														// and the property is not mortgaged, or any other in the set mortgaged
						if(board[chosenProperty].getHouses() < 4)		// if there are less than 4 houses on the proeprty
						{
							cout << "H = Add House" << endl;
							h = true;			// h set to true
						}
						
						else if(board[chosenProperty].getHouses() == 4) // if there are 4 houses on the proeprty
						{
							cout << "H = Add Hotel" << endl;			// set h to be true
							h = true;
						}
					}
					if(board[chosenProperty].getHouses() > 0)			// if there are houses on the proeprty
					{
						if(board[chosenProperty].getHouses() <= 4)		// if there are less than 5 houses on the property
						{
							cout << "R = Remove House" << endl;
						}
						
						else if(board[chosenProperty].getHouses() == 5)	// if there are 5 houses on the proeprty
						{
							cout << "R = Remove Hotel" << endl;
						}
						r = true;			// set r to be true;
					}
					cout << "X = Return" << endl;
					cin >> userInput;			// ask for input
					system("PAUSE");
				}while((userInput == 'U' && u == false) || (userInput == 'M' && m == false) || (userInput == 'H' && h == false) || (userInput == 'R' && r == false)); // if the input is not valid, repeat
						
				if(userInput == 'M')
				{
					mortgage(players, chosenProperty, playerNo);	// call the mortgage function if they entered m
				}
				
				else if(userInput == 'U')
				{
					unmortgage(players, chosenProperty, playerNo); // call the unmortgage function if they entered u
				}
				
				else if(userInput == 'R')
				{
					if(board[chosenProperty].getHouses() <= 4)	// call the remove house function if there are less than 5 houses on the property and r is true
					{
  					    removeHouse(players, chosenProperty, playerNo);
					}
					
					else if(board[chosenProperty].getHouses() == 5) // call the remove hotel function if the input was r and the house count is 5, and r is true
					{
						removeHotel(players, chosenProperty, playerNo);
					}					
				}
				else if(userInput == 'H')		
				{
					if(board[chosenProperty].getHouses() < 4)		// call the add house function if h is true, and the houses on the proeprty is less than 5
					{
  					    addHouse(players, chosenProperty, playerNo);
					}
					
					else if(board[chosenProperty].getHouses() == 4)	// call the add hotel function if the house count is 4 and h is true
					{
						addHotel(players, chosenProperty, playerNo);
					}					
				}					
			}
				
		}while(userDec != 0);	
	}
}

void mortgage(player* players, int propertyPos, int playerNo)		// function to mortgage properties
{
	int mortgageValue;			// stores the vmortgage alue of the property 
	int propertyPrice;			// stores the price of the property
	
	cout << "You mortgaged " << board[propertyPos].getName() << endl;	// output which property was mortgaged
	mortgageValue = board[propertyPos].getMortgageValue();		// get the mortgage value
	cout << "You receive $" << mortgageValue << endl;		// output the mortgage value
	players[playerNo].addToWallet(mortgageValue);		// add to the players wallet and total worth the value of the mortgage
	players[playerNo].addToTotalWorth(mortgageValue);
	
	propertyPrice = board[propertyPos].getPrice();	// get the price of the proeprty
	players[playerNo].subtractFromTotalWorth(propertyPrice);	// subtract the price from the total worth
	board[propertyPos].setMortgaged(true);	// set the property to be mortgaged
}

void unmortgage(player* players, int propertyPos, int playerNo)	// unmortgage function
{
	int unmortgageCost = board[propertyPos].getUnmortgageValue();	// get the cost to unmortgage the property
	bool canPay = (players[playerNo].getWallet() >= unmortgageCost);	// check if they can afford to unmortgage it
	int propertyPrice;
	if(canPay == true)	// if they can afford to 
	{
		cout << "You unmortgaged " << board[propertyPos].getName() << endl; // output what was unmortgaged
		cout << "You paid $" << unmortgageCost << endl;	// output what was paid
		
		players[playerNo].subtractFromWallet(unmortgageCost);		// subtract the money from the wallet and total worth
		players[playerNo].subtractFromTotalWorth(unmortgageCost);
		
		propertyPrice = board[propertyPos].getPrice();	// get the price of the proeprty
		players[playerNo].addToTotalWorth(propertyPrice);		// add the price to the total worth
		board[propertyPos].setMortgaged(false);	// set the property to be unmortgaged
	}
	else if(canPay == false)		// if they cannot afford it
	{
		cout << "Sorry you cant afford that! Go back and sell houses or mortgage properties first!" << endl;
	}	
}

void addHouse(player* players, int propertyPos, int playerNo) // function to add a house
{
	bool canPay = players[playerNo].getWallet() >= board[propertyPos].getHouseCost();	// check if they can afford a house
	int cost = board[propertyPos].getHouseCost();		// get the cost of the house
	string colour = board[propertyPos].getColour();		// get the colour of the property
	int leastAmountOfHouses = 5;		// set the least amount of houses to be 5
	int currentHouses = board[propertyPos].getHouses();	// get the amount of houses on the property
	int newRent;	// stores the new rent of the property
	int houses;		// stores the amount of houses on other properties
	
	for(int i = 0; i < 40; i++)		// checks if they are building evenly
	{
		if(board[i].getColour() == colour)	// if they are in the same colour group
		{
			houses = board[i].getHouses();	// get the amount of houses
			if(houses < leastAmountOfHouses)		// if the amount of houses is less than the current smallest amount
			{
				leastAmountOfHouses = houses;	//set the new amount to be the amount of houses on the current property
			}
		}
	}
	
	if(currentHouses > leastAmountOfHouses)		// if the amount of houses on the current property is greater than the amount on another property in the group
	{
		cout << "You must build on the other properties of this group, before building again on this one." << endl;
	}
	
	else if(currentHouses == leastAmountOfHouses)	// if they are the same as the least amout of houses
	{
		if(canPay)	// is they can pay
		{
			if(housesLeft > 0)	// if there are houses left
			{
				cout << "You bought a house for $" << cost << endl;
				players[playerNo].subtractFromWallet(cost);	// subtract money from total worth and wallet
				board[propertyPos].setHouses(currentHouses+1);
				housesLeft--;	// decrement the houses left
				newRent = board[propertyPos].getRentForHouseCount(currentHouses+1);			// get the new rent for the amount of houses on the property 
				board[propertyPos].setCurrentRent(newRent);		// set the new rent 
			}
			
			else if(housesLeft == 0)	// if there are no more houses
			{
				cout << "Sorry, there are no houses left to buy!" << endl;
			}
		}
		
		else if(canPay == false)	// if they cannot afford it
		{
			cout << "Sorry you cant afford that! Go back and sell houses or mortgage properties first!" << endl;
		}			
	}		
}

void addHotel(player* players, int propertyPos, int playerNo)	// add hotel function, exactly the same as the above function, with hotels instead of houses
{
	bool canPay = players[playerNo].getWallet() >= board[propertyPos].getHouseCost();
	int cost = board[propertyPos].getHouseCost();
	string colour = board[propertyPos].getColour();
	int leastAmountOfHouses = 5;
	int currentHouses = board[propertyPos].getHouses();
	int newRent;
	int houses;
	
	for(int i = 0; i < 40; i++)
	{
		if(board[i].getColour() == colour)
		{
			houses = board[i].getHouses();
			if(houses < leastAmountOfHouses)
			{
				leastAmountOfHouses = houses;
			}
		}
	}
	
	if(currentHouses > leastAmountOfHouses)
	{
		cout << "You must build on the other properties of this group, before building again on this one." << endl;
	}
	
	else if(currentHouses == leastAmountOfHouses)
	{
		if(canPay)
		{
			if(hotelsLeft > 0)
			{
				cout << "You bought a hotel for $" << cost << endl;
				players[playerNo].subtractFromWallet(cost);
				board[propertyPos].setHouses(currentHouses+1);
				hotelsLeft--;
				newRent = board[propertyPos].getRentForHouseCount(currentHouses+1);
				board[propertyPos].setCurrentRent(newRent);
			}
			
			else if(hotelsLeft == 0)
			{
				cout << "Sorry, there are no hotels left to buy!" << endl;
			}
		}
		
		else if(canPay == false)
		{
			cout << "Sorry you cant afford that! Go back and sell houses or mortgage properties first!" << endl;
		}			
	}
}
void removeHouse(player* players, int propertyPos, int playerNo)		// function to remove houses
{
	int cost = board[propertyPos].getHouseCost() / 2;					// get the cost of a house and divide by 2
	string colour = board[propertyPos].getColour();						// get the colour of the property
	int mostAmountOfHouses = 0;											// set the most amount of houses to be 0
	int currentHouses = board[propertyPos].getHouses();					// get the current houses on the current property
	int newRent;														// stores the new rent of a property
	int houses;															// stores the houses on other properties in the colour group
	
	for(int i = 0; i < 40; i++)
	{
		if(board[i].getColour() == colour)	// if the colours match
		{
			houses = board[i].getHouses();		// get the amount of houses
			if(houses > mostAmountOfHouses)		// if the amount of houses on this property is greater than the most amount of houses
			{
				mostAmountOfHouses = houses;	// set a new amount 
			}
		}
	}
	
	if(currentHouses < mostAmountOfHouses) // if the houses on the current property is less than the property with the most on them
	{
		cout << "You must remove buildings on the other properties of this group, before removing more on this one." << endl;
	}
	
	else if(currentHouses == mostAmountOfHouses)		// if they are equal
	{
		cout << "You sold a house for $" << cost << endl;	// output how much it was sold for
		players[playerNo].addToWallet(cost);		// add the amount to the wallet and total worth
		players[playerNo].addToTotalWorth(cost);
		players[playerNo].subtractFromTotalWorth(cost*2);	// subtract the value of a house from the total worth
		board[propertyPos].setHouses(currentHouses-1);	// set the amount of houses on the current property
		housesLeft++;	// increment the houses left in the bank
		newRent = board[propertyPos].getRentForHouseCount(currentHouses-1); // get rent for the new amount of houses
		board[propertyPos].setCurrentRent(newRent);				// set the new rent
	}		
}

void removeHotel(player* players, int propertyPos, int playerNo)		//  same as above, with hotels instead of houses
{
	int cost = board[propertyPos].getHouseCost() / 2;
	string colour = board[propertyPos].getColour();
	int mostAmountOfHouses = 0;
	int currentHouses = board[propertyPos].getHouses();
	int newRent;
	int houses;
	
	for(int i = 0; i < 40; i++)
	{
		if(board[i].getColour() == colour)
		{
			houses = board[i].getHouses();
			if(houses > mostAmountOfHouses)
			{
				mostAmountOfHouses = houses;
			}
		}
	}
	
	if(currentHouses < mostAmountOfHouses)
	{
		cout << "You must remove buildings on the other properties of this group, before removing more on this one." << endl;
	}
	
	else if(currentHouses == mostAmountOfHouses)
	{
		cout << "You sold a hotel for $" << cost << endl;
		players[playerNo].addToWallet(cost);
		players[playerNo].addToTotalWorth(cost);
		players[playerNo].subtractFromTotalWorth(cost*2);
		board[propertyPos].setHouses(currentHouses-1);
		hotelsLeft++;
		newRent = board[propertyPos].getRentForHouseCount(currentHouses-1);
		board[propertyPos].setCurrentRent(newRent);
	}		
}

void mainMenu()	// function to display the main menu of the game
{
	char userDecision;	// stores the user decision
	
	do	// repeat until user decides to begin the game
	{
		system("cls");
		do			//repeat until user inputs valid character
		{
			system("cls");
			cout << "#####################################################" << endl;
			cout << "#    __  __           HASBRO(c)            _        #" << endl;
			cout << "#   |  \\/  |                             | |        #" << endl;
			cout << "#   | \\  / | ___  _ __   ___  _ __   ___ | |_   _   #" << endl;
			cout << "#   | |\\/| |/ _ \\| '_ \\ / _ \\| '_ \\ / _ \\| | | | |  #" << endl;
			cout << "#   | |  | | (_) | | | | (_) | |_) | (_) | | |_| |  #" << endl;
			cout << "#   |_|  |_|\\___/|_| |_|\\___/| .__/ \\___/|_|\\__, |  #" << endl;
			cout << "#                            | |             __/ |  #" << endl;
			cout << "#                            |_|            |___/   #" << endl;
			cout << "#   2 - 6 Players                                   #" << endl;
			cout << "#####################################################" << endl;
			cout << "#                                                   #" << endl;
			cout << "#  R = Rules                        B = Begin Game  #" << endl;	
			cout << "#                                                   #" << endl;
			cout << "#####################################################" << endl;
			cin >> userDecision;
		
		}while(userDecision != 'R' && userDecision != 'B');
	
		if(userDecision == 'R') // if they entered r, display the rules
		{
			displayRules();
		}	
	}while(userDecision != 'B');
} 

void displayRules()		// function to siplay rules
{
	system("cls");
	cout << "#####################################################" << endl;
	cout << "#    __  __           HASBRO(c)            _        #" << endl;
	cout << "#   |  \\/  |                             | |        #" << endl;
	cout << "#   | \\  / | ___  _ __   ___  _ __   ___ | |_   _   #" << endl;
	cout << "#   | |\\/| |/ _ \\| '_ \\ / _ \\| '_ \\ / _ \\| | | | |  #" << endl;
	cout << "#   | |  | | (_) | | | | (_) | |_) | (_) | | |_| |  #" << endl;
	cout << "#   |_|  |_|\\___/|_| |_|\\___/| .__/ \\___/|_|\\__, |  #" << endl;
	cout << "#                            | |             __/ |  #" << endl;
	cout << "#                            |_|            |___/   #" << endl;
	cout << "#   2 - 6 Players                                   #" << endl;
	cout << "#####################################################" << endl;
	cout << "#                                                   #" << endl;
	cout << "#                       Object                      #" << endl;	
	cout << "#                                                   #" << endl;
	cout << "#####################################################" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  The object of the game is to become the richest  #" << endl;
	cout << "#  player by buying, renting and selling property.  #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#####################################################" << endl;
	cout << "#                                                   #" << endl;
	cout << "#                    Preparation                    #" << endl;	
	cout << "#                                                   #" << endl;
	cout << "#####################################################" << endl;
	cout << "#                                                   #" << endl;
	cout << "#   As this is a digital game, no preparation is    #" << endl;
	cout << "#   needed. Each player starts with $1500 in their  #" << endl;
	cout << "#   wallet and no properties. The Chance and        #" << endl;
	cout << "#   Community Chest cards are shuffled.             #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#####################################################" << endl;
	cout << "#                                                   #" << endl;
	cout << "#                     The Play                      #" << endl;	
	cout << "#                                                   #" << endl;
	cout << "#####################################################" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  The player's each roll the die in turn, and the  #" << endl;
	cout << "#  player with the highest roll starts.             #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  At the start of each turn, the player decides    #" << endl;
	cout << "#  what action they would like to take.             #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  The player may trade with other players, manage  #" << endl;
	cout << "#  the properties they own, declare bankruptcy,     #" << endl;
	cout << "#  end the game, or roll the dice.                  #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  These options are also available at the end of   #" << endl;
	cout << "#  the player's turn, with the exception of rolling #" << endl;
	cout << "#  the dice, which is replaced instead by an option #" << endl;
	cout << "#  to end the turn.                                 #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  During the player's turn, they may be able to    #" << endl;
	cout << "#  purchase property. If they cannot afford it or   #" << endl;
	cout << "#  do not wish to buy it, it is auctioned off to    #" << endl;
	cout << "#  all of the players in the game.                  #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#  When improving properties, the player must first #" << endl;
	cout << "#  have all properties in the colour group, once    #" << endl;
	cout << "#  all proeprties in the group are owned by the     #" << endl;
	cout << "#  player they may erect houses on them to improve  #" << endl;
	cout << "#  the rent. Houses must be built evenly across all #" << endl;
	cout << "#  properties in the group. This means that a 2nd   #" << endl;
	cout << "#  house cannot be built until all properties in    #" << endl;
	cout << "#  the group have one house on them. This is true   #" << endl;
	cout << "#  for the removal of houses as well.               #" << endl;	
	cout << "#                                                   #" << endl;
	cout << "#  Player's can mortgage unimproved properties in   #" << endl;
	cout << "#  order to gain money during the game. No rent is  #" << endl;
	cout << "#  collected on mortgaged properties.               #" << endl;
	cout << "#                                                   #" << endl;
	cout << "#####################################################" << endl;
	system("PAUSE");
}


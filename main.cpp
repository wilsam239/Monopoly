#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include "square.h"
#include "player.h"

using namespace std;

void primeRead();
int mainMenu();
void errorMenuScreen();
char endOfTurnScreen(int playerNo, string name, int wallet, int position, string* properties, player* players);
char startOfTurnScreen(int playerNo, string name, int wallet, int position, string* properties, player* players);
int spaceCountCheck(int spaceCount);
square* assignPropAttr(square* array);
player* assignPlayerAttr(player* array, int playerCount);
int* rollDice(int* array);
bool isDoubles(int dice1, int dice2, int playerNo);

square arrayOfProperties[40];
square* allSquares = assignPropAttr(arrayOfProperties);
int turnCount = 0;

int main(int argc, char** argv) 
{
	char userInput;	 													// userInput on start of turn and end of turn screen
	int playerCount;													// amount of players
	int arrayOfDice[2];													// array to store dice;
	bool playerRolledDoubles;											// boolean for when player rolls doubles
	int sumOfDice;														// is the sum of two dice
	int currentWallet;													// current player's wallet
	int currentPos;														// current player's position
	int newPos;															// current player's new position
	string currentName;													// current player's name
	string playerProperties[28];										// current player's properties
	playerCount = mainMenu();											// calls the main menu function
													
	player arrayOfPlayers[playerCount];									// creates an array that will be sent to a function
	player* players = assignPlayerAttr(arrayOfPlayers, playerCount);	// assigns attributes to the players
	system("PAUSE");
	system("cls");
	
	for ( ; ; )															// infinite loop that will be broken when all player's are bankrupt
	{	
		turnCount++;															// or when game is terminated		
		for (int i = 0; i < playerCount ; i++)
		{
			currentName = players[i].getName();
			currentWallet = players[i].getWallet();
			currentPos = players[i].getPosition();
			players[i].setManaged(false);
			for(int j = 0; j < 28; j++)
			{
				playerProperties[j] = players[i].getProperties(j);
			}
			
			do
			{
				userInput = startOfTurnScreen(i, currentName, currentWallet, currentPos, playerProperties, players);
				if (userInput == 'X')
				{
					cout << "Terminating Game..." << endl;
					exit(0);
				}
				
				else if(userInput == 'T')
				{
					cout << "Call Trade Function" << endl;
				}
				
				else if(userInput == 'M')
				{
					cout << "Call Manage Function" << endl;
				}
				
				else if(userInput == 'D')
				{
					cout << currentName << " is now bankrupt!" << endl;
					cout << "Call bankrupt function to return owned proeprties to bank" << endl;
				}
			}while(userInput != 'R');
			
			
			int* dice = rollDice(arrayOfDice);
			sumOfDice = dice[0] + dice[1];
			
			newPos = currentPos + sumOfDice;
			if (newPos > 39)
			{
				newPos -= 40;
				cout << "You passed GO! Collect $200." << endl;
				currentWallet += 200;
				players[i].setWallet(currentWallet);
			}
			
			players[i].setPosition(newPos);
			currentPos = players[i].getPosition();
			
			cout << currentName << " is now on " << allSquares[currentPos].getName() << endl;
			
			if(allSquares[newPos].getOwner() == "USER")
			{
				cout << "This property can be purchased" << endl;
				cout << "Call purchase function" << endl;
			}
			
			playerRolledDoubles = isDoubles(dice[0], dice[1], i);
			if (playerRolledDoubles)
			{
				cout << "You got doubles!" << endl;
				cout << "ROLL AGAIN" << endl;
				players[i].incrementDoublesCount();
				i--;
			}
			
			do
			{
				userInput = endOfTurnScreen(i, currentName, currentWallet, currentPos, playerProperties, players);
				if (userInput == 'X')
				{
					cout << "Terminating Game..." << endl;
					exit(0);
				}
				
				else if(userInput == 'T')
				{
					cout << "Call Trade Function" << endl;
				}
				
				else if(userInput == 'M')
				{
					cout << "Call Manage Function" << endl;
				}
				
				else if(userInput == 'D')
				{
					cout << currentName << " is now bankrupt!" << endl;
					cout << "Call bankrupt function to return owned proeprties to bank" << endl;
				}
			}while(userInput != 'E');
						
		}
	}
	system("PAUSE");
	
	
	return 0;
}

void primeRead()
{
	// Sam
	ifstream propertyNameFile;
	propertyNameFile.open("txts/propertyNames.txt");
	ifstream propertyPriceFile;
	propertyPriceFile.open("txts/propertyPrice.txt");	
	ifstream propertyRentFile;
	propertyRentFile.open("txts/propertyBaseRent.txt");
	ifstream propertyOwnerFile;
	propertyOwnerFile.open("txts/propertyOwner.txt");
	ifstream propertyRentHotelFile;
	propertyRentHotelFile.open("txts/propertyRentHotel.txt");
	ifstream propertyColourFile;
	propertyColourFile.open("txts/propertyColour.txt");
	ifstream propertyHousesCostFile;
	propertyHousesCostFile.open("txts/propertyHouseCost.txt");
	ifstream propertyMortgageCostFile;
	propertyMortgageCostFile.open("txts/propertyMortgageValue.txt");
	ifstream propertyUnmortgageCostFile;
	propertyUnmortgageCostFile.open("txts/propertyUnmortgageValue.txt");
	ifstream propertyRentOneFile;
	propertyRentOneFile.open("txts/propertyRentOneHouse.txt");
	ifstream propertyRentTwoFile;
	propertyRentTwoFile.open("txts/propertyRentTwoHouse.txt");
	ifstream propertyRentThreeFile;
	propertyRentThreeFile.open("txts/propertyRentThreeHouse.txt");	
	ifstream propertyRentFourFile;
	propertyRentFourFile.open("txts/propertyRentFourHouse.txt");
					
	string primer;
	int spaceCount = 0;
    while(getline(propertyNameFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking Names" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while (getline(propertyPriceFile,primer))
	{
		spaceCount++;		
	}
//	cout << "checking price" << endl;
	spaceCountCheck(spaceCount);

	spaceCount = 0;
	while (getline(propertyRentFile,primer))
	{
		spaceCount++;
	}
//	cout << "checking rent" << endl;
	spaceCountCheck(spaceCount);

	spaceCount = 0;
	while(getline(propertyOwnerFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking Owner" << endl;
	spaceCountCheck(spaceCount);

	spaceCount = 0;
	while(getline(propertyColourFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking Colour" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyHousesCostFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking House Cost" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyMortgageCostFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking mortgage" << endl;
	spaceCountCheck(spaceCount);

	spaceCount = 0;
	while(getline(propertyUnmortgageCostFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking unmortgage" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyRentOneFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking rent w 1" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyRentTwoFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking rent w 2" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyRentThreeFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking rent w 3" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyRentFourFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking rent w 4" << endl;
	spaceCountCheck(spaceCount);
	
	spaceCount = 0;
	while(getline(propertyRentHotelFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking rent w H" << endl;
	spaceCountCheck(spaceCount);
	
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
}

int spaceCountCheck(int spaceCount)
{
	//Sam
	if (spaceCount != 40)
	{
		cout << "ERROR: Board Incomplete" << endl;
		cout << "Terminating Game.." << endl;
		system("PAUSE");
		exit(0);
	}
}

square* assignPropAttr(square* array)
{
	//Sam
	primeRead();
	ifstream propertyNames;
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
		
	square* allSquares = new square[40];
	
	string propertyName, propertyPrice, propertyRent, propertyOwner, propertyColour, propertyCostOfHouse;
	string propertyMortgageValue, propertyUnmortgageValue, propertyRentOneHouse, propertyRentTwoHouse;
	string propertyRentThreeHouse, propertyRentFourHouse, propertyRentHotel, propertyCurrentRent;
	int priceOfProp, baseRent, houseCost, mortgageValue, unmortgageCost, rentWithOne, rentWithTwo;
	int rentWithThree, rentWithFour, rentWithHotel, currentRent;

	
	int count = 0;
	while (getline(propertyNames,propertyName))
	{
		allSquares[count].setName(propertyName);
		count++;
	}
	
	count = 0;
	while (getline(propertyPrices,propertyPrice))
	{
		priceOfProp = atoi(propertyPrice.c_str());
		allSquares[count].setPrice(priceOfProp);
		count++;
	}

	count = 0;
	while (getline(propertyRents,propertyRent))
	{
		baseRent = atoi(propertyRent.c_str());
		allSquares[count].setBaseRent(baseRent);
		count++;
	}
	
	count = 0;
	while (getline(propertyOwners,propertyOwner))
	{
		allSquares[count].setOwner(propertyOwner);
		count++;
	}
	
	count = 0;
	while (getline(propertyColours,propertyColour))
	{
		allSquares[count].setColour(propertyColour);
		count++;
	}
	
	count = 0;
	while (getline(propertyHouseCost,propertyCostOfHouse))
	{
		houseCost = atoi(propertyCostOfHouse.c_str());
		allSquares[count].setHouseCost(houseCost);
		count++;
	}
	
	count = 0;
	while (getline(propertyMortgage,propertyMortgageValue))
	{
		mortgageValue = atoi(propertyMortgageValue.c_str());
		allSquares[count].setMortgageValue(mortgageValue);
		count++;
	}
	
	count = 0;
	while (getline(propertyUnmortgage,propertyUnmortgageValue))
	{
		unmortgageCost = atoi(propertyUnmortgageValue.c_str());
		allSquares[count].setUnmortgageValue(unmortgageCost);
		count++;
	}
	
	count = 0;
	while (getline(propertyR1,propertyRentOneHouse))
	{
		rentWithOne = atoi(propertyRentOneHouse.c_str());
		allSquares[count].setR1House(rentWithOne);
		count++;
	}
	
	count = 0;
	while (getline(propertyR2,propertyRentTwoHouse))
	{
		rentWithTwo = atoi(propertyRentTwoHouse.c_str());
		allSquares[count].setR2House(rentWithTwo);
		count++;
	}
	
	count = 0;
	while (getline(propertyR3,propertyRentThreeHouse))
	{
		rentWithThree = atoi(propertyRentThreeHouse.c_str());
		allSquares[count].setR3House(rentWithThree);
		count++;
	}
	
	count = 0;
	while (getline(propertyR4,propertyRentFourHouse))
	{
		rentWithFour = atoi(propertyRentFourHouse.c_str());
		allSquares[count].setR4House(rentWithFour);
		count++;
	}
	
	count = 0;
	while (getline(propertyRH,propertyRentHotel))
	{
		rentWithHotel = atoi(propertyRentHotel.c_str());
		allSquares[count].setRHotel(rentWithHotel);
		count++;
	}
	
	for (int i = 0; i < 40; i++)
	{
		allSquares[i].setCurrentRent(allSquares[i].getBaseRent());
		allSquares[i].setHouses(0);
	}
	
	propertyNames.close();
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
	
	return allSquares;
}

player* assignPlayerAttr(player* array, int playerCount)
{
	//Sam
	player* allPlayers = new player[playerCount];
	string playerName;
	for (int i = 0; i < playerCount; i++)
	{
		cout << "What is Player " << i +1 << "'s name?" << endl;
		cin >> playerName; 
		allPlayers[i].setName(playerName);
		allPlayers[i].setWallet(1500);
		allPlayers[i].setPosition(0);
		allPlayers[i].setBankruptStatus(false);
		allPlayers[i].setManaged(false);
		for (int j = 0; j< 28; j++)
		{
			allPlayers[i].setProperties(j, "NULL");
		}
	}	
	return allPlayers;		
}

int* rollDice(int* array)												// function to roll and output dice roll
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
																								
	if(diceValues[0] == 1 && diceValues[1] == 1 || diceValues[1] == 1 && diceValues[0] == 1 )					// the rest of this function is for ascii art
	{																												// I have done every combination of numbers but
		cout << " ---------- " << "    ---------- " << endl;														// order is not important for the ascii output
		cout << "|          |" << "   |          |" << endl;														// eg: if i roll a 4 and a 3, it will output the
		cout << "|          |" << "   |          |" << endl;														// same ascii art for both variations
		cout << "|    ()    |" << "   |    ()    |" << endl;										
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[0] == 1 && diceValues[1] == 2 || diceValues[1] == 1 && diceValues[0] == 2 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[0] == 1 && diceValues[1] == 3 || diceValues[1] == 1 && diceValues[0] == 3 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[0] == 1 && diceValues[1] == 4 || diceValues[1] == 1 && diceValues[0] == 4 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if(diceValues[0] == 1 && diceValues[1] == 5 || diceValues[1] == 1 && diceValues[0] == 5 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[0] == 1 && diceValues[1] == 6 || diceValues[1] == 1 && diceValues[0] == 6 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}	
	
	else if(diceValues[1] == 2 && diceValues[0] == 2 || diceValues[0] == 2 && diceValues[1] == 2 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 2 && diceValues[0] == 3 || diceValues[0] == 2 && diceValues[1] == 3 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 2 && diceValues[0] == 4 || diceValues[0] == 2 && diceValues[1] == 4 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 2 && diceValues[0] == 5 || diceValues[0] == 2 && diceValues[1] == 5 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}			
	
	else if(diceValues[1] == 2 && diceValues[0] == 6 || diceValues[0] == 2 && diceValues[1] == 6 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 3 && diceValues[0] == 3 || diceValues[0] == 3 && diceValues[1] == 3 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()       |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   |       () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 3 && diceValues[0] == 4 || diceValues[0] == 3 && diceValues[1] == 4 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if(diceValues[1] == 3 && diceValues[0] == 5 || diceValues[0] == 3 && diceValues[1] == 5 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if(diceValues[1] == 3 && diceValues[0] == 6 || diceValues[0] == 3 && diceValues[1] == 6 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()       |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|       () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 4 && diceValues[0] == 4 || diceValues[0] == 4 && diceValues[1] == 4 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 4 && diceValues[0] == 5 || diceValues[0] == 4 && diceValues[1] == 5 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   |    ()    |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if(diceValues[1] == 4 && diceValues[0] == 6 || diceValues[0] == 4 && diceValues[1] == 6 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|          |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
		
	else if(diceValues[1] == 5 && diceValues[0] == 5 || diceValues[0] == 5 && diceValues[1] == 5 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   |    ()    |"<< endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}

	else if(diceValues[1] == 5 && diceValues[0] == 6 || diceValues[0] == 5 && diceValues[1] == 6 )
	{
		cout << " ---------- " << "    ---------- " << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "|    ()    |" << "   | ()    () |"<< endl;
		cout << "|          |" << "   |          |" << endl;
		cout << "| ()    () |" << "   | ()    () |" << endl;
		cout << " ---------- " << "    ---------- " << endl;
	}
	
	else if(diceValues[1] == 6 && diceValues[0] == 6 || diceValues[0] == 6 && diceValues[1] == 6 )
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

bool isDoubles(int dice1, int dice2, int playerNo)
{
	//Sam
	if (dice1 == dice2)
	{
		return true;
	}
	
	else
	{
		return false;
	}
}

int mainMenu()
{
	//Sam
	 string playerCountStr;
	 int playerCount;
	 bool isNotValidDigits = false;
	 
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
	     cin >> playerCountStr;
	     
	     for (int i = 0; i < playerCountStr.size(); i++)
	     {
	     	if (playerCountStr[i] != '2' && playerCountStr[i] != '3' && playerCountStr[i] != '4' && playerCountStr[i] != '5' && 
				playerCountStr[i] != '6')
			{
				isNotValidDigits = true;
				break;
			}
			else
			{
				isNotValidDigits = false;
			}			
		 }
		 
		 if(isNotValidDigits == false)
	     {
			playerCount = atoi(playerCountStr.c_str());
		 }
		 
		 if(isNotValidDigits == true || playerCount < 1 || playerCount > 6)
		 {
		 	errorMenuScreen();
		 }
		 	 	
	 }while(isNotValidDigits == true || playerCount < 1 || playerCount > 6);
	 return playerCount;     
}

void errorMenuScreen()
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

char startOfTurnScreen(int playerNo, string name, int wallet, int position, string* properties, player* players)
{
	char userDecision;
	//bool playerCannotManage;
	//playerCannotManage = players[playerNo].hasManagedAlready();
	
	cout << "TURN: " << turnCount << endl;
	cout << "NAME: " << name << endl;
	cout << "WALLET: $" << wallet << endl;
	cout << "POSITION: " << allSquares[position].getName() << endl;
	cout << "PROPERTIES OWNED: " << endl;
	int propertyCount = 28;
	for (int i = 0; i < 28; i++)
	{
		if(properties[i] != "NULL")
		{
			cout << properties[i] << endl;
		}
		
		else
		{
			propertyCount -= 1;
		}
	}
	
	if (propertyCount == 0)
	{
		cout << "NO PROPERTIES OWNED" << endl;
	}
	
	do
	{
		cout << "D = Declare Bankrupt" << endl;
		cout << "R = Roll Dice" << endl;
		cout << "X = Exit Game" << endl;
		if (propertyCount != 0 && players[playerNo].hasManagedAlready() == false)
		{
			cout << "M = Manage Properties" << endl;	
		}
		cout << "T = Trade" << endl;
		cin >> userDecision;
		
		if (userDecision == 'M' && players[playerNo].hasManagedAlready() == false && propertyCount != 0)
		{
			return userDecision;
		}		
	}while(userDecision != 'D' && userDecision != 'R' && userDecision != 'X' && userDecision != 'T');
	
	return userDecision;
}

char endOfTurnScreen(int playerNo, string name, int wallet, int position, string* properties, player* players)
{
	char userDecision;
	//bool playerCannotManage;
	//playerCannotManage = players[playerNo].hasManagedAlready();
	
	cout << "TURN: " << turnCount << endl;
	cout << "NAME: " << name << endl;
	cout << "WALLET: $" << wallet << endl;
	cout << "POSITION: " << allSquares[position].getName() << endl;
	cout << "PROPERTIES OWNED: " << endl;
	int propertyCount = 28;
	for (int i = 0; i < 28; i++)
	{
		if(properties[i] != "NULL")
		{
			cout << properties[i] << endl;
		}
		
		else
		{
			propertyCount -= 1;
		}
	}
	
	if (propertyCount == 0)
	{
		cout << "NO PROPERTIES OWNED" << endl;
	}
	
	do
	{
		cout << "D = Declare Bankrupt" << endl;
		cout << "E = End Turn" << endl;
		cout << "X = Exit Game" << endl;
		if (propertyCount != 0 && players[playerNo].hasManagedAlready() == false)
		{
			cout << "M = Manage Properties" << endl;	
		}
		cout << "T = Trade" << endl;
		cin >> userDecision;
		
		if (userDecision == 'M' && players[playerNo].hasManagedAlready() == false && propertyCount != 0)
		{
			return userDecision;
		}		
	}while(userDecision != 'D' && userDecision != 'E' && userDecision != 'X' && userDecision != 'T');
	
	return userDecision;
}

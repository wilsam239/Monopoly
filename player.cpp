//player.cpp
#include <string>
#include <cstdlib>
#include <iostream>
#include "player.h"
#include "gameTile.h"

using namespace std;

player::player()		// default constructor
{
	wallet = 1500;		// set the wallet to 1500
	currentPosition = 0;	// set the position to 0
	totalWorth = 1500;	// set the total worth to 1500
	isBankrupt = false;	// set the bankrupt status to false
	inJail = false;		// set in player to be not in jail
	turnsInJail = 0;	// set the turns in jail to 0
	hasJailCards = false;	// set the player to not have jail cards
	jailCards = 0;			
	
	browns = 0;		// set count for colours to 0
	lightBlues = 0;
	purples = 0;
	oranges = 0;
	reds = 0;
	yellows = 0;
	greens = 0;
	blues = 0;
	stations = 0;
	utilities = 0;	
	
	for(int i = 0; i < 28; i++)	// populate the properties owned by the player with sentinel values
	{
		propertiesOwned[i] = 99;
	}
}

int player::getPropertyCount()	// return the property count for the player
{
	return propertyCount;
}

void player::updateProperties(gameTile* board)	// update the properties owned by the player
{
	int count = 0;
	for(int i = 0; i < 40; i++)		// iterate through the board
	{
		if(board[i].getOwner() == name)		// if the owner is the players name
		{
			propertiesOwned[count] = i;	// add the current iteration to the properties owned array
			count++;		// increment the count
		}
	}
	propertyCount = count;	// set the propertyCount to be the count
}

void player::addToColourCount(string colour, player* players, int playerNo, gameTile* board)	// add to a specified colour count
{
	if(colour == "Light Blue")
	{
		lightBlues++;	// increment lightBlues
		if(lightBlues == 3)	// if they own all of the proeprties in the colour group
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[6].doubleRent();	// double the rent on the properties ion the group
	  		board[8].doubleRent();
		 	board[9].doubleRent();
		}
	}
	// repeat for other colours
	else if(colour == "Brown")
	{
		browns++;
		if(browns == 2)
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[1].doubleRent();
	  		board[3].doubleRent();
		}
	}
	
	else if(colour == "Purple")
	{
		purples++;
		if(purples == 3)
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[11].doubleRent();
	  		board[13].doubleRent();
		 	board[14].doubleRent();
		}
	}
	
	else if(colour == "Orange")
	{
		oranges++;
		if(oranges == 3)
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[16].doubleRent();
	  		board[18].doubleRent();
		 	board[19].doubleRent();
		}
	}
	
	else if(colour == "Red")
	{
		reds++;
		if(reds == 3)
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[21].doubleRent();
	  		board[23].doubleRent();
		 	board[24].doubleRent();
		}
	}
	
	else if(colour == "Yellow")
	{
		yellows++;
		if(yellows == 3)		
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[26].doubleRent();
	  		board[27].doubleRent();
		 	board[29].doubleRent();
		}
	}
	
	else if(colour == "Green")
	{
		greens++;
		if(greens == 3)
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[31].doubleRent();
	  		board[32].doubleRent();
		 	board[34].doubleRent();
		}
	}
	
	else if(colour == "Blue")
	{
		blues++;
		if(blues == 2)
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			board[37].doubleRent();
	  		board[39].doubleRent();
		}
	}
	
	else if(colour == "Station")	// if the colour is station
	{
		stations++;	// increment the stations
		if(stations == 2)		// if the stations is 2
		{
			for(int i = 0; i < 40; i++)
			{
				if(board[i].getOwner() == players[playerNo].getName() && board[i].getColour() == "Station")
				{
					board[i].setCurrentRent(50); // set the rent on the other stations owned by the player to be 50
				}
			}
		}
		if(stations == 3)		// if they have 3 stations
		{
			for(int i = 0; i < 40; i++)
			{
				if(board[i].getOwner() == players[playerNo].getName() && board[i].getColour() == "Station")
				{
					board[i].setCurrentRent(100);		// set the rent on other stations owned by the player to be 100
				}
			}
		}
		if(stations == 4)	// if they have four stations
		{
			cout << "You completed the " << colour << " colour group!" << endl;
			for(int i = 0; i < 40; i++)
			{
				if(board[i].getOwner() == players[playerNo].getName() && board[i].getColour() == "Station")
				{
					board[i].setCurrentRent(200);	// set the stations to have a rent of 200
				}
			}
		}
	}
	
	else if(colour == "Utility")
	{
		utilities++;
	}
}

void player::removeFromColourCount(string colour, player* players, int playerNo, gameTile* board)	// remove from specified property count
{
	if(colour == "Light Blue")	// if the colour is light blue
	{
		lightBlues--;	// decrement the light blues count
		if(lightBlues == 2)	// if the count is now 2 (ie 1 less than the amount of properties in the set)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[6].halveRent();	// halve the rent on other proeprties in the set
	  		board[8].halveRent();
		 	board[9].halveRent();
		}
	}
	// repeat for other colours
	else if(colour == "Brown")
	{
		browns--;
		if(browns == 1)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[1].halveRent();
	  		board[3].halveRent();
		}
	}
	
	else if(colour == "Purple")
	{
		purples--;
		if(purples == 2)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[11].halveRent();
	  		board[13].halveRent();
		 	board[14].halveRent();
		}
	}
	
	else if(colour == "Orange")
	{
		oranges--;
		if(oranges == 2)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[16].halveRent();
	  		board[18].halveRent();
		 	board[19].halveRent();
		}
	}
	
	else if(colour == "Red")
	{
		reds--;
		if(reds == 2)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[21].halveRent();
	  		board[23].halveRent();
		 	board[24].halveRent();
		}
	}
	
	else if(colour == "Yellow")
	{
		yellows--;
		if(yellows == 2)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[26].halveRent();
	  		board[28].halveRent();
		 	board[29].halveRent();
		}
	}
	
	else if(colour == "Green")
	{
		greens--;
		if(greens == 2)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[31].halveRent();
	  		board[32].halveRent();
		 	board[34].halveRent();
		}
	}
	
	else if(colour == "Blue")
	{
		blues--;
		if(blues == 1)
		{
			cout << "You no longer own all of the " << colour << " colour group!" << endl;
			board[37].halveRent();
	  		board[39].halveRent();
		}
	}
	
	else if(colour == "Station")	// if it is a station
	{
		stations--;		// decrement the stations count
		if(stations == 1)	// if the count is now 1
		{
			for(int i = 0; i < 40; i++)
			{
				if(board[i].getOwner() == players[playerNo].getName() && board[i].getColour() == "Station")
				{
					board[i].setCurrentRent(25);	// set the rent of stations owned to 25
				}
			}
		}
		if(stations == 2)	// if the count is now 2
		{
			for(int i = 0; i < 40; i++)
			{
				if(board[i].getOwner() == players[playerNo].getName() && board[i].getColour() == "Station")
				{
					board[i].setCurrentRent(50);	// set the rent on stations owned to 50
				}
			}
		}
		if(stations == 3)	// if the count is now 3
		{
			cout << "You no longer own the " << colour << " colour group!" << endl;
			for(int i = 0; i < 40; i++)
			{
				if(board[i].getOwner() == players[playerNo].getName() && board[i].getColour() == "Station")
				{
					board[i].setCurrentRent(100);	// set the rent on other stations owned to 100
				}
			}
		}
	}
	
	else if(colour == "Utility")
	{
		utilities--;

	}
}
int player::getColourCount(string colour)	// return the specified colour count
{
	if(colour == "Light Blue")
	{
		return lightBlues;
	}
	
	else if(colour == "Brown")
	{
		return browns;
	}
	
	else if(colour == "Purple")
	{
		return purples;
	}
	
	else if(colour == "Orange")
	{
		return oranges;
	}
	
	else if(colour == "Red")
	{
		return reds;
	}
	
	else if(colour == "Yellow")
	{
		return yellows;
	}
	
	else if(colour == "Green")
	{
		return greens;
	}
	
	else if(colour == "Blue")
	{
		return blues;
	}
	
	else if(colour == "Station")
	{
		return stations;
	}
	
	else if(colour == "Utility")
	{
		return utilities;
	}
	return 0;
}

string player::getName()	// return the players name
{
	return name;
}

int player::getWallet()		// return the wallet of the player
{
	return wallet;
}

int player::getProperties(int propertyNo)	// return the property at the specified position in the properties owned array
{
	return propertiesOwned[propertyNo];
}

int player::getPosition()	// return the players position
{
	return currentPosition;
}

int player::getDoublesCount()	// return the amount of doubles thrown by the player in succession
{
	return doublesThrown;
}

bool player::getBankruptStatus()	// return the bankrupt status of the player
{
	return isBankrupt;
}

bool player::isInJail()		// return the players jail status
{
	return inJail;
}

void player::incrementTurnsInJail()		// increment the amount of turns the player has spent in jail
{
	turnsInJail++;
}

int player::getTurnsInJail()	// return the amount of turns the player has spent in jail
{
	return turnsInJail;
}

int player::getJailCards()		// return the amount of jail cards owned
{
	return jailCards;
}

bool player::hasJailFreeCards()	//return whether the player has jail cards
{
	return hasJailCards;
}
 
bool player::hasForfeited()		// return whether the player has forfeited or not
{
	return forfeitedAuction;
}

bool player::hasPassed()		// return whether the player has passed or not
{
	return passedAuction;
}

int player::getTotalWorth()		// return the players total worth
{
	return totalWorth;
} 

void player::setName(string playerName)	//set the name of the player
{
	name = playerName;
}

void player::setPosition(int newPosition)	// set the position of the player
{
	currentPosition = newPosition;
}

void player::incrementDoublesCount()	// increment the doubles thrown 
{
	doublesThrown += 1;
}

void player::resetDoubles()	// reset the amount of doubles thrown
{
	doublesThrown = 0;
}

void player::declareBankrupt(gameTile* board, bool bankOwed)	// declare the player bankrupt
{
	int position;
	isBankrupt = true;
	
	if(bankOwed == true)	// if the bank was owed instead of a player
	{
		for(int i = 0; i < 28; i++)
		{
			position = propertiesOwned[i];
			propertiesOwned[i] = 99;
			
			if(position != 99)
			{
				board[position].setOwner("NULL");	// set the owner of the property to be NULL
			}
		}
	}	
}

void player::setInJail(bool jail)	// set the player to be in jail or not
{
	if (jail == true)		// if they are being put in jail
	{
		currentPosition = 10;	// set the position of the player to be 10
	}
	else if(jail == false)	// if they are being taken out of jail
	{
		turnsInJail = 0;	// set the turns in jail to 0	
	}	
	inJail = jail;
}

void player::incrementJailCards()		//increment the jail cards
{
	jailCards++;
}

void player::setHasJailCards(bool jailCards)	// set the player to have jail cards or not
{
	hasJailCards = jailCards;
}

void player::decrementJailCards()	// decrement the players jail cards
{
	jailCards--;
}

void player::forfeit(bool forfeit)	// forfeit the auction or not
{
	forfeitedAuction = forfeit;
}

void player::passed(bool pass)	// pass the auction turn or not
{
	passedAuction = pass;
}

void player::subtractFromWallet(int amount)	// subtract from the player's wallet
{
	wallet -= amount;
}

void player::subtractFromTotalWorth(int amount)	// subtract from the player's total worth
{
	totalWorth -= amount;
}

void player::addToWallet(int amount)	// add to the players wallet
{
	wallet += amount;
}

void player::addToTotalWorth(int amount)	// add to the total worth of the player
{
	totalWorth += amount;
}

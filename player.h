//player.h
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "gameTile.h"
#ifndef PLAYER_H
#define PLAYER_H 1

using namespace std;

class player	// player class
{
	public:		// public variables
		player();	// default constructor
		
		int browns;		// browns owned
		int lightBlues;	// light blues owned
		int purples;	// purples owned
		int oranges;	// ornages owned
		int reds;		// reds owned
		int yellows;	// yellows owned
		int greens;		// greens owned
		int blues;		// blues owned
		int stations;	// stations owned
		int utilities;	// utilities owned	
		
		string getName();	// get the name of the player	
		int getWallet();	// get the player's wallet
		int getProperties(int propertyNo);	// get the properties owned by the player
		int getPosition();		// get the position of the player
		int getDoublesCount();	// get the doubles thrown in a row by the player
		bool getBankruptStatus();	// get the player's bankrupt status
		bool isInJail();			// check if the player is in jail
		int getTurnsInJail();		// get how many turns the player has spent in jail
		bool hasJailFreeCards();	// check whether the player has jail  free cards
		int getJailCards();			// get the amount of jail cards the player has
		bool hasForfeited();		// check if the player has forfeited
		bool hasPassed();			// check if the player has passed
		int getTotalWorth();		// get the player's total worth
		int getColourCount(string colour);	// get the player's colour count for the specified colour
		int getPropertyCount();				// get the property count of the player
		
		void setName(string playerName);	// set the players name
		void setPosition(int currentPos);	// set the player's position	
		void declareBankrupt(gameTile* board, bool BankOwed);	// declare the player bankrupt
		void setInJail(bool jail);			// set the player to be in jail or not
		void setHasJailCards(bool jailCards);	// set the amount of jail cards the player has
		void updateProperties(gameTile* board);// update the properties array owned by the player
		
		void addToWallet(int amount);		// add to the players wallet
		void addToTotalWorth(int amount);		// add to the players total worth
		void subtractFromWallet(int amount);	// subtract from the players wallet and total worth
		void subtractFromTotalWorth(int amount);
		
		void passed(bool pass);			// set the player to have passed or not
		void forfeit(bool forfeit);		// set the player to have forfeited or not
		void resetDoubles();			// reset the doubles the player has thrown
		
		void incrementDoublesCount();	// increment the amount of doubles thrown
		void incrementTurnsInJail();	// increment the amount of turns the player has had in jail
		void incrementJailCards();		// increment the amount of jail cards they have
		void decrementJailCards();		// decrement the amount of jail cards they have
		void addToColourCount(string colour, player* players, int playerNo, gameTile* board);	// add to the colour count of the specified colour
		void removeFromColourCount(string colour, player* players, int playerNo, gameTile* board);	// remove from the colour count of the specified colour
				
				
	private:				// private variables
		string name;		// store the players name
		int wallet;			// store the players wallet
		int currentPosition;	// store the players position
		int doublesThrown;		// store the doubles thrown by the player
		bool isBankrupt;		// store the players bankrupt status
		bool inJail;			// store the players jail status
		int turnsInJail;		// store the amount of turns the player has had in jail
		int jailCards;			// store the amount of jail cards the player has
		bool hasJailCards;		// store whether or not the player has jail cards
		bool forfeitedAuction;	// store whether the player has forfeited in the auction or not
		int totalWorth;			// store the players total worth
		bool passedAuction;		// store whether the player has passed in the auction or not
		int propertiesOwned[28];	// store the properties owned by the player
		int propertyCount;			// store the amount of properties owned by the player			
};
#endif

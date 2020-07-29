//player.h
#include <string>
#include <cstdlib>
#include <iostream>
#ifndef PLAYER_H
#define PLAYER_H 1

using namespace std;

class player
{
	public:
		player();
		
		string propertiesOwned[28];
		
		string getName();
		int getWallet();
		string getProperties(int propertyNo);
		int getPosition();
		int getDoublesCount();
		bool getBankruptStatus();
		bool hasManagedAlready();
		
		void setName(string playerName);
		void setWallet(int amount);
		void setProperties(int propertyNo, string newProperty);
		void setPosition(int currentPos);
		void incrementDoublesCount();	
		void setBankruptStatus(bool bankrupt);
		void setManaged(bool managed);
				
	private:
		string name;
		int wallet;
		int currentPosition;
		int doublesThrown;
		bool isBankrupt;
		bool hasManaged;
		
};

#endif




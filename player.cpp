//player.cpp
#include <string>
#include <cstdlib>
#include <iostream>
#include "player.h"

using namespace std;

player::player()
{
	
}

string player::getName()
{
	return name;
}

int player::getWallet()
{
	return wallet;
}

string player::getProperties(int propertyNo)
{
	return propertiesOwned[propertyNo];
}

int player::getPosition()
{
	return currentPosition;
}

int player::getDoublesCount()
{
	return doublesThrown;
}

bool player::getBankruptStatus()
{
	return isBankrupt;
}

bool player::hasManagedAlready()
{
	return hasManaged;
}

void player::setName(string playerName)
{
	name = playerName;
}

void player::setWallet(int amount)
{
	wallet = amount;
}

void player::setProperties(int propertyNo, string newProperty)
{
	propertiesOwned[propertyNo] = newProperty;
}

void player::setPosition(int newPosition)
{
	currentPosition = newPosition;
}

void player::incrementDoublesCount()
{
	doublesThrown++;
}

void player::setBankruptStatus(bool bankrupt)
{
	isBankrupt = bankrupt;
}

void player::setManaged(bool managed)
{
	hasManaged = managed;
}


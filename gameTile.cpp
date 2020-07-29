//gameTile.cpp
#include <string>
#include <cstdlib>
#include <iostream>
#include "gameTile.h"

using namespace std;

gameTile::gameTile()	//default constructor
{
	housesOnSet = false;	// sets the houses on set to be false
	houses = 0;	// set houses to be 0
}

void gameTile::doubleRent()	// double the rent on the property
{
	currentRent *= 2;
}

void gameTile::halveRent()	// halve the rent on the property
{
	currentRent /= 2;
}
string gameTile::getName()	// return the name of the property
{
	return name;
}

int gameTile::getPrice()	// return the price of the property
{
	return price;
}

int gameTile::getBaseRent()	// return the rent of the property without houses
{
	return baseRent;
}

string gameTile::getOwner()	// return the owner of the property
{
	return owner;
}

string gameTile::getColour()	// return the colour of the property
{
	return colour;
}

int gameTile::getHouses()		// return the amount of houses on the property
{
	return houses;
}

int gameTile::getHouseCost()	// return the cost of a house
{
    return houseCost;   
}

int gameTile::getMortgageValue()	// return the mortgage value of the property
{
    return mortgageValue;   
}

int gameTile::getUnmortgageValue()	// return the cost to unmortgage a property
{
    return unmortgageValue;   
}

int gameTile::getRentForHouseCount(int houseCount)	// get the rent for the specified house count
{
	if(houseCount == 1)	// return the rent with one house
	{
		return R1House;
	}
	
	else if(houseCount == 2)	// return the rent with 2 houses
	{
		return R2House;
	}
	
	else if(houseCount == 3)	// return the rent with 3 houses
	{
		return R3House;
	}
	
	else if(houseCount == 4)	// return the rent with 4 houses
	{
		return R4House;
	}
	
	else if(houseCount == 5)	// return the rent with 5 houses ( a hotel )
	{
		return RHotel;
	}
	
	else if(houseCount == 0)	// return the base rent
	{
		return baseRent;
	}
	return 0;
}

int gameTile::getCurrentRent()	// return the current rent of the property
{
	return currentRent;
}

bool gameTile::isMortgaged()	// return whether the property is mortgaged or not
{
	return mortgaged;
}

bool gameTile::hasHousesOnSet(string colour, gameTile* board)	// check whether the set has houses on any property in it
{
	if(colour == "Light Blue")	// if the colour is light blue
	{
		if(board[6].getHouses() > 0 || board[8].getHouses() > 0 || board[9].getHouses() > 0) // check the light blue properties for houses
		{
			return true;	// if there are any houses on any of the properties, return true
		}
		else	// otherwise return false
		{
			return false;
		}
	}
	
	// repeat for other colours
	
	else if(colour == "Brown")
	{
		if(board[1].getHouses() > 0 || board[3].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Purple")
	{
		if(board[11].getHouses() > 0 || board[13].getHouses() > 0 || board[14].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Orange")
	{
		if(board[16].getHouses() > 0 || board[18].getHouses() > 0 || board[19].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Red")
	{
		if(board[21].getHouses() > 0 || board[23].getHouses() > 0 || board[24].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Yellow")
	{
		if(board[26].getHouses() > 0 || board[27].getHouses() > 0 || board[29].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Green")
	{
		if(board[31].getHouses() > 0 || board[32].getHouses() > 0 || board[34].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Blue")
	{
		if(board[37].getHouses() > 0 || board[39].getHouses() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else
	{
		return false;
	}
}
void gameTile::setName(string PropertyName)	// set the name of the property
{
	name = PropertyName;
}

void gameTile::setPrice(int PropertyPrice)	// set the price of the property
{
	price = PropertyPrice;
}

void gameTile::setBaseRent(int NewRent)	// set the baseRent of the proeprty
{
	baseRent = NewRent;
}

void gameTile::setOwner(string PropertyOwner)	// set the owner of the property
{
	owner = PropertyOwner;
}

void gameTile::setColour(string PropertyColour)	// set the colour of the property
{
	colour = PropertyColour;
}

void gameTile::setHouses(int HouseNo)	// set the houses on the property
{
	houses = HouseNo;
}

void gameTile::setHouseCost(int HouseCost)	// set the cost of a house
{
    houseCost = HouseCost;     
}

void gameTile::setMortgageValue(int PropertyMortgageValue)	// set the mortgage value of the property
{
    mortgageValue = PropertyMortgageValue;
}

void gameTile::setUnmortgageValue(int PropertyUnmortgageValue)   // set the cost to unmortgage a property
{
    unmortgageValue = PropertyUnmortgageValue;     
} 

void gameTile::setR1House(int PropertyR1House)	// set the rent for one house
{
    R1House = PropertyR1House;
}

void gameTile::setR2House(int PropertyR2House)	// set the rent for 2 houses
{
    R2House = PropertyR2House;
}

void gameTile::setR3House(int PropertyR3House)	// set the rent for 3 houses
{
    R3House = PropertyR3House;
}

void gameTile::setR4House(int PropertyR4House)	// set the rent for 4 houses
{
    R4House = PropertyR4House;
}

void gameTile::setRHotel(int PropertyRHotel)	// set the rent for a hotel
{
    RHotel = PropertyRHotel;    
}

void gameTile::setCurrentRent(int newRent)	// set the current rent of the property
{
 	currentRent = newRent;     
}

void gameTile::setMortgaged(bool mortgage)	// set the property to be mortgaged or not
{
	mortgaged = mortgage;
}

bool gameTile::hasMortgagedProperty(string colour, gameTile* board)	// check if the set has any mortgaged proeprties in it
{
	if(colour == "Light Blue")	// if the colour is light blue
	{
		if(board[6].isMortgaged() == true || board[8].isMortgaged() == true || board[9].isMortgaged() == true)	// check if any properties in the set are mortgaged
		{
			return true;	// if any are mortgaged, return true
		}
		else	// otherwise return false
		{
			return false;
		}
	}
	//repeat for other colours
	else if(colour == "Brown")
	{
		if(board[1].isMortgaged() == true || board[3].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Purple")
	{
		if(board[11].isMortgaged() == true || board[13].isMortgaged() == true || board[14].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Orange")
	{
		if(board[16].isMortgaged() == true || board[18].isMortgaged() == true || board[19].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Red")
	{
		if(board[21].isMortgaged() == true || board[23].isMortgaged() == true || board[24].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Yellow")
	{
		if(board[26].isMortgaged() == true || board[27].isMortgaged() == true || board[29].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Green")
	{
		if(board[31].isMortgaged() == true || board[32].isMortgaged() == true || board[34].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else if(colour == "Blue")
	{
		if(board[37].isMortgaged() == true || board[39].isMortgaged() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	else
	{
		return false;
	}
}


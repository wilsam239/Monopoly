//gameTile.h
#include <string>
#include <cstdlib>
#include <iostream>
#ifndef GAMETILE_H
#define GAMETILE_H 1

using namespace std;

class gameTile
{
	public:					// public variables 
		gameTile();			// default constructor
		
		string getName();	// get the name of the property
		int getPrice();		// get the price of the property
		int getBaseRent();	// get the base rent of the property
		string getOwner();	// get the owner of the property
		string getColour();	// get the colour of the property
		int getHouses();	// get the houses on the property
		int getHouseCost();	// get the price of a house on the property
		int getMortgageValue();		// get the mortgage value of the property
		int getUnmortgageValue();	// get the price to unmortgage the property
		int getRentForHouseCount(int houseCount);	// get the rent for the amount of houses on the property
		int getCurrentRent();		// get the rent on the property
		bool isMortgaged();			// get the proeprty's mortgage status
		bool hasHousesOnSet(string colour, gameTile* board);		// get the amount of houses on the set
		bool hasMortgagedProperty(string colour, gameTile* board);	// get whether the set has any mortgaged properties 
		
		void doubleRent();		// double the rent on the property
		void halveRent();		// halve the rent on the property
		void setName(string PropertyName);		// set the name of the property
		void setPrice(int PropertyPrice);		// set the price of the property
		void setBaseRent(int NewRent);			// set the base rent of the property
		void setOwner(string PropertyOwner);	// set the owner of the property
		void setColour(string PropertyColour);	// set the colour of the proeprty
		void setHouses(int HouseNo);			// set the amount of houses on the property
		void setHouseCost(int HouseCost);		// set the cost of the houses on the property
		void setMortgageValue(int PropertyMortgageValue);		// set the mortgage value of the property
		void setUnmortgageValue(int PropertyUnmortgageValue);	// set the price to unmortgage the property
		void setR1House(int PropertyR1House);	// set the rent for one house on the property
		void setR2House(int PropertyR2House);	// set the rent for two houses on the property
		void setR3House(int PropertyR3House);	// set the rent for three houses on the property
		void setR4House(int PropertyR4House);	// set the rent for four houses on the property
		void setRHotel(int PropertyRHotel);		// set the rent for a hotel on the property	
		void setCurrentRent(int newRent);		// set the current rent on the property
		void setMortgaged(bool mortgage);		// set the property to be mortgaged or not
		
	private:			//private variables
		string name;	// store the name of the property
		int price;		// store the price of the property
		int baseRent;	// store the base rent of the property
		string owner;	// store the owner of the property
		string colour;	// store the colour of the property
		int houses;		// store the amount of houses on the property
		int houseCost;	// stores the cost of a house
		int mortgageValue;	// stores the mortgage value of a property
		int unmortgageValue;// stores the cost to unmortgage a property
        int R1House;	// store the rent for one house
        int R2House;	// store the rent for two houses
        int R3House;	// store the rent for three houses
        int R4House;	// store the rent for four houses
		int RHotel;		// store the rent for a hotel
		int currentRent;// store the rent for the property
		bool mortgaged;	// store whether the property is mortgaged or not
		bool housesOnSet;	// store whether or not the set has houses on it
};
#endif

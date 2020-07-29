//square.h
#include <string>
#include <cstdlib>
#include <iostream>
#ifndef SQUARE_H
#define SQUARE_H 1

using namespace std;

class square
{
	public:
		square();
		
		string getName();
		int getPrice();
		int getBaseRent();
		string getOwner();
		string getColour();
		int getHouses();
		int getHouseCost();
		int getMortgageValue();
		int getUnmortgageValue();
		int getR1House();
		int getR2House();
		int getR3House();
		int getR4House();
		int getRHotel();
		int getCurrentRent();
		bool isMortgaged();
		
		void setName(string PropertyName);
		void setPrice(int PropertyPrice);
		void setBaseRent(int NewRent);
		void setOwner(string PropertyOwner);
		void setColour(string PropertyColour);
		void setHouses(int HouseNo);
		void setHouseCost(int HouseCost);
		void setMortgageValue(int PropertyMortgageValue);
		void setUnmortgageValue(int PropertyUnmortgageValue);
		void setR1House(int PropertyR1House);
		void setR2House(int PropertyR2House);
		void setR3House(int PropertyR3House);
		void setR4House(int PropertyR4House);
		void setRHotel(int PropertyRHotel);
		void setCurrentRent(int newRent);
		void setMortgaged(bool mortgage);
		
	private:
		string name;
		int price;
		int baseRent;
		string owner;
		string colour;
		int houses;
		int houseCost;
		int mortgageValue;
		int unmortgageValue;
        int R1House;
        int R2House;
        int R3House;
        int R4House;		
		int RHotel;
		int currentRent;
		bool mortgaged;
};

#endif




//square.cpp
#include <string>
#include <cstdlib>
#include <iostream>
#include "square.h"

using namespace std;

square::square()
{
	
}

string square::getName()
{
	return name;
}

int square::getPrice()
{
	return price;
}

int square::getBaseRent()
{
	return baseRent;
}

string square::getOwner()
{
	return owner;
}

string square::getColour()
{
	return colour;
}

int square::getHouses()
{
	return houses;
}

int square::getHouseCost()
{
    return houseCost;   
}

int square::getMortgageValue()
{
    return mortgageValue;   
}

int square::getUnmortgageValue()
{
    return unmortgageValue;   
}

int square::getR1House()
{
    return R1House;
}

int square::getR2House()
{
    return R2House;
}

int square::getR3House()
{
    return R3House;
}

int square::getR4House()
{
    return R4House;
}

int square::getRHotel()
{
    return RHotel;
}

int square::getCurrentRent()
{
	return currentRent;
}

bool square::isMortgaged()
{
	return mortgaged;
}

void square::setName(string PropertyName)
{
	name = PropertyName;
}

void square::setPrice(int PropertyPrice)
{
	price = PropertyPrice;
}

void square::setBaseRent(int NewRent)
{
	baseRent = NewRent;
}

void square::setOwner(string PropertyOwner)
{
	owner = PropertyOwner;
}

void square::setColour(string PropertyColour)
{
	colour = PropertyColour;
}

void square::setHouses(int HouseNo)
{
	houses = HouseNo;
}

void square::setHouseCost(int HouseCost)
{
    houseCost = HouseCost;     
}

void square::setMortgageValue(int PropertyMortgageValue)
{
    mortgageValue = PropertyMortgageValue;
}

void square::setUnmortgageValue(int PropertyUnmortgageValue)   
{
    unmortgageValue = PropertyUnmortgageValue;     
} 

void square::setR1House(int PropertyR1House)
{
    R1House = PropertyR1House;
}

void square::setR2House(int PropertyR2House)
{
    R2House = PropertyR2House;
}

void square::setR3House(int PropertyR3House)
{
    R3House = PropertyR3House;
}

void square::setR4House(int PropertyR4House)
{
    R4House = PropertyR4House;
}

void square::setRHotel(int PropertyRHotel)
{
    RHotel = PropertyRHotel;    
}

void square::setCurrentRent(int newRent)
{
 	currentRent = newRent;     
}

void square::setMortgaged(bool mortgage)
{
	mortgaged = mortgage;
}


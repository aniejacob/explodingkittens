/*
Author: Aniebiet Jacob
Date: 3/09/2017
Description:
This file contains the Card program for project 2.
This program creates a card object for every card in the deck
*/


#include "Card.h"
#include <iostream>
using namespace std;


Card::Card()
{
  // no code, default constructor
}

/*
  Card()
  Constructor that creates a card using the integer type of card and the card
  description
*/
Card::Card(int type, string desc)
{
  //Safetly changes card type
  m_type = type;

  //Safely changes card description
  m_desc = desc;
}

/*
  GetType()
  returns the integer type of card the card is
 */
int Card::GetType()
{
  return m_type;
}

/*
  ToString()
  returns the string description of card
*/
string Card::ToString()
{
  return m_desc;
}

/*
Author: Aniebiet Jacob
Date: 3/09/2017

Description:
This file contains the Player program for project 2.
This program creates a players object to be able to play the game.
These players will have a name, a hand of cards, and will either have lost or not lost.
*/


#include "Player.h"
#include "Card.h"
#include <iostream>
#include <string>
using namespace std;


/*
  Player()
  desc - default constructor
*/
Player::Player(){
  //no code
}



/*
  Player()
  desc - creates a player through a plaer name and whether or not they have lost
*/
Player::Player(string name){

  m_name = name;
  m_lost = false;

}


/*
  PlayCard()
  desc - ask player what card the want to play and then takes the card from
  the player's hand
*/
Card Player::PlayCard(){

  int theCard = 0;

  //Shows what cards are in the player's hand
  DisplayHand();

  cout << "Please choose a card: " << endl;
  cin >> theCard;

  //While the card the user chooses doesn't exist in user's hand
  //Repromts the user
  while (theCard < 0 or theCard > (int)m_hand.size()-1){
    cout << " " << endl
	 << "Invalid entry" << endl
	 << " " << endl;
    DisplayHand();
    cout << "Please choose a card: " << endl;
    cin >> theCard;
  }

  // Takes selected card and stores it in a temporary new card to return
  Card newCard = m_hand.at(theCard);

  // Removes played card from players hand
  m_hand.erase(m_hand.begin()+theCard);
  return newCard;

}




/*
  LoseCard()
  desc - takes a card out a players hand
*/
Card Player::LoseCard(int card){

  // Takes selected card and stores it to a temporary new card to return
  Card newCard = m_hand.at(card);

  // Removes card selected from other players hand
  m_hand.erase(m_hand.begin()+card);

  return newCard;

}


/*
  HasCards()
  desc - checks to see the number of cards that the user has
*/
bool Player::HasCards(){

  // Returns number of cards in the player's hand
  return m_hand.size() > 0;

}



/*
  HasLost()
  desc - tells whether or not the player has lost
*/
bool Player::HasLost(){

  // Tells whtether or not player has lost
  return m_lost;

}


/*
  HasExtraTurn()
  desc - tells whether or not the player has an extra turn
*/
bool Player::HasExtraTurn(){

  // if player hasn't lost tells if the player has an extra turn
  if (m_lost == false)
    return m_extraTurn;

  // if player has lost doesn't give player extra turn
  else
    return m_extraTurn = false;
}



/*
  AddToHand()
  desc - puts a drawn card into the player's hand
  tells whether or not the card is a bomb and if the player has a defuse card for that bomb
*/
int Player::AddToHand(Card theCard){

  //Adds the card to the player's hand
  m_hand.push_back(theCard);

  // If added card is a bomb checks if player has a defuse card
  if (theCard.GetType() == 0){
    for (int i = 0; i < (int)m_hand.size(); i++){
      // if player has a defuse card tells user that through integer
      if (m_hand.at(i).GetType() == 1)
	return 1;
      // if player doesn't has defuse card tells player through integer
      else
	return 0;
    }
  }
  //if not a bomb card tells player through integer
  return 2;

}



/*
  GetName()
  desc - tells the name of the player
*/
string Player::GetName(){

  //Tells the name of the player
  return m_name;

}



/*
  DisplayHand()
  desc - shows what cards are in a players hand and what those cards do
*/
void Player::DisplayHand(){

  // Sets up what type of card corrilates with what card number - 1
  string typeCard[10];
  typeCard[0] = "Exploding Kitten!: ";
  typeCard[1] = "Defuse: ";
  typeCard[2] = "Attack: ";
  typeCard[3] = "Skip: ";
  typeCard[4] = "See-the-Future: ";
  typeCard[5] = "Shuffle: ";
  typeCard[6] = " ";
  typeCard[7] = " ";
  typeCard[8] = " ";
  typeCard[9] = " ";

  // Goes through a player's hand and reports:
  //index at, card type, and card description
  for (int i= 0; i < (int)m_hand.size(); i++)
    cout << i << " " << typeCard[m_hand.at(i).GetType()]
	 << m_hand.at(i).ToString() << endl;


}



/*
  GetNumberOfCards()
  desc - tells how many cards are in a player's hand
*/
int Player::GetNumberOfCards(){

  //Returns the amount of cards in a players hand
  return m_hand.size();

}




/*
  ReturnBomb()
  desc - takes away card from players
*/
Card Player::ReturnBomb(){

  // Obtains the bomb card from player's hand to return
  Card bombCard = m_hand.at(m_hand.size()-1);
  //Removes the bomb card from the player's hand
  m_hand.erase(m_hand.begin() + m_hand.size()-1);

  return bombCard;
}



/*
  SetLost()
  desc - Sets that the player has lost
 */
void Player::SetLost(bool hasLost){

  //Sets the player's lost status to true
  if (hasLost == true)
    m_lost = true;

}



/*
  SetExtraTurn()
  desc - sets whether or not the player has an extra turn
 */
void Player::SetExtraTurn(bool hasTurn){

  //Set's the player's extra turn status
  m_extraTurn = hasTurn;

}

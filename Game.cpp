/*
Author: Aniebiet Jacob
Date: 3/09/2017
Description:
This file contains the Game program for project 2.
This program runs the entire game utilizing the player class and the card class.
The game will handle the deck and all of the cards during the game and determine the winner of the game.
*/


#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include "Player.h"
#include "Card.h"
#include "Game.h"
using namespace std;




/*
  Game()
  desc - Runs the game
 */
Game::Game(){

  cout << endl << "MEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOW" << endl
       << "Welcome to Exploding Kittens!" << endl
       << "MEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOW" << endl;

  //Asks users for how many people are playing
  SetNumPlayers(m_numPlayers);

  //initializes different player names
  string player1 = "Player 1";
  string player2 = "Player 2";
  string player3 = "Player 3";
  string player4 = "Player 4";

  switch (m_numPlayers)
    {
    // If player chooses 2 players adds 2 players to player array
    case 2:
      m_players[0] = Player(player1);
      m_players[1] = Player(player2);
      break;

    // If player chooses 3 players adds 3 players to player array
    case 3:
      m_players[0] = Player(player1);
      m_players[1] = Player(player2);
      m_players[2] = Player(player3);
      break;

    // If player chooses 4 players adds 4 players to player array
    case 4:
      m_players[0] = Player(player1);
      m_players[1] = Player(player2);
      m_players[2] = Player(player3);
      m_players[3] = Player(player4);
      break;
    }


  //Loads in deck from file
  LoadDeck(DECK_NAME);
  cout << "Shuffling Cards..." << endl;

  //Initially shuffles before dealing out cards
  Shuffle();


  cout << "Dealing Cards..." << endl;

  //Deals out initial number of cards to each player's hand in the game
  for (int i = 0; i < m_numPlayers; i++){
    for(int j = 0; j < INIT_CARDS; j++){
      m_players[i].AddToHand(m_deck.at(j));
      // Deleted deallt out cards from the deck
      m_deck.erase(m_deck.begin()+j);
    }
  }

  // Add's bomb cards into the deck of cards
  AddBombCards(BOMB_NAME);
  cout << "Shuffling in Exploding Kittens!" << endl;
  // Final shuffle of cards with the bombs in
  Shuffle();

  // Runs the rest of the game
  PlayGame();

}



/*
  SetNumPlayers
  desc - through user input sets how many people are playing
 */
void Game::SetNumPlayers(int numPlayers){

  cout << " " << endl
       << "How many players would you like?" << endl
       << "Choose 2 - 4 players: " << endl;

  cin >> numPlayers;

  // While user is enter in invalid input, reprompts user for valid input
  while (numPlayers > 4 or numPlayers < 2){

    cout << "Error, plaese choose a number of players 2 - 4: " << endl;
    cin >> numPlayers;

  }
  //Sets up the number of players
  m_numPlayers = numPlayers;

}


/*
  Shuffle()
  desc - Randomly shuffles cards through the entire deck
*/
void Game::Shuffle(){

  //Creates number of times deck will be shuffled
  int shuffleNum = 10;
  //Initializes random shuffling by the random seed
  srand(RAND_SEED);
  int lengthDeck = m_deck.size();

  //Shuffles deck for however many times the shuffle number is set to
  for (int i = 0; i < shuffleNum; i++){
    //Shuffles throughout the entire deck
    for (int j = 0; j < (lengthDeck); j++){
      //Switches the places of two cards in the deck in order to shuffle the deck
      Card orgCard = m_deck.at(j);
      int newPlace = rand()%10;
      m_deck.at(j) = m_deck.at(newPlace);
      m_deck.at(newPlace) = orgCard;

    }
   }
}




/*
  Peek()
  desc - loops and looks through the first PEEK_NUM cards in the deck
*/
void Game::Peek(){

  //Prints out the card descriptions of the first 3 cards in the deck
  for (int i = 0; i < (int)PEEK_NUM; i++)
    cout << m_deck.at(i).ToString() << endl;

}




/*
  DrawCard()
  desc - Allows a player to remove a card from the deck
*/
Card Game::DrawCard(){

  // Picks card from deck that they want to delete in order to return
  Card deletedCard =  m_deck.at(m_deck.size()-1);
  // Removes card user draws drom the deck
  m_deck.erase(m_deck.begin()+(m_deck.size()-1));
  return deletedCard;
}




/*
  HandleCard()
  desc - depending on the .GetType() of card, handles the card in a certain manner
*/
int Game::HandleCard(int player){

  //Initializes random just incase player decides to attack
  srand(RAND_SEED);
  Card currentCard = m_players[player].PlayCard();
  //Initializes the type of card to use in the switch statment
  int theType = currentCard.GetType();
  //Initializes random in case player decides to steal a card
  int randNum =  rand()%(m_numPlayers-1);
  //Initializes a boolean just in case player decides to attack
  bool extra = true;


  m_discard.push_back(currentCard);

  //Uses the players selection of card to see how to handle card
  switch (theType)
    {


     // Error if users decides to use a defuse card,
    case 1:
      cout << "You can't play your defuse card now! Save it for if you encounter an Exploding Kitten!" << endl;
      m_players[player].AddToHand(currentCard);
      return 0;
      break;


     //Attacks another player randomly
    case 2:
      cout << "You attacked " << m_players[randNum].GetName() << "!" << endl;
      //Gives randomly choosen player and extra turn
      m_players[randNum].SetExtraTurn(extra);

      return 3;
      break;


    //Skips the current players turn
    case 3:
      cout << "You Played a Skip Card" << endl
	   << "Now you don't risk drawing an Exploding Kitten!" << endl;
      //Ends this player's turn
      return 3;
      break;


    //Allows player to see unplayed/selected cards in the deck
    case 4:
      cout << " " << endl
	   << "You played a See-the-Future Card!" << endl
	   << "Lets see what cards are coming up next!" << endl;

      Peek();

      return 0;
      break;


    //Shuffles the cards in the deck
    case 5:
      cout << "You played a Shuffle Card!" << endl
	   << "Now Shuffling Cards!" << endl;

      Shuffle();
      //Continues the player's turn
      return 0;
      break;



    //If player plays a pair of two identical cards, allows them to steal card
    //from another player
    case 6:
      //Asks player to play a second card
      currentCard = m_players[player].PlayCard();

      //If second card is the same as first, allows player to steal
      if (currentCard.GetType() == 6){

	//initializes a selection of a player
	int selectPlayer = 0;
	//initializes the number of cards a player holds
	int noOfCards = 0;

	cout << "You played a pair!"
	     << "You can now steal a card from another player!" << endl
	     << "Select a Player: " << endl;

	//Goes through the player array
	for (int i = 0; i < m_numPlayers; i++){
	  // if the player in array isn't the current player
	  // prints our player name and index
	  if (i != player)
	    cout << i << " " << m_players[i].GetName() << endl;
	}
	cin >> selectPlayer;

	//Validation
	while (selectPlayer > m_numPlayers-1 or selectPlayer < 0 or selectPlayer == player){
	  cout << "That's not a player! Choose and ACUAL player!" << endl;
	  for (int i = 0; i < m_numPlayers; i++){
	    // if the player in array isn't the current player
	    // prints our player name and index
	    if (i != player)
	      cout << i << " " << m_players[i].GetName() << endl;
	  }
	  cin >> selectPlayer;
	}

	//Finds out how many cards the selected player is holding
	noOfCards = m_players[selectPlayer].GetNumberOfCards();

	Card newerCard;
	int theCard = 0;

	//For every card selected user has, prints out the card's index
	for (int i = 0; i < noOfCards; i++)
	  cout << "Card " << " " << i << endl;

	cout << "Select a card: " << endl;
	cin >> theCard;

	//user validation
	while( theCard > noOfCards-1 or theCard < 0){
	  cout << "That's not one of the cards! select an actual card!" << endl;
	  for (int i = 0; i < noOfCards; i++)
	    cout << "Card " << " " << i << endl;

	  cout << "Select a card: " << endl;
	  cin >> theCard;
	}

	//Takes away the selected card from the selected users
	newerCard = m_players[selectPlayer].LoseCard(theCard);
	m_players[player].AddToHand(newerCard);
	//Tells players who just lost a card
	cout << m_players[selectPlayer].GetName() << " lost a card!" << endl;
      }

      //If card's don't match, asks the user to play a new card
      else{
	cout << "These cards don't match! Please try again!" << endl;
	m_players[player].AddToHand(currentCard);
	m_players[player].AddToHand(m_discard.at((int)m_discard.size()-1));
      }

      break;

    //If player plays a pair of two identical cards, allows them to steal card
    //from another player
    case 7:
      //Asks user to play a second card
      currentCard = m_players[player].PlayCard();
      //If second card is the same as first, allows player to steal
      if (currentCard.GetType() == 7){

        //initializes a selection of a player
        int selectPlayer = 0;
        //initializes the number of cards a player holds
        int noOfCards = 0;

        cout << "You played a pair!"
             << "You can now steal a card from another player!" << endl
             << "Select a Player: " << endl;

        //Goes through the player array
        for (int i = 0; i < m_numPlayers; i++){
          // if the player in array isn't the current player
          // prints our player name and index
          if (i != player)
            cout << i << " " << m_players[i].GetName() << endl;
        }
        cin >> selectPlayer;

	//Validation
        while (selectPlayer > m_numPlayers-1 or selectPlayer < 0 or selectPlayer == player){
          cout << "That's not a player! Choose and ACUAL player!" << endl;
          for (int i = 0; i < m_numPlayers; i++){
            // if the player in array isn't the current player
            // prints our player name and index
            if (i != player)
              cout << i << " " << m_players[i].GetName() << endl;
          }
          cin >> selectPlayer;
        }

        //Finds out how many cards the selected player is holding
        noOfCards = m_players[selectPlayer].GetNumberOfCards();

        Card newerCard;
        int theCard = 0;

        //For every card selected user has, prints out the card's index
        for (int i = 0; i < noOfCards; i++)
          cout << "Card " << " " << i << endl;

        cout << "Select a card: " << endl;
        cin >> theCard;

	//user validation
	while( theCard > noOfCards-1 or theCard < 0){
          cout << "That's not one of the cards! select an actual card!" << endl;
          for (int i = 0; i < noOfCards; i++)
            cout << "Card " << " " << i << endl;

          cout << "Select a card: " << endl;
          cin >> theCard;
        }

        //Takes away the selected card from the selected users
        newerCard = m_players[selectPlayer].LoseCard(theCard);
	m_players[player].AddToHand(newerCard);

	//Tells players who just lost a card
        cout << m_players[selectPlayer].GetName() << " lost a card!" << endl;
      }
      //If card's don't match, asks the user to play a new card
      else{
        cout << "These cards don't match! Please try again!" << endl;
      m_players[player].AddToHand(currentCard);
      m_players[player].AddToHand(m_discard.at((int)m_discard.size()-1));
      }
      break;


    //If player plays a pair of two identical cards, allows them to steal card
    //from another player
    case 8:
      //Asks user to play a second card
      currentCard = m_players[player].PlayCard();
      //If second card is the same as first, allows player to steal
      if (currentCard.GetType() == 8){

        //initializes a selection of a player
        int selectPlayer = 0;
        //initializes the number of cards a player holds
        int noOfCards = 0;

        cout << "You played a pair!"
             << "You can now steal a card from another player!" << endl
             << "Select a Player: " << endl;

        //Goes through the player array
        for (int i = 0; i < m_numPlayers; i++){
          // if the player in array isn't the current player
          // prints our player name and index
          if (i != player)
            cout << i << " " << m_players[i].GetName() << endl;
        }
        cin >> selectPlayer;

	//Validation
        while (selectPlayer > m_numPlayers-1 or selectPlayer < 0 or selectPlayer == player){
          cout << "That's not a player! Choose and ACUAL player!" << endl;
          for (int i = 0; i < m_numPlayers; i++){
            // if the player in array isn't the current player
            // prints our player name and index
            if (i != player)
              cout << i << " " << m_players[i].GetName() << endl;
          }
          cin >> selectPlayer;
        }

        //Finds out how many cards the selected player is holding
        noOfCards = m_players[selectPlayer].GetNumberOfCards();

        Card newerCard;
        int theCard = 0;

        //For every card selected user has, prints out the card's index
        for (int i = 0; i < noOfCards; i++)
          cout << "Card " << " " << i << endl;

        cout << "Select a card: " << endl;
        cin >> theCard;

	//user validation
	while( theCard > noOfCards-1 or theCard < 0){
          cout << "That's not one of the cards! select an actual card!" << endl;
          for (int i = 0; i < noOfCards; i++)
            cout << "Card " << " " << i << endl;

          cout << "Select a card: " << endl;
          cin >> theCard;
        }


        //Takes away the selected card from the selected users
        newerCard = m_players[selectPlayer].LoseCard(theCard);
	m_players[player].AddToHand(newerCard);

	//Tells players who just lost a card
        cout << m_players[selectPlayer].GetName() << " lost a card!" << endl;
      }
      //If card's don't match, asks the user to play a new card
      else{
        cout << "These cards don't match! Please try again!" << endl;
	cout << "These cards don't match! Please try again!" << endl;
	m_players[player].AddToHand(currentCard);
	m_players[player].AddToHand(m_discard.at((int)m_discard.size()-1));
      }

      break;


    //If player plays a pair of two identical cards, allows them to steal card
    //from another player
    case 9:
      //Asks user for second input card
      currentCard = m_players[player].PlayCard();
      //If second card is the same as first, allows player to steal
      if (currentCard.GetType() == 9){

        //initializes a selection of a player
        int selectPlayer = 0;
        //initializes the number of cards a player holds
        int noOfCards = 0;

        cout << "You played a pair!"
             << "You can now steal a card from another player!" << endl
             << "Select a Player: " << endl;

        //Goes through the player array
        for (int i = 0; i < m_numPlayers; i++){
          // if the player in array isn't the current player
          // prints our player name and index
          if (i != player)
            cout << i << " " << m_players[i].GetName() << endl;
        }
        cin >> selectPlayer;

	//Validation
        while (selectPlayer > m_numPlayers-1 or selectPlayer < 0 or selectPlayer == player){
          cout << "That's not a player! Choose and ACUAL player!" << endl;
          for (int i = 0; i < m_numPlayers; i++){
            // if the player in array isn't the current player
            // prints our player name and index
            if (i != player)
              cout << i << " " << m_players[i].GetName() << endl;
          }
          cin >> selectPlayer;
        }

        //Finds out how many cards the selected player is holding
        noOfCards = m_players[selectPlayer].GetNumberOfCards();

        Card newerCard;
        int theCard = 0;

        //For every card selected user has, prints out the card's index
        for (int i = 0; i < noOfCards; i++)
          cout << "Card " << " " << i << endl;

        cout << "Select a card: " << endl;
        cin >> theCard;

	//user validation
	while( theCard > noOfCards-1 or theCard < 0){
          cout << "That's not one of the cards! select an actual card!" << endl;
          for (int i = 0; i < noOfCards; i++)
            cout << "Card " << " " << i << endl;

          cout << "Select a card: " << endl;
          cin >> theCard;
        }


        //Takes away the selected card from the selected users
        newerCard = m_players[selectPlayer].LoseCard(theCard);
	m_players[player].AddToHand(newerCard);

	//Tells players who just lost a card

        cout << m_players[selectPlayer].GetName() << " lost a card!" << endl;
      }
      //If card's don't match, asks the user to play a new card
      else{
        cout << "These cards don't match! Please try again!" << endl;
	cout << "These cards don't match! Please try again!" << endl;
	m_players[player].AddToHand(currentCard);
	m_players[player].AddToHand(m_discard.at((int)m_discard.size()-1));
      }

      break;

    }
  return 0;

}


/*
  PlayGame()
  desc - runs the playing of the game and determines who the winner is
*/
void Game::PlayGame(){

  //Initalizes extra turn to be false
  bool extraT = false;
  for (int i = 0; i < m_numPlayers; i++){
    m_players[i].SetExtraTurn(extraT);
  }
  //Initializes the first player to go
  int thePlayer = 0;
  int losers = 0;
  int userChoice = 0;

  //While more than one player hasn't lost
  while (losers!= m_numPlayers - 1){

    userChoice = 0;
    losers = 0;

    //While the user hasn't lost and hasn't drawn a card
    while (userChoice != 3 and m_players[thePlayer].HasLost() != true){

      //tells how many cards remain in the deck
      cout << "Number of Cards Left: " << m_deck.size() << " " << endl;

      cout << endl
	   << "It's " << m_players[thePlayer].GetName() << "'s turn!" << endl
	   << "What would you like to do?" << endl
	   << "1. View Cards" << endl
	   << "2. Play A Card" << endl
	   << "3. Draw A Card" << endl;
      cin >> userChoice;

      //If the user enters an invalid choice from the menu, reprompts user
      while( (userChoice > 3) or (userChoice < 1) ){
	cout << endl
	     << "Invalid entry, enter a number 1-3" << endl
	     << m_players[thePlayer].GetName() << ", what would you like to do?" << endl
	     << "1. View Cards" << endl
	     << "2. Play A Card" << endl
	     << "3. Draw A Card" << endl;
	cin >> userChoice;
      }

      //Depending on choice in menu, chooses what do do with the player
      switch (userChoice){


      //Lets player see their cards
      case 1:

	m_players[thePlayer].DisplayHand();
	break;


      //Lets player play on of their cards
      case 2:
	if (m_players[thePlayer].GetNumberOfCards() != 0){
	  userChoice = HandleCard(thePlayer);
	}
	else
	  cout << "You have no more cards! Draw a card" << endl;
	break;


      //Let's player draw a card from the deck
      case 3:
	int theCard = m_players[thePlayer].AddToHand(m_deck.at(0));


	//If the card is an exploding kitten and the player doesn't have a defuse
	//Player loses
	if ( theCard == 0){
	  cout << "Oh No! You drew Exploding Kitten: "
	       << m_deck.at(0).ToString() << endl
	       << "Sorry! You're out of the game!" << endl;

	  //sets the player's lost status to true
	  bool loser = true;
	  m_players[thePlayer].SetLost(loser);
	  //removes exploding kitten card from the deck
	  m_deck.erase(m_deck.begin()+0);
	}

	//If the card is an exploding kitten and the player does have a defuse
	//Gives a chance to player to defuse the exploding kitten
	if (theCard == 1){
	  cout << " " << endl
	       <<"Oh No! You drew Exploding Kitten: "
	       << m_deck.at(0).ToString() << endl
	       << "But you have a defuse card! Play it or lose!" << " " << endl;

	  //Ask current player to play another card to defuse bomb
	  Card theDefuser = m_players[thePlayer].PlayCard();

	  //If card choosen is a defuse card defuses exploding kitten
	  if (theDefuser.GetType() == 1){
	    cout << "Phew!" << theDefuser.ToString() << endl
		 << "you defused the Exploding Kitten!" << endl
		 << "You're still in the game!" << endl;


	    m_players[thePlayer].ReturnBomb();
	    Shuffle();
	  }

	  //If card choosen isn't a defuse card player loses
	  else if (theDefuser.GetType() != 1){
	    cout << "Why didn't you play your defuse card you idiot?" << endl
		 << "Now you're out of the game!" << endl;

	    //Sets player's lost status to true
	    bool loser = true;
	    m_players[thePlayer].SetLost(loser);
	    //Removes exploding kitten from deck
	    m_deck.erase(m_deck.begin()+0);

	  }
	}

	//If the card isn't a bomb card the card picked is removed from the deck
	if (theCard == 2)
	  m_deck.erase(m_deck.begin()+0);
      }
    }


    //If the player has lost or doesn't have and extra turn changes player
    if (m_players[thePlayer].HasExtraTurn() == 1 and m_players[thePlayer].HasLost() != true){
      bool turn = false;
      thePlayer = thePlayer;
      m_players[thePlayer].SetExtraTurn(turn);
    }

    //If final player in array isn't reach increments player by 1
    else if (thePlayer != m_numPlayers - 1)
      thePlayer = thePlayer + 1;
    //switches back to player 1
    else
      thePlayer = 0;


    //Checks every player to see how many players have already lost
    for (int i = 0; i < m_numPlayers;i++){
      if (m_players[i].HasLost() == true)
	losers = losers + 1;

    }

  }

  //Checks to see which player has not lost and then prints out the winner
  for (int i = 0; i < m_numPlayers;i++){
    if (m_players[i].HasLost() == false)
      cout << "The winner is " << m_players[i].GetName() << "!" << endl;


  }

}

/*
  LoadDeck()
  desc - reads in the cards from the deck file and creates the deck
*/
int Game::LoadDeck(string adeck){

  string numArray;
  string strArray;
  ifstream myFile;

  //opens text file and converts it to a c-string
  myFile.open(adeck.c_str() ,ios::in);


  //Reads in 2 lines of the file at a time until at the end of the file
  while (getline(myFile, numArray) and getline(myFile, strArray)){

    m_deck.push_back(Card (( atoi(numArray.c_str())), strArray));
  }

  return 0;


}


/*
  AddBombCards()
  desc - reads in the bomb cards and puts them into the deck
*/
int Game::AddBombCards(string bombs){


  string numArray;
  string strArray;
  int numBombs = 0;
  ifstream myFile;

  //opens text file and converts it to a c-string
  myFile.open(bombs.c_str() ,ios::in);

  //Reads in 2 lines of the file at a time until the number of bombs needed is
  //read in
  while (getline(myFile, numArray) and getline(myFile, strArray) and numBombs != m_numPlayers-1){

    m_deck.push_back(Card (( atoi(numArray.c_str())), strArray));

    numBombs = numBombs + 1;
  }


  return 0;

}

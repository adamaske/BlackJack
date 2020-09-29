#include <iostream>
#include <vector>
#include <conio.h>
#include "BlackJack.h"
int cards[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

std::vector<int> playerCards{};
int playerSum = 0;
std::vector<int> dealerCards{};
int dealerSum = 0;

enum GameState{ setup, player, dealer,betting, execute};
GameState state = setup;

int playerCash = 100;
int dealerCash = 100;

int playerBet = 0;
int dealerBet = 0;

int main()
{
    do
    {
        switch (state) 
        {
            case setup:
                Setup();
                break;
            case player:
                Player();
                break;
            case dealer:
                DealerTurn();
                break;
            case betting:

                break;
            case execute:
                Execute();
                
                break;
        }
    } while (true);
}

void Setup() 
{
    //Setup stuff

    //Clear player cards
    playerCards.clear();
    //Clear dealer cards
    dealerCards.clear();

    state = player;
}

void Player() 
{

    //Draw the players cards
    std::cout << "Player cards: ";
    for (int i = 0; i < playerCards.size(); i++)
    {
        std::cout << playerCards[i] << ", ";
    }
    std::cout << "" << std::endl;
    //Check if player wants more cards
    std::cout << "Pick card or stand? " << std::endl;
    //Delcare the new input
    char input = _getch();

    switch (input)
    {
        //Add a new card to the vector of cards
        case 'n':
            playerCards.push_back(PickedCard());
            //Check if its over 21, then loose
            playerSum = CardSum(playerCards);
            if (playerSum > 21) {
                //Lost
                std::cout << "You lost by over 21" << std::endl;
                state = setup;
            }
            break;
        //Stand, exit out of player state, go to dealer
        case 's':
            state = dealer;
            break;
        default:

         break;
    }

}

void DealerTurn() 
{
    //Give the dealer cards
    playerSum = CardSum(playerCards);
    //Draw cards until its == or above players sum
    do 
    {
        //Delcare the new card
        int newDealerCard = PickedCard();

        //Check if ace, then check if its 11 is cloaser to 21 than 1 is
        if (newDealerCard == 1) 
        {
            if ((CardSum(dealerCards) + 1) - 12 > (CardSum(dealerCards) + 11) - 12) {
                //Dont change it
            }
            else 
            {
                //Set it to 11
                newDealerCard = 11;
            }
        }

        dealerCards.push_back(newDealerCard);

        dealerSum = CardSum(dealerCards);
        //Check if dealer lost;
        if (dealerSum > 21) 
        {
            std::cout << "Dealer lost by over 21" << std::endl;
            state = setup;
        }
    } while (dealerSum < playerSum);

    state = execute;
}

void Betting() 
{
    std::cout << "Place your bet: ";
    //Place the inuput in k, then check if its valid
    int k = _getch();

    if (k <= playerCash) {
        //Accept
    }
}

void Execute() {
    //Find who won
    Draw();
    state = setup;

    playerSum = CardSum(playerCards);
    dealerSum = CardSum(dealerCards);

    //Check draw
    if (playerSum == dealerSum) {
        std::cout << "It's a draw!";
    }
    //who is closets to 21, using abs to get the absoulute, if its -3 goees to 3, and if dealer has -2 it becomes 2, then win
    if (abs(playerSum-21) < abs(dealerSum - 21)) {
        //Player won
        std::cout << "Player won!" << std::endl;
    }
    else {
        //Delaer won
        std::cout << "Dealer won!" << std::endl;
    }


    std::cout << "Press any button to continue.." << std::endl;
    if (_getch()) {
        state = setup;
    }
}

int PickedCard() 
{
    int k = rand() % 11;
    return k;
}

void Draw() 
{
    //Player cards
    std::cout << "Player cards: ";
    for (int i = 0; i < playerCards.size(); i++) 
    {
        std::cout << playerCards[i] << ", ";
    }

    std::cout << "Dealer cards: ";
    for (int i = 0; i < dealerCards.size(); i++)
    {
        std::cout << dealerCards[i] << ", ";
    }

    std::cout << "Player sum: " << CardSum(playerCards) << ", Dealer sum: " << CardSum(dealerCards);
}

int CardSum(std::vector<int> cards) {
    int k = 0;
    for (int i = 0; i < cards.size(); i++) 
    {
        k += cards[i];
    }
    return k;
}


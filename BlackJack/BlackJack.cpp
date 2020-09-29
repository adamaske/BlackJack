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

char newCard = 'n';
char stand = 's';

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
                Draw();
                return 0;
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

    char input = _getch();

    switch (input)
    {
        //Add a new card to the vector of cards
        case 'n':
            playerCards.push_back(PickedCard());
            break;
        //Stand, exit out of player state
        case 's':
            state = dealer;
            break;
        default:

         break;
    }

}

void DealerTurn() 
{
    playerSum = CardSum(playerCards);
    //Draw cards until its == or above players sum
    do 
    {
        int newDealerCard = PickedCard();
        dealerCards.push_back(newDealerCard);

        dealerSum = CardSum(dealerCards);
    } while (dealerSum < playerSum);

    state = execute;
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


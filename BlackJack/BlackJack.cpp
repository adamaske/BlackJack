#include <iostream>
#include <vector>
#include <conio.h>
#include "BlackJack.h"
std::vector<int>cards {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

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
        if (playerCash <= 0)
        {
            system("cls");
            std::cout << "Player out of cash, goodbye!";
            return 0;
        }
        else if (dealerCash <= 0)
        {
            system("cls");
            std::cout << "Dealer out of cash, goodbye!";
            return 0;
        }
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
                Betting();
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
    playerBet = 0;
    dealerBet = 0;
    //Clear player cards
    playerCards.clear();
    //Clear dealer cards
    dealerCards.clear();

    state = betting;
}

void Player() 
{
    system("cls");
    //Draw the players cards
    std::cout << "Player cards: " << std::endl;
    for (int i = 0; i < playerCards.size(); i++)
    {
        std::cout << "|" << playerCards[i] << "|";
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
                ExchangeBets(false);
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


        //dealerSum = CardSum(dealerCards);
        ////Check if dealer lost;
        //if (dealerSum > 21) 
        //{
        //    std::cout << "Dealer lost by over 21" << std::endl;
        //    state = setup;
        //    std::cout << "Press any key to continue..." << std::endl;
        //    if (_getch()) {
        //        state = execute;
        //        return;
        //    }
        //    return;
        //}
    } while (dealerSum < playerSum);

    state = execute;
}

void Betting() 
{
    std::cout << "Place your bet: ";
    //Place the inuput in k, then check if its valid
    double k = 0;
    std::cin >> k;

    if (k <= playerCash) {
        //Accept
        playerBet = k;
    }

    if (dealerCash >= playerBet) {
        dealerBet = playerBet;
    }

    state = player;
}

void Execute() {
   
    state = setup;

    playerSum = CardSum(playerCards);
    dealerSum = CardSum(dealerCards);

    bool pWon;
    //Check draw
    if (playerSum == dealerSum || playerSum > 21 && dealerSum > 21 || playerSum == 21 && dealerSum == 21) {
        std::cout << "It's a draw!";
    }
    //Check player bust
    if (playerSum > 21) 
    {

    }
    //who is closets to 21, using abs to get the absoulute, if its -3 goees to 3, and if dealer has -2 it becomes 2, then win
    if (playerSum > dealerSum) {
        //Player won
        std::cout << "Player won!" << std::endl;
        //Put the dealer bet in the playersCash
        playerCash = playerCash + dealerBet;
        dealerCash = dealerCash - dealerBet;
        pWon = true;
    }
    else {
        //Delaer won
        std::cout << "Dealer won!" << std::endl;
        //Players bet in the dealers cash
        dealerCash = dealerCash + playerBet;
        playerCash = playerCash - playerBet;
        pWon = false;
    }

    //Find who won
    Draw(pWon);

    std::cout << "Press any button to continue.." << std::endl;
    if (_getch()) {
        state = setup;
    }
}

int PickedCard() 
{
    int k = rand() % cards.size() + 1;
    return k;
}

void Draw(bool playerWon) 
{
    system("cls");
    //Cash
    std::cout << "Player cash: " << playerCash << "\t" << "Dealer cash: " << dealerCash << std::endl;

    //Player cards
    std::cout << "Player cards: " << std::endl;
    for (int i = 0; i < playerCards.size(); i++) 
    {
        std::cout << "|" << playerCards[i] << "|";
    }
   
    std::cout << "\t" << "Dealer cards: ";
    for (int i = 0; i < dealerCards.size(); i++)
    {
        std::cout << "|" << dealerCards[i] << "|";
    }

    std::cout << std::endl << "Player bet: " << playerBet << "\t" << "Dealer bet: " << dealerBet << std::endl;
    
    std::cout << "Player sum: " << CardSum(playerCards) << "\t" <<"Dealer sum: " << CardSum(dealerCards) << std::endl << std::endl; 
    if (playerWon) {
        std::cout << "Player won!" << std::endl;
    }
    else {
        std::cout << "Dealer won!" << std::endl;
    }

}

int CardSum(std::vector<int> cards) {
    int k = 0;
    for (int i = 0; i < cards.size(); i++) 
    {
        k += cards[i];
    }
    return k;
}


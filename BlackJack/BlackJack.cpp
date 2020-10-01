#include <iostream>
#include <vector>
#include <conio.h>
#include "BlackJack.h"
#include <string>
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

std::string win;

bool startWith2Cards = true;
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

    if (startWith2Cards) {
        for (int i = 0; i < 2; i++) {
            playerCards.push_back(PickedCard());
            dealerCards.push_back(PickedCard());
        }
    }

    state = betting;
}

void Player() 
{
    Draw();
    char input = _getch();

    switch (input)
    {
        //Add a new card to the vector of cards
        case 'n':
            playerCards.push_back(PickedCard());
            //Check if its over 21, then loose
            playerSum = CardSum(playerCards);
            
            
            if (playerSum > 21) 
            {
                win = "bust";
                state = execute;
                Draw(); 
                std::cout << "\n\nPress any key to continue...\n";
                if (_getch()) {
                    state = setup;
                }

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
        playerSum = CardSum(playerCards);
    } while (dealerSum < playerSum);

    state = execute;
}

void Betting() 
{
    Draw();
    //Place the inuput in k, then check if its valid
    int bet = 0;
    
    std::cin >> bet;

    if (bet <= playerCash) {
        //Accept
        playerBet = bet;
    }

    if (dealerCash >= playerBet) {
        dealerBet = playerBet;
    }

    state = player;
}

void Execute() {
    //Fresh sums
    playerSum = CardSum(playerCards);
    dealerSum = CardSum(dealerCards);

    //Check who wins, then get exchange values
    if (playerSum == dealerSum || playerSum == 21 && dealerSum == 21)
    {
        win = "draw";
    }
    else if (playerSum > 21) {
        win = "bust";
        dealerCash = dealerCash + playerBet;
        playerCash = playerCash - playerBet;
    }
    else if (playerSum == 21) {
        win = "player";
        playerCash = playerCash + dealerBet;
        dealerCash = dealerCash - dealerBet;
    }
    else if (dealerSum == 21) {
        win = "dealer";
        dealerCash = dealerCash + playerBet;
        playerCash = playerCash - playerBet;
    }
    else if (playerSum > dealerSum) 
    {
        win = "player";
        playerCash = playerCash + dealerBet;
        dealerCash = dealerCash - dealerBet;
    }
    else if (dealerSum > playerSum) {
        win = "dealer";
        dealerCash = dealerCash + playerBet;
        playerCash = playerCash - playerBet;
    }

    //Find who won
    Draw();
    //Wait for player to press something before restarting
    std::cout << "\nPress any button to continue.." << std::endl;
    if (_getch()) {
        state = setup;
    }
}
//Returns a random number from the card vector
int PickedCard() 
{
    int k = rand() % cards.size();
    return cards[k];
}

void Draw() 
{
    //Get fresh sums
    playerSum = CardSum(playerCards);
    dealerSum = CardSum(dealerCards);
    system("cls");
    //New drawing
    std::cout << "Blackjack" << "\n\n\n";

    std::cout << "Player cash: " << playerCash << "\t" << "\tDealer cash: " << dealerCash << "\n\n";
    std::cout << "[N] for new card\n";
    std::cout << "[S] to stand\n" << "\n\n";

    
    if (state == betting) 
    {
        std::cout << "Place your bet: ";

        return;
    }

    std::cout << "Player bet: " << playerBet << "\t" << "Dealer bet: " << dealerBet << "\n\n\n";

    if (state == execute) 
    {
        if (win == "player") 
        {
            std::cout << "Player won!\n" << "Player gets " << dealerBet << "!";
        }
        else if (win == "dealer") 
        {
            std::cout << "Dealer won!\n" << "Dealer gets " << playerBet << "!";
        }
        else if (win == "draw") 
        {
            std::cout << "It's a draw!";
        }
        else if (win == "bust") 
        {
            std::cout << "Player bust, dealer wins!";
        }
    }

    std::cout << "\nYour sum: " << playerSum << "\tDealer sum: " << dealerSum << "\n\n";

    std::cout << "\nDealer cards: \n";
    //Draw dealer cards, temp display
    for (int i = 0; i < dealerCards.size(); i++) {
        std::cout << dealerCards[i] << ", ";
    }

    if (state == player)
    {
        std::cout << "\n\nPlayer's turn, stand or new card?";
    }

    std::cout << "\nPlayer cards: \n";
    //Draw player cards, temp display
    for (int i = 0; i < playerCards.size(); i++) {
        std::cout << playerCards[i] << ", ";
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


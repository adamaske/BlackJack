#include <iostream>
#include <vector>
#include <conio.h>
#include "BlackJack.h"
#include <string>
#include <ctime>

//Values possible
std::vector<int>cards{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
//Faces aligned with the values, t as 10, then check for it later
std::vector<char> faces{ 'A','2','3' ,'4' ,'5' ,'6' ,'7' ,'8','9','T', 'J','Q','K' };

struct Card {
    char face = 'e';
    int value = 0;
    //Sets the value and face to what its passed, set face to v-1 cause PickedCard returns the value of the card, but the faces array A=0
    //Also checks if the value is a Jack, Queen or King, give it the correct face from the value, then set the value back to 10, due to that they should only be 10s
    void SetupCard(int v) {
        value = v;
        face = faces[v-1];
        if (v > 10) {
            value = 10;
        }
    }
};
//Declaring the vectors
std::vector<Card> myCards{};
std::vector<Card> dealerCards{};

//keeping sums in own ints
int playerSum = 0;
int dealerSum = 0;

enum GameState{ setup, player, dealer,betting, execute};
GameState state = setup;

int playerCash = 100;
int dealerCash = 100;

int playerBet = 0;
int dealerBet = 0;

std::string win;
int main()
{
    //Intro to blackjack
    std::cout << "Welocome to blackjack!\n";
    std::cout << "The game proceeds as such: \n" << "1.You will pick your cards, when you are happy with your collection you can stand! If you go above 21, you loose.\n"
        << "2. You will then place your bet, the house will always match your bet.\n" <<
        "3. The house will show it's cards, then a winner will be decided and bets will be exchanged.\n\n";
    //Function to print and wait for any key to pressed
    AnyKey();
    
    //Set rand to random at start of game, instead of inside PickedCard() so the dealer wich gets instantly dealt cards gets same cards everytime
    srand((unsigned)time(0));

    do
    {
        //Checks if the player and dealer has any cash left, if one dosent, game is finsihed
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
        //Switch with all the states of the game, I find it easeier to make the game more easily expandebale/change the flow
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
        //While true due it always runninig, main() only returns when someone is out of cash
    } while (true);
}

void Setup() 
{
    //Sets starting bids at 10, as per the rules
    playerBet = 10;
    dealerBet = 10;
    //Clear player cards
    myCards.clear();
    //Clear dealer cards
    dealerCards.clear();
    //Next state
    state = player;
}

void Player() 
{
    //Draw to display the input and cards
    Draw();
    //Stores PickedCard in a k var to check it for Ace
    int k = PickedCard();
    //Declare the new card
    Card c;
    //Setup the card first incase its an ace, then the change setup makes to values over 10, can be overridden by the ace check
    c.SetupCard(k);
    //Input to be used, either n or s
    char input = _getch();
    //Switch stand or hit
    switch (input)
    {
        //Add a new card to the vector of cards
        case 'n':
            //Check if its an ace, if player choose 2 for 11 as the value, change the drawn card 
            //Ignore for wrong input
            if (k == 1) {
                std::cout << "You got drew an ace, do you want it to count as 1 or 11? \n[1] for 1 and [2] for 11\n";
                char answer = _getch();
                if (answer == '2') {
                    c.value = 11;
                }
            }
            //Add to the player cards the card initalized above
            myCards.push_back(c);
            //Check if its over 21, then loose
            SetSums();
            if (playerSum > 21) 
            {
                win = "bust";
                dealerCash = dealerCash + playerBet;
                playerCash = playerCash - playerBet;    
                state = execute;
                Draw();
                AnyKey();
                
            }
            if (playerSum == 21) {
                win = "player";
                dealerCash = dealerCash - playerBet;
                playerCash += playerBet;
                state = execute;
                Draw();
                AnyKey();
                
            }
            break;
        //Stand, exit out of player state, go to dealer
        case 's':
            state = betting;
            break;
        default:
         break;
    }

}
//just continues when a key is presseds
void AnyKey() {
    std::cout << "\n\nPress any key to continue...\n";
    if (_getch()) {
        state = setup;
    }
}

void DealerTurn() 
{
    SetSums();
    //Draw cards until its == or above players sum
    do 
    {
        //Delcare the new card, check the new sums then determined if another card is needed
        int newDealerCard = PickedCard();
        Card c;
        c.SetupCard(newDealerCard);
        //Check if its an ace, then check if 11 or 1 gets it futher away from 1, then pick the other one
        if (newDealerCard == 1) {
            int k = dealerSum + 1;
            int j = dealerSum + 11;
            //Takes the absolute/whole number to k and j - 21, to find what is the lowest, without negative numbers interfering, if its 16 then ace
            //17 - 21 = -4, then four, but if its +11, 27 - 21 = 6, so the lower wins and the otherway if its already 21, then 22-21 is 1, so 21 + 11 - 11 is 21 and 1 gets picked
            if (abs(k - 21) < abs(j -21)) {
                c.value = 1;
            }
            else {
                c.value = 11;
            }
        }
        //Adds it to the dealers cards
        dealerCards.push_back(c);
        SetSums();
    } while (dealerSum < playerSum);
    //Only go while the dealer is lower than the player, once its equal or higher, even if the player is at a very low number, dont go further for less risk of going above 21
    state = execute;
}

void Betting() 
{
    Draw();
    std::cin.clear();
    //Place the inuput in k, then check if its valid
    int bet = 0;
    //Gets the bet
    std::cin >> bet;
    //If you can afford the bet go ahead
    if (bet +10 <= playerCash) {
        //Accept, add it to the playerBet var
        playerBet += bet;
    }
    dealerBet = playerBet;
    if (dealerCash >= playerBet) {
        dealerBet = playerBet;
    }

    state = dealer;
}

void Execute() {

    SetSums();
    //Check the diffrent scenarios for win, loose and draw, then get exchange values
    //Should replace the win string with an enum or something, win gets checked by the draw function to know what it should display as the winner
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
        dealerCash = dealerCash - playerBet;
        playerCash += playerBet;
    }
    else if (dealerSum == 21) {
        win = "dealer";
        dealerCash = dealerCash + playerBet;
        playerCash = playerCash - playerBet;
    }
    else if (playerSum < 21 && dealerSum > 21)
    {
        win = "player";
        dealerCash = dealerCash - playerBet;
        playerCash +=  playerBet;
    }
    else if (playerSum > dealerSum)
    {
        win = "player";
        dealerCash = dealerCash - playerBet;
        playerCash += playerBet;
    }
    else if (dealerSum > playerSum) {
        win = "dealer";
        dealerCash = dealerCash + playerBet;
        playerCash = playerCash - playerBet;
    }
    //Find who won
    Draw();
    //Wait for player to press something before restarting
    AnyKey();
}
//Returns a random number from the card vector
int PickedCard() 
{
    int j = rand() % cards.size();
    return cards[j];
}

void Draw() 
{
    SetSums();
    system("cls");
    //New drawing
    std::cout << "Blackjack" << "\n\n\n";
    //always display how much cash they have
    std::cout << "Player cash: " << playerCash << "\t" << "\tDealer cash: " << dealerCash << "\n\n";
    //Always displays the controls, inspired by a snippet of a blackjack program i saw i class
    std::cout << "[N] for new card\n";
    std::cout << "[S] to stand\n" << "\n\n";
    //Dont display any more if the state is betting
    if (state == betting) 
    {
        std::cout <<"Starting bet: 10\n" <<"Place your bet: ";
        return;
    }
    //Show bets
    std::cout << "Player bet: " << playerBet << "\t" << "Dealer bet: " << dealerBet << "\n\n\n";
    //What winner to display
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

    std::cout << "\nYour sum: " << playerSum;
    //if this is the final stage, then also show the house cards
    if (state == execute) {
        std::cout << "\tDealer sum: " << dealerSum << "\n\n";
        std::cout << "\nDealer cards: \n";
        
        //Draw dealer cards, display 11,12,13 as j,q,k and draw t as 10
        for (int i = 0; i < dealerCards.size(); i++) {           
             if (dealerCards[i].face == 'T') {
                std::cout << "10, ";
             }
             else {
                std::cout << dealerCards[i].face << ", ";
             }
        }
    }
    
    //If its the players turn, tell it, else its hard to notice
    if (state == player)
    {
        std::cout << "\n\nPlayer's turn, stand or new card?";
    }
    std::cout << "\nPlayer cards: \n";
   
    //Draw player cards, display 11,12,13 as j,q,k and t as 10
    for (int i = 0; i < myCards.size(); i++) {
        if (myCards[i].face == 'T') {
            std::cout << "10, ";
        }
        else {
            std::cout << myCards[i].face << ", ";
        }
    }
    
}
//Refreshes the sums of cards
void SetSums() {
    int k = 0;
    for (int i = 0; i < myCards.size(); i++) {
        k += myCards[i].value;
    }
    int j = 0;
    for (int i = 0; i < dealerCards.size(); i++) {
        j += dealerCards[i].value;
    }
    playerSum = k;
    dealerSum = j;
}



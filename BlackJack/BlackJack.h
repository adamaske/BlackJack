#pragma once

int main();

void Setup();

void Player();

void DealerTurn();

void Betting();

void Execute();

void ExchangeBets(bool playerWon);

int PickedCard();

void Draw(bool);

int CardSum(std::vector<int> cards);

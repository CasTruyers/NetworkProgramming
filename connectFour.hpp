#pragma once
#include <iostream>
using namespace std;

class connectFourGame
{
public:
    string players[2];
    connectFourGame();
    ~connectFourGame();

    void logUser();
    void render();
    bool enterToken();
    int checkConnect();

private:
    void updateBoard();
    int **board;
    char column;
    bool currentPlayer = 0;
};
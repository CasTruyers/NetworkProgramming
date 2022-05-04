#include <iostream>
#include "connectFour.hpp"
#include <zmq_addon.hpp>

using namespace std;

int main()
{
    connectFourGame connectFour;
    int winner;

    connectFour.logUser();
    while (true)
    {

        // Render playboard, this function clears screen!
        connectFour.render();

        // user enters token in playboard, returns 1 when 'esc'
        if (connectFour.enterToken())
            break;

        // checks for winner, returns 0 when there is no winner
        winner = connectFour.checkConnect();
        if (winner == 0)
            continue;

        connectFour.render();
        cout << connectFour.players[winner - 1] << " has connect-four en wins the game!\n";
        break;
    }

    return 1;
}
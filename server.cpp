#include <iostream>
#include <zmq_addon.hpp>
#include "connectFour.hpp"

bool newThread = 1;
void game()
{
    cout << "new Thread" << endl;

    connectFourServer connectFour;
    connectFour.waitForPlayers();
    newThread = 1;

    while (connectFour.handleNetworkEvent())
    {
        connectFour.checkConnect();

        if (connectFour.getWinner())
            break;
        else
            continue;
    }
    connectFour.declareWinner();
}

int main()
{
    while (true) // closeServer
    {
        if (newThread)
        {
            std::thread gameInstance(game);
            gameInstance.detach();
            newThread = 0;
        }
    }

    return 0;
}
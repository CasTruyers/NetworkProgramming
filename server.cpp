#include <iostream>
#include <zmq_addon.hpp>
#include "connectFour.hpp"

int main()
{
    connectFourServer connectFour;

    connectFour.waitForPlayers();

    while (connectFour.handleNetworkServerEvent())
    {
        connectFour.checkConnect();

        if (connectFour.getWinner())
            break;
        else
            continue;
    }

    connectFour.declareWinner();

    return 0;
}
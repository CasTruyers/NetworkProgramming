#include <iostream>
#include <zmq_addon.hpp>
#include "connectFour.hpp"

int main()
{
    connectFourClient connectFour;

    connectFour.join();

    connectFour.waitForOpponent();

    while (connectFour.handleNetworkClientEvent())
    {
    };

    return 1;
}
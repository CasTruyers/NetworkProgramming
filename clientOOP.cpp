#include <iostream>
#include <zmq_addon.hpp>
#include "connectFour.hpp"

int main()
{
    connectFourClient connectFour;

    connectFour.join();

    connectFour.waitForOpponent();

    while (connectFour.handleNetworkEvent())
    {
    };

    cout << "\nCiao!\n";
    connectFour.~connectFourClient();
    cout << "\nCiao!\n";

    return 1;
}
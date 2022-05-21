#include <iostream>
#include <thread>
#include <zmq_addon.hpp>
#include "connectFour.hpp"

bool newThread = 1;
bool closeServer = 0;

void game()
{
    cout << "New thread\n";
    connectFourServer connectFour;
    // thread id meegeven om communicatie tussen veschillende thread te scheiden
    connectFour.waitForPlayers();
    newThread = 1;

    while (connectFour.handleNetworkEvent() & !closeServer)
    {
        connectFour.checkConnect();

        if (connectFour.getWinner())
            break;
        else
            continue;
    }
    connectFour.declareWinner(closeServer);
}

void getInput()
{
    std::cin.get();
    closeServer = 1;
}

int main()
{
    std::thread close(getInput);
    while (!closeServer)
    {
        // class hier instantiaten, waitForPLayer() en class pointer meegeven aan thread. Lastig om locatie van gameInstatie in vector bij te houden wanneer andere zich verwijderen uit vector.
        if (newThread)
        {
            std::thread gameInstance(game);
            gameInstance.detach();
            newThread = 0;
        }
    }

    return 0;
}
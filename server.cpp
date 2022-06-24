#include <iostream>
#include <zmq_addon.hpp>
#include "connectFour.hpp"

int count1 = 0; // p1 wint zonder hold
int count2 = 0; // p2 wint zonder hold
int count3 = 0; // p1 wint na hold
int count4 = 0; // p2 wint na hold
bool newThread = 1;
zmq::context_t ctx;
zmq::socket_t *sockPush;
zmq::message_t z_out;
string msg_out;

void sendStats()
{
    cout << "in sendStats\n";
    msg_out = "connectFourStat>" + to_string(count1) + ">" + to_string(count2) + ">" + to_string(count3) + ">" + to_string(count4) + ">";
    z_out.rebuild(msg_out.data(), msg_out.length());
    cout << "sending:" << msg_out << endl;
    sockPush->send(z_out);
    cout << "stats sund\n";
}

void game()
{
    int count = 0;
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
    count = connectFour.declareWinner();
    if (count == 1)
        count1++;
    else if (count == 2)
        count2++;
    else if (count == 3)
        count3++;
    else if (count == 4)
        count4++;
    sendStats();
}

int main()
{
    sockPush = new zmq::socket_t(ctx, ZMQ_PUSH);
    sockPush->connect("tcp://benternet.pxl-ea-ict.be:24041");
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
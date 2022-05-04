#include <iostream>
#include <string>
#include <array>
#include <zmq_addon.hpp>
using namespace std;

zmq::context_t ctx;

int main()
{
    zmq::message_t *z_in = new zmq::message_t;
    zmq::message_t z_out;
    string msg_in;
    string msg_out;
    string players[2];
    unsigned first;
    unsigned last;
    bool currenPlayer;

    // Push and Sub client socket

    zmq::socket_t sockPush(ctx, ZMQ_PUSH);
    sockPush.connect("tcp://benternet.pxl-ea-ict.be:24041");

    zmq::socket_t sockSub(ctx, ZMQ_SUB);
    sockSub.connect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub.setsockopt(ZMQ_SUBSCRIBE, "connectFourPlayer>", 18);

    // Receiving clients

    sockSub.recv(z_in);
    msg_in = z_in->to_string();
    first = msg_in.find_first_of('>');
    last = msg_in.find_last_of('>');
    players[0] = msg_in.substr(first + 1, last - first - 1);
    cout << players[0] << " is player 1\n";

    sockSub.recv(z_in);
    msg_in = z_in->to_string();
    first = msg_in.find_first_of('>');
    last = msg_in.find_last_of('>');
    players[1] = msg_in.substr(first + 1, last - first - 1);
    cout << players[1] << " is player 2\n";

    // Push players to clients

    msg_out = "connectFourServer>0>" + players[0];
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush.send(z_out);

    msg_out = "connectFourServer>1>" + players[1];
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush.send(z_out);

    while (true)
    {
        if (currenPlayer)
            msg_out = "connectFourServer>1>turn";
        else
            msg_out = "connectFourServer>0>turn";

        z_out.rebuild(msg_out.data(), msg_out.length());
        sockPush.send(z_out);
        sockSub.recv(z_in);
        cout << z_in->to_string() << endl;
        currenPlayer = !currenPlayer;
    }

    // // free connection
    delete z_in;
    sockSub.disconnect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub.close();
    sockPush.disconnect("tcp://benternet.pxl-ea-ict.be:24041");
    sockPush.close();
}
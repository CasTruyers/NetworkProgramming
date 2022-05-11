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
    string action;
    string player;
    string players[2];
    string substr;
    string column;
    unsigned first;
    unsigned last;
    bool currentPlayer;
    int i = 0;

    int **board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;

    // Push and Sub client socket
    zmq::socket_t sockSub(ctx, ZMQ_SUB);
    sockSub.connect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub.setsockopt(ZMQ_SUBSCRIBE, "connectFourPlayer>", 18);

    zmq::socket_t sockPush(ctx, ZMQ_PUSH);
    sockPush.connect("tcp://benternet.pxl-ea-ict.be:24041");

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
        if (currentPlayer)
            msg_out = "connectFourServer>1>turn";
        else
            msg_out = "connectFourServer>0>turn";

        z_out.rebuild(msg_out.data(), msg_out.length());
        sockPush.send(z_out);

        do
        {
            sockSub.recv(z_in);

            msg_in = z_in->to_string();
            // message filtering
            istringstream(msg_in.substr(msg_in.find_first_of('>') + 1, 1)) >> currentPlayer;
            action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.find_last_of(':') - msg_in.find_last_of('>') - 1);
            if (action == "enter")
                column = msg_in.substr(msg_in.find_last_of(":") + 1, 1);
        } while (action != "enter" && action != "quit");

        if (action == "enter")
            msg_out = "connectFourServer>" + to_string(currentPlayer) + ">enter:" + column;
        else if (action == "quit")
            msg_out = "connectFourServer>" + to_string(currentPlayer) + ">quit";

        cout << "Sending message: '" + msg_out + "'" << endl;
        z_out.rebuild(msg_out.data(), msg_out.length());
        sockPush.send(z_out);

        // next players turn
        currentPlayer = !currentPlayer;
        i++;
    }

    // free connection
    delete z_in;
    sockSub.disconnect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub.close();
    sockPush.disconnect("tcp://benternet.pxl-ea-ict.be:24041");
    sockPush.close();
}
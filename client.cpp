#include <iostream>
#include <zmq_addon.hpp>
using namespace std;

zmq::context_t ctx;

int main()
{
    zmq::message_t *z_in = new zmq::message_t;
    zmq::message_t z_out;
    string msg_in;
    string msg_out;
    string name;
    string players[2];
    bool me;
    unsigned first;
    unsigned last;
    string action;
    bool currentPlayer;

    // Subscribe to server

    zmq::socket_t sockSub(ctx, ZMQ_SUB);
    sockSub.connect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub.setsockopt(ZMQ_SUBSCRIBE, "connectFourServer>", 18);

    // Send username

    zmq::socket_t sockPush(ctx, ZMQ_PUSH);
    sockPush.connect("tcp://benternet.pxl-ea-ict.be:24041");
    cout << "Enter username: ";
    cin >> name;
    msg_out = "connectFourPlayer>" + name + ">";
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush.send(z_out);

    cout << "\nWaiting for opponent... \n";

    sockSub.recv(z_in);
    system("clear");
    msg_in = z_in->to_string();
    last = msg_in.find_last_of('>');
    players[0] = msg_in.substr(last + 1, msg_in.length() - last - 1);

    sockSub.recv(z_in);
    msg_in = z_in->to_string();
    last = msg_in.find_last_of('>');
    players[1] = msg_in.substr(last + 1, msg_in.length() - last - 1);

    if (players[0] == name)
        me = 0;
    else
        me = 1;

    cout << players[0] << " is player 1\n"
         << players[1] << " is player 2\n"
         << "I am player " << me + 1 << endl;

    while (action != "quit")
    {
        sockSub.recv(z_in);
        msg_in = z_in->to_string();
        action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.length());
        istringstream(msg_in.substr(msg_in.find_first_of('>') + 1, 1)) >> currentPlayer;
        cout << "action: " << action << endl
             << "currentPlayer: " << currentPlayer << endl;

        cin.clear();
        fflush(stdin);
        if (currentPlayer == me)
        {
            cout << "press enter to make move" << endl;
            cin.ignore();
            msg_out = "connectFourPlayer>" + to_string(me) + ">done";
            z_out.rebuild(msg_out.data(), msg_out.length());
            sockPush.send(z_out);
        }
        else
            cout << players[!me] << " is making a move" << endl;
    }

    // cleanup

    delete z_in;
    sockSub.disconnect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub.close();
    sockPush.disconnect("tcp://benternet.pxl-ea-ict.be:24041");
    sockPush.close();

    return 1;
}
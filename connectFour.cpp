#include "connectFour.hpp"

//* connectFour BaseClass

connectFour::connectFour()
{
    player = 0;
    sockSub = new zmq::socket_t(ctx, ZMQ_SUB);
    sockSub->connect("tcp://benternet.pxl-ea-ict.be:24042");

    sockPush = new zmq::socket_t(ctx, ZMQ_PUSH);
    sockPush->connect("tcp://benternet.pxl-ea-ict.be:24041");
    z_in = new zmq::message_t;

    board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;
}

connectFour::~connectFour()
{
    sockSub->disconnect("tcp://benternet.pxl-ea-ict.be:24042");
    sockPush->disconnect("tcp://benternet.pxl-ea-ict.be:24041");
    sockSub->close();
    sockPush->close();
    delete sockSub;
    delete sockPush;

    delete z_in;
    for (int i = 0; i < 6; i++)
    {
        delete[] board[i];
    }
    delete[] board;
}

void connectFour::updateBoard()
{
    cout << "\nupdating board on column: " << column << endl;

    if (board[5][column - '0' - 1] == 0) // first token in column
        board[5][column - '0' - 1] = player ? 2 : 1;
    else // search for last token in column x
        for (int row = 0; (row < 5); row++)
            if (board[row + 1][column - '0' - 1] != 0)
            {
                board[row][column - '0' - 1] = player ? 2 : 1;
                break;
            }
}

void connectFourClient::updateBoard()
{
    cout << "\nupdating board on column: " << column << endl;

    if (board[5][column - '0' - 1] == 0) // first token in column
        board[5][column - '0' - 1] = player ? 2 : 1;
    else // search for last token in column x
        for (int row = 0; (row < 5); row++)
            if (board[row + 1][column - '0' - 1] != 0)
            {
                board[row][column - '0' - 1] = player ? 2 : 1;
                break;
            }
}

//* connectFourClient

connectFourClient::connectFourClient()
{
    player = 0;
    sockSub = new zmq::socket_t(ctx, ZMQ_SUB);
    sockSub->connect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub->setsockopt(ZMQ_SUBSCRIBE, "connectFourServer>", 18);

    sockPush = new zmq::socket_t(ctx, ZMQ_PUSH);
    sockPush->connect("tcp://benternet.pxl-ea-ict.be:24041");
    z_in = new zmq::message_t;

    board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;
}

connectFourClient::~connectFourClient()
{
    sockSub->disconnect("tcp://benternet.pxl-ea-ict.be:24042");
    sockPush->disconnect("tcp://benternet.pxl-ea-ict.be:24041");
    sockSub->close();
    sockPush->close();
    delete sockSub;
    delete sockPush;

    delete z_in;
    for (int i = 0; i < 6; i++)
    {
        delete[] board[i];
    }
    delete[] board;
}

void connectFourClient::join()
{
    cout << "Enter username: ";
    cin >> name;

    // create message with username, send to subscribed server, to join game.
    msg_out = "connectFourPlayer>" + name + ">join:";
    z_out.rebuild(msg_out.data(), msg_out.length());
    cout << "\nC: sending join request...\n";
    sockPush->send(z_out);
    cout << "\nC: join request send\n";
}

void connectFourClient::waitForOpponent()
{
    cout << "\nwaiting for opponent...\n";

    // opponent joined, server sending players.
    sockSub->recv(z_in);
    system("clear");
    msg_in = z_in->to_string();
    last = msg_in.find_last_of('>');
    players[0] = msg_in.substr(last + 1, msg_in.length() - last - 1);

    sockSub->recv(z_in);
    msg_in = z_in->to_string();
    last = msg_in.find_last_of('>');
    players[1] = msg_in.substr(last + 1, msg_in.length() - last - 1);

    if (players[0] == name)
        me = 0;
    else
        me = 1;

    cout << players[0] << " is player 1\n"
         << players[1] << " is player 2\n\n"
         << "I am player " << me + 1 << "\n\n";
}

bool connectFourClient::handleNetworkEvent()
{
    sockSub->recv(z_in);
    msg_in = z_in->to_string();
    action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.find_last_of(':') - msg_in.find_last_of('>') - 1);
    istringstream(msg_in.substr(msg_in.find_first_of('>') + 1, 1)) >> player;
    system("clear");
    cout << "\naction: " << action << endl
         << "currentPlayer: " << player << endl;

    if (action == "turn")
    {
        if (player == me)
        {
            if (enterToken())
                msg_out = "connectFourPlayer>" + to_string(me) + ">quit:";
            else
                msg_out = "connectFourPlayer>" + to_string(me) + ">enter:" + column;

            // cout << "\nsending: " << msg_out << endl;
            z_out.rebuild(msg_out.data(), msg_out.length());
            sockPush->send(z_out);
        }
        else
            cout << players[!me] << " is making a move" << endl;
    }
    else if (action == "quit")
    {
        if (player == me)
            cout << "You left the game\n";
        else
            cout << players[player] << " left the game\n";

        return 0;
    }
    else if (action == "enter")
    {
        receivedColumn = msg_in.substr(msg_in.find_last_of(":") + 1, 1);
        column = receivedColumn.front();
        // cout << "message received: " << msg_in << endl
        //      << "updating board on column: " << column << endl;
        updateBoard();
        render();
    }
    else if (action == "wins")
    {
        if (player == me)
            cout << "You win the game! Congratulations " << players[me] << ".\n\n";
        else
            cout << "You suck, " << players[!me] << " wins the game.\n\n";
        return 0;
    }

    return 1;
}

bool connectFourClient::enterToken()
{
    int returnVal;
    render();
    cin.clear();
    fflush(stdin); //! vraag aan bart
    cout << "place in column ('esc' to exit): ";

    while (true)
    {
        cin >> column;
        // cout << "\nOK\n";
        if (column == 27) // escape
            returnVal = 1;
        else if (board[0][column - '0' - 1] == 0) // Token can be placed
            returnVal = 0;
        else
        {
            cout << "column " << column << " is already full\nChoose an empty column ('esc' for exit): ";
            continue;
        }
        break;
    }
    return returnVal;
}

void connectFourClient::render()
{
    cout << " 1 2 3 4 5 6 7" << endl;
    for (int i = 0; i < 6; i++)
    {
        cout << '|';
        for (int j = 0; j < 7; j++)
        {
            if (board[i][j] == 0)
                cout << ' ';
            else if (board[i][j] == 1)
                cout << 'O';
            else if (board[i][j] == 2)
                cout << 'X';
            else
                cout << 'E';
            cout << '|';
        }
        cout << "\n";
    }
    cout << "\n";
}

//* connectFourServer

connectFourServer::connectFourServer() : connectFour()
{
    winner = 0;
    sockSub->setsockopt(ZMQ_SUBSCRIBE, "connectFourPlayer>", 18);
}

void connectFourServer::waitForPlayers()
{
    do
    {
        sockSub->recv(z_in);
        msg_in = z_in->to_string();
        action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.find_last_of(':') - msg_in.find_last_of('>') - 1);
    } while (action != "join");

    first = msg_in.find_first_of('>');
    last = msg_in.find_last_of('>');
    players[0] = msg_in.substr(first + 1, last - first - 1);
    cout << players[0] << " is player 1\n";

    do
    {
        sockSub->recv(z_in);
        msg_in = z_in->to_string();
        action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.find_last_of(':') - msg_in.find_last_of('>') - 1);
    } while (action != "join");

    first = msg_in.find_first_of('>');
    last = msg_in.find_last_of('>');
    players[1] = msg_in.substr(first + 1, last - first - 1);
    cout << players[1] << " is player 2\n";

    // Push players to clients
    msg_out = "connectFourServer>0>" + players[0];
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush->send(z_out);
    cout << "\nsend: " + msg_out;

    msg_out = "connectFourServer>1>" + players[1];
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush->send(z_out);
    cout << "\nsend: " + msg_out;
}

// void connectFourServer::publishThreadID()
// {
//     thisThreadID = (idk hier)this_thread::get_id();
//     msg_out = thisThreadID;
//     z_out.rebuild(msg_out.data(), msg_out.length());
//     sockPush->send(z_out);
// }

bool connectFourServer::handleNetworkEvent()
{
    if (player)
        msg_out = "connectFourServer>1>turn:";
    else
        msg_out = "connectFourServer>0>turn:";

    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush->send(z_out);
    cout << "\nsend: " + msg_out;

    do
    {
        sockSub->recv(z_in);
        msg_in = z_in->to_string();
        istringstream(msg_in.substr(msg_in.find_first_of('>') + 1, 1)) >> player;
        action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.find_last_of(':') - msg_in.find_last_of('>') - 1);
    } while (action != "enter" && action != "quit");

    if (action == "enter")
    {
        receivedColumn = msg_in.substr(msg_in.find_last_of(":") + 1, 1);
        column = receivedColumn.front();
        msg_out = "connectFourServer>" + to_string(player) + ">enter:" + column;
        updateBoard();
    }
    else if (action == "quit")
        msg_out = "connectFourServer>" + to_string(player) + ">quit";

    cout << "Sending message: '" + msg_out + "'" << endl;
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush->send(z_out);

    // next players turn
    player = !player;

    if (action == "quit")
        return 0;
    else
        return 1;
}

void connectFourServer::declareWinner()
{
    // if (!closeServer)
    // {
    msg_out = "connectFourServer>" + to_string(winner - 1) + ">wins:";
    cout << players[!player] << " wins.\n";
    // }
    // else
    // {
    //     msg_out = "connectFourServer>" + to_string(winner - 1) + ">close:";
    //     cout << "Closing game thread: " << std::this_thread::get_id();
    // }
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush->send(z_out);
}

void connectFourServer::checkConnect()
{
    int c1h, c2h, c1v, c2v, c1dr, c2dr, c1dl, c2dl;
    for (int i = 0; i < 7; i++)
    {
        c1h = 0, c2h = 0, c1v = 0, c2v = 0;
        for (int j = 0; j < 7; j++)
        {
            if (i < 6)
            {
                // horizontal
                if (board[i][j] == 1)
                    c1h++, c2h = 0;
                else if (board[i][j] == 2)
                    c1h = 0, c2h++;
                else
                    c1h = 0, c2h = 0;
            }

            if (j < 6)
            {
                // vertical
                if (board[j][i] == 1)
                    c1v++, c2v = 0;
                else if (board[j][i] == 2)
                    c1v = 0, c2v++;
                else
                    c1v = 0, c2v = 0;
            }

            if (c1h == 4 || c1v == 4)
                winner = 1;
            else if (c2h == 4 || c2v == 4)
                winner = 2;
        }
    }

    // diagonal
    for (int r = 0; r < 3; r++)
    {
        for (int i = 0; i < 4; i++)
        {
            c1dr = 0, c2dr = 0, c1dl = 0, c2dl = 0;
            // right
            for (int j = 0; j < 4; j++)
            {
                if (board[3 - j + r][i + j] == 1)
                    c1dr++, c2dr = 0;
                else if (board[3 - j + r][i + j] == 2)
                    c1dr = 0, c2dr++;
                else
                    c1dr = 0, c2dr = 0;
            }

            // left
            for (int j = 0; j < 4; j++)
            {
                if (board[j + r][3 + j - i] == 1)
                    c1dl++, c2dl = 0;
                else if (board[j + r][3 + j - i] == 2)
                    c1dl = 0, c2dl++;
                else
                    c1dl = 0, c2dl = 0;
            }

            if (c1dr == 4 || c1dl == 4)
                winner = 1;
            else if (c2dr == 4 || c2dl == 4)
                winner = 2;
        }
    }
}
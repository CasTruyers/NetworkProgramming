#include "connectFour.hpp"

connectFourClient::connectFourClient()
{
    sockSub = new zmq::socket_t(ctx, ZMQ_SUB);
    sockSub->connect("tcp://benternet.pxl-ea-ict.be:24042");
    sockSub->setsockopt(ZMQ_SUBSCRIBE, "connectFourServer>", 18);

    sockPush = new zmq::socket_t(ctx, ZMQ_PUSH);
    sockPush->connect("tcp://benternet.pxl-ea-ict.be:24041");

    board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;
}

connectFourClient::~connectFourClient()
{
    sockSub->close();
    sockPush->close();
    // sockSub->disconnect("tcp://benternet.pxl-ea-ict.be:24042");
    // sockSub->~socket_t();
    // sockPush->disconnect("tcp://benternet.pxl-ea-ict.be:24041");
    // sockPush->~socket_t();

    delete z_in;
    for (int i = 0; i < 6; i++)
        delete[] board[i];
    delete[] board;
}

void connectFourClient::join()
{
    cout << "Enter username: ";
    cin >> name;

    // create message with username, send to subscribed server, to join game.
    msg_out = "connectFourPlayer>" + name + ">";
    z_out.rebuild(msg_out.data(), msg_out.length());
    sockPush->send(z_out);
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
    //! hier dus ook beter filteren op enter values
    sockSub->recv(z_in);
    msg_in = z_in->to_string();
    action = msg_in.substr(msg_in.find_last_of('>') + 1, msg_in.find_last_of(':') - msg_in.find_last_of('>') - 1);
    istringstream(msg_in.substr(msg_in.find_first_of('>') + 1, 1)) >> player;
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

            cout << "\nsending: " << msg_out << endl;
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
        cout << "message received: " << msg_in << endl
             << "updating board on column: " << column << endl;
        updateBoard();
        render();
    }

    return 1;
}

bool connectFourClient::enterToken()
{
    int returnVal;
    render();
    cin.clear();
    fflush(stdin);
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

void connectFourClient::updateBoard()
{
    // first token in column
    if (board[5][column - '0' - 1] == 0)
        board[5][column - '0' - 1] = player ? 2 : 1;

    // search for last token in column x
    else
        for (int row = 0; (row < 5); row++)
            if (board[row + 1][column - '0' - 1] != 0)
            {
                board[row][column - '0' - 1] = player ? 2 : 1;
                break;
            }
}

void connectFourClient::render()
{
    // system("clear");
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

//********************/
//**connectFourLocal**/
//********************/

connectFourLocal::connectFourLocal()
{
    board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;
}

connectFourLocal::~connectFourLocal()
{
    for (int i = 0; i < 6; i++)
        delete[] board[i];
    delete[] board;
}

void connectFourLocal::logUser()
{
    system("clear");
    cout << "Klik op ENTER om de 2022 award winning 4-op-een-rij te spelen";
    cin.ignore();
    cout << "Geef de namen in \n";
    cout << "Speler 1: ";
    cin >> players[0];
    cout << "speler 2: ";
    cin >> players[1];
    system("clear");
}

void connectFourLocal::render()
{
    system("clear");
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

bool connectFourLocal::enterToken()
{
    cout << players[currentPlayer] << " -> place in column ('esc' to exit): ";

    while (true)
    {
        cin >> column;

        if (column == 27) // escape
            return 1;
        else if (board[0][column - '0' - 1] == 0) // Token can be placed
            updateBoard();
        else
        {
            cout << "column " << column << " is already full\nChoose an empty column ('esc' for exit): ";
            continue;
        }
        break;
    }

    currentPlayer = !currentPlayer;
    return 0;
}

void connectFourLocal::updateBoard()
{
    // first token in column
    if (board[5][column - '0' - 1] == 0)
        board[5][column - '0' - 1] = currentPlayer ? 2 : 1;

    // search for last token in column x
    else
        for (int row = 0; (row < 5); row++)
            if (board[row + 1][column - '0' - 1] != 0)
            {
                board[row][column - '0' - 1] = currentPlayer ? 2 : 1;
                break;
            }
}

int connectFourLocal::checkConnect()
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
                return 1;
            else if (c2h == 4 || c2v == 4)
                return 2;
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
                return 1;
            else if (c2dr == 4 || c2dl == 4)
                return 2;
        }
    }
    return 0;
}
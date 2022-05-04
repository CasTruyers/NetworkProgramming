#include "connectFour.hpp"

connectFourGame::connectFourGame()
{
    board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;
}

connectFourGame::~connectFourGame()
{
    for (int i = 0; i < 6; i++)
        delete[] board[i];
    delete[] board;
}

void connectFourGame::logUser()
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

void connectFourGame::render()
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

bool connectFourGame::enterToken()
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

void connectFourGame::updateBoard()
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

int connectFourGame::checkConnect()
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
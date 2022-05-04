#include <iostream>
#include <string>
#include <array>
#include <zmq_addon.hpp>
using namespace std;

void displayBoard(int **board);
void logUsers(string &player1, string &player2);
void enterToken(char input, bool currentPlayer, int **board);
int checkRows(int **board);

zmq::context_t ctx;

int main()
{
    string players[2];
    unsigned first;
    unsigned last;
    char column;
    bool currentPlayer = 0;
    int winner;

    int **board = new int *[6];
    for (int i = 0; i < 7; i++)
        board[i] = new int[7];

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 1;

    logUsers(players[0], players[1]);
    while (true)
    {
        cout << players[currentPlayer] << " is aan de beurt:\n\n";
        displayBoard(board);
        cin >> column;

        if (column == 27) // escape
            break;

        if (board[0][column - '0' - 1] == 0) // Token can be placed
            enterToken(column, currentPlayer, board);
        else
        {
            cout << "Deze positie is al bezet door een token\n";
            continue;
        }
        system("clear");
        currentPlayer = !currentPlayer;
        winner = checkRows(board);

        if (winner == 0)
            continue;
        else if (winner == 1)
        {
            cout << players[0] << " heeft vier op een rij en wint het spel!\n\n";
            displayBoard(board);
        }
        else if (winner == 2)
        {
            cout << players[1] << " heeft vier op een rij en wint het spel!\n\n";
            displayBoard(board);
        }
        break;
    }

    // free memory
    for (int i = 0; i < 6; i++)
        delete[] board[i];
    delete[] board;
}

int checkRows(int **board)
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

void displayBoard(int **board)
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

void enterToken(char column, bool currentPlayer, int **board)
{
    if (board[5][column - '0' - 1] == 0) // first token in column
        board[5][column - '0' - 1] = currentPlayer ? 2 : 1;
    else // search for last token in row x
        for (int row = 0; (row < 5); row++)
            if (board[row + 1][column - '0' - 1] != 0)
            {
                board[row][column - '0' - 1] = currentPlayer ? 2 : 1;
                break;
            }
}

void logUsers(string &player1, string &player2)
{
    system("clear");
    cout << "Klik op ENTER om de 2022 award winning 4-op-een-rij te spelen";
    cin.ignore();
    cout << "Geef de namen in \n";
    cout << "Speler 1: ";
    cin >> player1;
    cout << "speler 2: ";
    cin >> player2;
    system("clear");
}
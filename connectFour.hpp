#pragma once
#include <iostream>
#include <zmq_addon.hpp>
using namespace std;

//* Base Class
class connectFour
{
public:
    connectFour();
    ~connectFour();

    void updateBoard();

protected:
    string players[2];
    bool player;
    int **board;
    string receivedColumn; //! hierover bart vragen
    char column;
    zmq::context_t ctx;
    zmq::socket_t *sockSub;
    zmq::socket_t *sockPush;
    zmq::message_t *z_in;
    zmq::message_t z_out;
    string msg_in;
    string msg_out;
    unsigned first;
    unsigned last;
    string action;
};

//* Client Class
class connectFourClient : public connectFour
{
public:
    connectFourClient();

    void join();
    void render();
    bool enterToken();
    void waitForOpponent();
    bool handleNetworkEvent();

private:
    bool me;
    string name;
};

//* Server Class
class connectFourServer : public connectFour
{
public:
    connectFourServer();

    void waitForPlayers();
    bool handleNetworkEvent();
    void checkConnect();
    void declareWinner();
    int getWinner() { return winner; };

private:
    int winner;
};

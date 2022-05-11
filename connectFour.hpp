#pragma once
#include <iostream>
#include <zmq_addon.hpp>
using namespace std;

class connectFourClient
{
public:
    string players[2];
    connectFourClient();
    ~connectFourClient();

    void join();
    void render();
    bool enterToken();
    int checkConnect();
    void waitForOpponent();
    bool handleNetworkEvent();

private:
    void updateBoard();
    int **board;
    char column;
    bool player = 0;
    zmq::context_t ctx;
    zmq::socket_t *sockSub;
    zmq::socket_t *sockPush;
    zmq::message_t *z_in;
    zmq::message_t z_out;
    string msg_in;
    string msg_out;
    bool me;
    unsigned first;
    unsigned last;
    string action;
    string name;
    string receivedColumn;
};

class connectFourServer
{
public:
    string players[2];
    connectFourServer();
    ~connectFourServer();

    void waitForPlayers();
    void render();
    bool enterToken();
    void checkConnect();
    bool handleNetworkEvent();
    void updateBoard();
    void declareWinner();
    int winner;

private:
    int **board;
    char column;
    bool player;
    zmq::context_t ctx;
    zmq::socket_t *sockSub;
    zmq::socket_t *sockPush;
    zmq::message_t *z_in;
    zmq::message_t z_out;
    string msg_in;
    string msg_out;
    bool me;
    unsigned first;
    unsigned last;
    string action;
    string name;
    string receivedColumn; //! hierover bart vragen
    int i;
};

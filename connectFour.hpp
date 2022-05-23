#pragma once
#include <iostream>
#include <zmq_addon.hpp>
#include <thread>
#include <chrono>
using namespace std;

//* Base Class
class connectFour
{
public:
    connectFour();
    ~connectFour();
    void updateBoard();

    string players[2];
    bool player;
    int **board;
    string receivedColumn; //! hierover vragen
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
class connectFourClient
{
public:
    connectFourClient();
    ~connectFourClient();

    void join();
    void render();
    bool enterToken();
    void waitForOpponent();
    bool handleNetworkEvent();

    void updateBoard();

    bool me;
    string name;

    string players[2];
    bool player;
    int **board;
    string receivedColumn; //! hierover vragen
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

//* Server Class
class connectFourServer : public connectFour
{
public:
    connectFourServer();

    void waitForPlayers();
    void publishThreadID();
    bool handleNetworkEvent();
    void checkConnect();
    void declareWinner();
    int getWinner() { return winner; };

private:
    char *thisThreadID;
    int winner;
};
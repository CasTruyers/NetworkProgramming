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
    zmq::message_t *z_in = new zmq::message_t;
    zmq::message_t z_out;
    string msg_in;
    string msg_out;
    bool me;
    unsigned first;
    unsigned last;
    string action;
    string name;
};

class connectFourLocal
{
public:
    string players[2];
    connectFourLocal();
    ~connectFourLocal();

    void logUser();
    void render();
    bool enterToken();
    int checkConnect();

private:
    void updateBoard();
    int **board;
    char column;
    bool currentPlayer = 0;
};

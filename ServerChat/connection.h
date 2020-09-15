#pragma once

#define MAX_CONN_QUEUE 3 // max number of connections the server can queue
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 2020

class Connection
{
public:
    Connection();
    int get_socketDesc();
    void start_server();
    void stop_server();
private:
    int socketDesc;
    bool running;
};
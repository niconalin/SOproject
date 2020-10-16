#pragma once

const int MAX_CONN_QUEUE = 3; // max number of connections the server can queue
const char SERVER_ADDRESS [16] = "127.0.0.1";
const int SERVER_PORT = 2020;

enum Option {LOGIN, REGISTRATION};

class Connection
{
public:
    Connection();
    int get_socketDesc();
    void start_server();
    void stop_server();
    int receive_option(int socketDesc);
private:
    int socketDesc;
    bool running;
};

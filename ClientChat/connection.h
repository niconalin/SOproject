#pragma once

#include <netinet/in.h>

const int MAX_CONN_QUEUE = 3; // max number of connections the server can queue
const char SERVER_ADDRESS [16] = "127.0.0.1";
const int SERVER_PORT = 2020;

enum Option {LOGIN, REGISTRATION};

class Connection
{
public:
    Connection();
    int get_socketDesc();
    void open_connection();
    void close_connection();
    void send_option(int option);
private:
    int socketDesc;
    struct sockaddr_in server_addr = {0}; // variables for handling a socket
};

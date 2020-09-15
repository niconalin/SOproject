#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"
#include "connection.h"

Connection::Connection(){}

//return the socket descriptor
int Connection::get_socketDesc(){
    return socketDesc;
}

//open connection
void Connection::open_connection()
{
    int ret;

    // variables for handling a socket
    struct sockaddr_in server_addr; // some fields are required to be fill with 0

    // create a socket
    socketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDesc < 0)
        handle_error("Could not create socket");

    // set up parameters for the connection
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); // network byte order!

    // initiate a connection on the socket
    ret = connect(socketDesc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret)
        handle_error("Could not create connection");
    if (DEBUG)
        fprintf(stderr, "Connection established\n");
}

//close connection
void Connection::close_connection() {
    int ret;

    // close the socket
    ret = close(socketDesc);
    if (ret)
        handle_error("Cannot close socket");
    if (DEBUG)
        fprintf(stderr, "Connection closed\n");
}

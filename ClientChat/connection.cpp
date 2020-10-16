#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#include "common.h"
#include "connection.h"

//constructor
Connection::Connection(){
  // create a socket
  this->socketDesc = socket(AF_INET, SOCK_STREAM, 0);
  if (this->socketDesc < 0)
      handle_error("Could not create socket");

  // set up parameters for the connection
  this->server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_port = htons(SERVER_PORT); // network byte order!
}

//return socket descriptor
int Connection::get_socketDesc(){
    return socketDesc;
}

//open connection
void Connection::open_connection()
{
    int ret;

    // initiate a connection on the socket
    ret = connect(this->socketDesc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret)
        handle_error("Could not create connection");
    if (DEBUG) cerr << "Connection: CONNECTION ESTABILISHED" << endl;
}

//close connection
void Connection::close_connection() {
    int ret;

    // close the socket
    ret = close(socketDesc);
    if (ret)
        handle_error("Cannot close socket");
    if (DEBUG) cerr << "Connection: CONNECTION CLOSED" << endl;
}

//send option to the server
void Connection::send_option(int option) {
  //informs the server what type of data we are going to send
  while ((send(this->socketDesc, &option, sizeof(option), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Connection: OPTION SENT" << endl;
}

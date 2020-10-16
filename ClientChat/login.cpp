#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "connection.h"
#include "login.h"

//constructor
Login::Login(Connection* connection, string username, string password)
{
  this->connection = connection;
  this->username = username;
  this->password = password;
  this->logged = false;
  //set and send mode option to the server
  this->connection->send_option(LOGIN);
}

//get login status
bool Login::get_login_status() {
  return this->logged;
}

//send login data to the server
void Login::send_login_data() {
  //send the username to the server
  while ((send(this->connection->get_socketDesc(), &this->username, sizeof(this->username), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Login: USERNAME SENT" << endl;
  //send the password to the server
  while ((send(this->connection->get_socketDesc(), &this->password, sizeof(this->password), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Login: PASSWORD SENT" << endl;
}

//wait for the login result from the server
void Login::wait_login_response() {
  int recv_bytes;
  while ((recv_bytes = recv(this->connection->get_socketDesc(), &this->logged, sizeof(this->logged), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot read from socket");
  }
  if (recv_bytes == 0) return;
  if (DEBUG) cerr << "Login: RESPONSE RECEIVED" << endl;
}

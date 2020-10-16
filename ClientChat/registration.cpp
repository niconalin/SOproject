#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "connection.h"
#include "registration.h"

//constructor
Registration::Registration(Connection* connection, string username, string password)
{
  this->connection = connection;
  this->username = username;
  this->password = password;
  this->registrated = NOT_YET_REGISTRATED;
  //set and send mode option to the server
  this->connection->send_option(REGISTRATION);
}

//get registration status
int Registration::get_registration_status() {
  return this->registrated;
}

//send registration data to the server
void Registration::send_registration_data() {
  //send the username to the server
  while ((send(this->connection->get_socketDesc(), &this->username, sizeof(this->username), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Registration: USERNAME SENT" << endl;
  //send the password to the server
  while ((send(this->connection->get_socketDesc(), &this->password, sizeof(this->password), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Registration: PASSWORD SENT" << endl;
}

//wait for the registration result from the server
void Registration::wait_registration_response() {
  int recv_bytes;
  while ((recv_bytes = recv(this->connection->get_socketDesc(), &this->registrated, sizeof(this->registrated), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot read from socket");
  }
  if (recv_bytes == 0) return;
  if (DEBUG) cerr << "Registration: RESPONSE RECEIVED" << endl;
}


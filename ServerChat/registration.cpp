#include <arpa/inet.h>
#include <errno.h>
#include <sstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "common.h"
#include "registration.h"
#include "userdatabase.h"

//constructor
Registration::Registration(int socketDesc) {
  this->socketDesc = socketDesc;
  this->db = new UserDatabase();
  this->registrated = NOT_YET_REGISTRATED;
}

//get resistration status
int Registration::get_registration_status() {
  return this->registrated;
}

//receive registration data from the client
void Registration::receive_registration_data() {
  int recv_bytes;

  while ((recv_bytes = recv(this->socketDesc, &this->username, sizeof(username), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot read from socket");
  }
  if (recv_bytes == 0) return;
  if (DEBUG) cerr << "Registration: USERNAME RECEIVED" << endl;

  while ((recv_bytes = recv(this->socketDesc, &this->password, sizeof(password), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot read from socket");
  }
  if (recv_bytes == 0) return;
  if (DEBUG) cerr << "Registration: PASSWORD RECEIVED" << endl;

}

//check if data received from the client were not already in the database
//(user already registrated)
void Registration::check_registration_data() {
  if (this->db->check_user(this->username)) this->registrated = ALREADY_REGISTRATED;
  if (DEBUG) cerr << "Registration: DATA CHECKED" << endl;
}

//create new user with given username and password (registration core)
void Registration::create_new_user() {
  this->db->create_new_entry(this->username, this->password);
  this->registrated = REGISTRATED;
  if (DEBUG) cerr << "Registration: NEW USER REGISTRATED" << endl;
}

//send the result of the registration to the client
void Registration::send_response() {
  while ((send(this->socketDesc, &this->registrated, sizeof(this->registrated), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Registration: RESPONSE SENT" << endl;
}

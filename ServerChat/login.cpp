#include <arpa/inet.h>
#include <errno.h>
#include <sstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "common.h"
#include "login.h"
#include "userdatabase.h"

//constructor
Login::Login(int socketDesc) {
  this->socketDesc = socketDesc;
  this->db = new UserDatabase();
  this->logged = false;
}

//receive login data from the client
void Login::receive_login_data() {
  int recv_bytes;

  while ((recv_bytes = recv(this->socketDesc, &this->username, sizeof(username), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot read from socket");
  }
  if (recv_bytes == 0) return;
  if (DEBUG) cerr << "Login: USERNAME RECEIVED" << endl;

  while ((recv_bytes = recv(this->socketDesc, &this->password, sizeof(password), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot read from socket");
  }
  if (recv_bytes == 0) return;
  if (DEBUG) cerr << "Login: PASSWORD RECEIVED" << endl;
}

//check if login data received from the client were right
void Login::check_login_data() {
  if (this->db->check_credentials(this->username, this->password)) this->logged = true;
  if (DEBUG) cerr << "Login: LOGIN DATA CHECKED" << endl;
}

//send the result of the login to the client
void Login::send_response() {
  while ((send(this->socketDesc, &this->logged, sizeof(this->logged), 0)) < 0) {
    if (errno == EINTR) continue;
    handle_error("Cannot write on socket");
  }
  if (DEBUG) cerr << "Login: RESPONSE SENT" << endl;
}

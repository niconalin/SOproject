#pragma once

#include <iostream>
using namespace std;

class UserDatabase;

class Login
{
public:
  Login(int socketDesc);
  void receive_login_data();
  void check_login_data();
  void send_response();
 private:
  int socketDesc;
  UserDatabase *db;
  string username;
  string password;
  bool logged;
};

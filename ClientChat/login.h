#pragma once

#include <iostream>
using namespace std;

class Connection;

class Login
{
  public:
    Login(Connection* connection, string username, string password);
    bool get_login_status();
    void send_login_data();
    void wait_login_response();
  private:
    Connection* connection;
    string username;
    string password;
    bool logged;
};

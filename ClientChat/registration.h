#pragma once

#include <iostream>
using namespace std;

enum Status {REGISTRATED, ALREADY_REGISTRATED, NOT_YET_REGISTRATED};

class Connection;

class Registration
{
public:
  Registration(Connection* connection, string username, string password);
  int get_registration_status();
  void send_registration_data();
  void wait_registration_response();
private:
  Connection* connection;
  string username;
  string password;
  int registrated;
};

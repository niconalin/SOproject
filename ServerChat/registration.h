#pragma once

#include <iostream>
using namespace std;

enum Status {REGISTRATED, ALREADY_REGISTRATED, NOT_YET_REGISTRATED};

class UserDatabase;

class Registration
{
public:
  Registration(int socketDesc);
  int get_registration_status();
  void receive_registration_data();
  void check_registration_data();
  void create_new_user();
  void send_response();
 private:
  int socketDesc;
  UserDatabase *db;
  string username;
  string password;
  int registrated;
};

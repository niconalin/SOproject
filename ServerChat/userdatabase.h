#pragma once

#include <fstream>
using namespace std;

const string USER_DATABASE_FILENAME = "/Users/niconalin/Documents/QTProjects/ServerChat/Database/users.txt";

class UserDatabase
{
public:
  UserDatabase();
  vector<string> get_credentials(string *line);
  int check_credentials(string username, string password);
  int check_user(string username);
  int create_new_entry(string username, string password);
private:
  int id;
};

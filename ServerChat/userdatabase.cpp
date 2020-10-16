#include <iostream>
#include <sstream>
#include <vector>

#include "common.h"
#include "filereader.h"
#include "filewriter.h"
#include "userdatabase.h"

//constructor
UserDatabase::UserDatabase()
{}

//parse line read from db to obtain the credentials
vector<string> UserDatabase::get_credentials(string *line) {

  stringstream ss(*line);
  vector<string> credentials;
  string temp;

  while(getline(ss, temp, ',')){
    credentials.push_back(temp);
  }

  return credentials;
}

//check if user provide right username and password and grant access)
int UserDatabase::check_credentials(string username, string password) {
  FileReader *fr = new FileReader();
  fr->open(USER_DATABASE_FILENAME);

  string line;
  int i = 0;
  while (fr->read_line(&line)) {
    vector<string> credentials = get_credentials(&line);
    if(!credentials.at(0).compare(username)) {
        if (!credentials.at(1).compare(password)) {
            fr->close();
            this->id = ++i;
            return 1;
          }
    }
    ++i;
  }

  fr->close();
  this->id = ++i;
  return 0;
}

//check if the username given for registration is not already used
int UserDatabase::check_user(string username) {
  FileReader *fr = new FileReader();
  fr->open(USER_DATABASE_FILENAME);

  string line;
  int i = 0;
  while (fr->read_line(&line)) {
    vector<string> credentials = get_credentials(&line);
    if(!credentials.at(0).compare(username)) {
      fr->close();
      this->id = ++i;
      return 1;
    }
    ++i;
  }

  fr->close();
  this->id = ++i;
  return 0;
}

//add new entry (user) to the database
int UserDatabase::create_new_entry(string username, string password) {
  FileWriter *fw = new FileWriter();
  fw->open(USER_DATABASE_FILENAME);

  string inputLine = username + "," + password + "," + to_string(this->id);
  fw->write_line(&inputLine);

  fw->close();
  return 0;
}


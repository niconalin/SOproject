#include "common.h"
#include "filereader.h"

#include <iostream>

//constructor
FileReader::FileReader()
{}

//open file in read mode
void FileReader::open(string fileName) {
  this->file.open(fileName.c_str());
  if (this->file.is_open()) {
    if (DEBUG) cerr << "FileReader: FILE OPENED" << endl;
  }
  else
    if (DEBUG) cerr << "FileReader: FAILED TO OPEN FILE" << endl;
}

//close file
void FileReader::close() {
  this->file.close();
  if (this->file.is_open()) {
    if (DEBUG) cerr << "FileReader: FAILED TO CLOSE FILE" << endl;
  }
  else
    if (DEBUG) cerr << "FileReader: FILE CLOSED" << endl;
}

//read a single line from file
bool FileReader::read_line(string *outputLine) {
  getline(this->file, *outputLine);

  if (DEBUG) cerr << "FileReader: LINE READ" << endl;

  if(this->file.eof()) return false;
  return true;
}


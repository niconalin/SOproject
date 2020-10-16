#include "common.h"
#include "filewriter.h"

#include <iostream>

//constructor
FileWriter::FileWriter()
{}

//open file in write mode
void FileWriter::open(string fileName) {
  this->file.open(fileName, fstream::app);
  if (this->file.is_open()) {
    if (DEBUG) cerr << "FileWriter: FILE OPENED" << endl;
  }
  else
    if (DEBUG) cerr << "FileWriter: FAILED TO OPEN FILE" << endl;
}

//close file
void FileWriter::close() {
  this->file.close();
  if (this->file.is_open()) {
    if (DEBUG) cerr << "FileWriter: FAILED TO CLOSE FILE" << endl;
  }
  else
    if (DEBUG) cerr << "FileWriter: FILE CLOSED" << endl;
}

//write the given line at the end of the file
void FileWriter::write_line(string *inputLine) {
  this->file << *inputLine << endl;
  if (DEBUG) cerr << "FileWriter: LINE WRITTEN" << endl;
}

#pragma once

#include <fstream>
using namespace std;

class FileWriter
{
public:
  FileWriter();
  void open(string fileName);
  void close();
  void write_line(string *inputLine);
private:
  ofstream file;
};

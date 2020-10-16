#pragma once

#include <fstream>
using namespace std;

class FileReader
{
public:
  FileReader();
  void open(string fileName);
  void close();
  bool read_line(string *outputLine);
private:
  ifstream file;
};

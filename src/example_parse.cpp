#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ini.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "usage: readini <config.ini>" << endl;
    exit(2);
  }
  try {
    ini::config cfg(argv[1]);
    vector<ini::option> options = cfg.all();
    for(int i = 0; i < options.size(); i++) {
      ini::option o = options[i];
      cout << o.name << ":" << o.section << ": " << o.value << endl;
    }
  } catch(exception& e) {
    cerr << "unexpected error: " << e.what() << endl;
    return 1;
  }
}

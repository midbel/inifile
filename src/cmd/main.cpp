#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scan.h"
#include "ini.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "invalid number of arguments" << endl;
    return 2;
  }
  ini::scanner scan(argv[1]);
  for(int i = 0; !scan.done(); i++) {
    try {
      ini::token tok = scan.scan();
      cout << tok.pos.to_string() << ") ";
      cout << (i+1) << ": " << tok.literal << ", type: " << ini::to_string(tok.type) << endl;
    } catch (ini::scanner_exception e){
      cerr << "error while scanning: " << e.what() << endl;
    } catch (const char* e) {
      cerr << "unexpected error: " << e << endl;
      return 3;
    }
  }
}

// int main(int argc, char** argv) {
//   if (argc != 2) {
//     cerr << "usage: readini <config.ini>" << endl;
//     exit(2);
//   }
//   try {
//     ini::config cfg(argv[1]);
//     vector<ini::option> options = cfg.all();
//     for(int i = 0; i < options.size(); i++) {
//       ini::option o = options[i];
//       cout << o.name << ":" << o.section << ": " << o.value << endl;
//     }
//     cout << "section1:host = " << cfg.get_string("section1", "host") << endl;
//     cout << "section1:addr = " << cfg.get_string("section1", "addr") << endl;
//     cout << "section3:addr = " << cfg.get_string("section3", "addr") << endl;
//
//     try {
//       cfg.get_string("hello", "world");
//     }
//     catch(ini::not_found nf) {
//       cerr << "not found ok " << string(nf.what()) << endl;
//     }
//   } catch(exception& e) {
//     cerr << "unexpected error: " << e.what() << endl;
//     return 1;
//   }
// }

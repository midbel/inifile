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

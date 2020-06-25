#ifndef INI_H
#define INI_H

#include <exception>
#include <vector>
#include <sstream>
#include "scan.h"
#include "ini.h"

namespace ini {

  class duplicate: public std::exception {
  private:
    std::string label;
    std::string part;
  public:
    duplicate(std::string a, std::string p): label(a), part(p) {}
    virtual ~duplicate() throw() {}
    virtual const char* what() const throw() {
      std::ostringstream str;
      str << "duplicate " << part << ": " << label;
      return str.str().c_str();
    }
  };

  class not_found: public std::exception {
  private:
    std::string label;
    std::string section;
  public:
    not_found(std::string s, std::string a): section(s), label(a) {}
    virtual ~not_found() throw() {}
    virtual const char* what() const throw() {
      std::ostringstream str;
      str << section << ": option " << label << " not found";
      return str.str().c_str();
    }
  };

  class unexpected_token: public std::exception {
  private:
    token_type want;
    token_type got;
    position pos;
  public:
    unexpected_token(position p, token_type w, token_type g): pos(p), want(w), got(g) {}
    virtual ~unexpected_token() throw() {}
    virtual const char* what() const throw() {
      std::ostringstream str;
      str << ": unexpected token! want " << to_string(want);
      str << ", got " << to_string(got);
      return str.str().c_str();
    }
  };

  struct option {
    std::string section;
    std::string name;
    std::string value;
  };

  class config {
  private:
    std::vector<option> options;
    std::vector<option> parse(std::string file);
    option get_option(std::string section, std::string name);

  public:
    config(std::string file);

    std::string get_string(std::string option);
    std::string get_string(std::string section, std::string option);
    int get_int(std::string option);
    int get_int(std::string section, std::string option);
    double get_double(std::string option);
    double get_double(std::string section, std::string option);
    bool get_bool(std::string option);
    bool get_bool(std::string section, std::string option);

    bool has_section(std::string section);
    bool has_option(std::string section, std::string name);

    std::vector<option> all() {
      return options;
    }
  };
}

#endif // INI_H

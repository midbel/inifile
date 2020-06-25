#include <vector>
#include <set>
#include <iostream>
#include "scan.h"
#include "ini.h"


namespace ini {

  class parser {
  private:
    token curr;
    token peek;
    scanner scan;

  public:
    parser(std::string file): scan(file) {
        next();
        next();

    };
    std::vector<option> parse_ini();
  private:
    std::vector<option> parse_section(std::string label);
    option parse_option(std::string label);
    void next();
  };

  config::config(std::string file) {
    options = parse(file);
  }

  std::string config::get_string(std::string section, std::string name) {
    option o = get_option(section, name);
    return o.value;
  }

  std::string config::get_string(std::string name) {
    return get_string("", name);
  }

  int config::get_int(std::string section, std::string name) {
    option o = get_option(section, name);
    return std::stoi(o.value);
  }

  int config::get_int(std::string name) {
    return get_int("", name);
  }

  double config::get_double(std::string section, std::string name) {
    option o = get_option(section, name);
    return std::stod(o.value);
  }

  double config::get_double(std::string name) {
    return get_double("", name);
  }

  bool config::get_bool(std::string section, std::string name) {
    option o = get_option(section, name);
    if (o.value == "" || o.value == "0" || o.value == "false") {
      return false;
    }
    return true;
  }

  bool config::get_bool(std::string name) {
    return get_bool("", name);
  }

  bool config::has_section(std::string section) {
    for (int i = 0; i < options.size(); i++) {
      option o = options[i];
      if (o.section == section) {
        return true;
      }
    }
    return false;
  }

  bool config::has_option(std::string section, std::string name) {
    for (int i = 0; i < options.size(); i++) {
      option o = options[i];
      if (o.section == section && o.name == name) {
        return true;
      }
    }
    return false;
  }

  option config::get_option(std::string section, std::string name) {
    for (int i = 0; i < options.size(); i++) {
      option o = options[i];
      if (o.section == section && o.name == name) {
        return o;
      }
    }
    throw not_found(section, name);
  }

  std::vector<option> config::parse(std::string file) {
    std::vector<option> options;
    parser ps(file);
    return ps.parse_ini();
  }

  std::vector<option> parser::parse_ini() {
    std::vector<option> options;
    std::set<std::string> sections;
    if (curr.type == ident) {
      std::vector<option> os = parse_section("default");
      options.insert(options.end(), os.begin(), os.end());
    }
    while(!scan.done()) {
      if (curr.type != header) {
        throw unexpected_token(curr.pos, header, curr.type);
      }
      std::string label = curr.literal;
      if (sections.find(label) != sections.end()) {
        throw duplicate(label, "section");
      }
      next();
      std::vector<option> os = parse_section(label);
      options.insert(options.end(), os.begin(), os.end());
    }
    return options;
  }

  std::vector<option> parser::parse_section(std::string label) {
    std::vector<option> options;
    std::set<std::string> labels;
    while (curr.type != header && !scan.done()) {
      option opt = parse_option(label);
      if (labels.find(opt.name) != labels.end()) {
        throw duplicate(opt.name, "option");
      }
      options.push_back(opt);
      next();
    }
    return options;
  }

  option parser::parse_option(std::string section) {
    option opt;
    if (curr.type != ident) {
      throw unexpected_token(curr.pos, ident, curr.type);
    }
    opt.section = section;
    opt.name = curr.literal;
    next();
    if (curr.type != assign) {
      throw unexpected_token(curr.pos, assign, curr.type);
    }
    next();
    if (curr.type != literal) {
      throw unexpected_token(curr.pos, literal, curr.type);
    }
    opt.value = curr.literal;

    return opt;
  }

  void parser::next() {
    curr = peek;
    peek = scan.scan();
    while (peek.type == comment) {
      peek = scan.scan();
    }
  };
}

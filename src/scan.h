#ifndef SCAN_H
#define SCAN_H

#include <exception>
#include <fstream>
#include <sstream>

namespace ini {
  namespace mk {
    const char space = ' ';
    const char tab = '\t';
    const char pound = '#';
    const char lsquare = '[';
    const char rsquare = ']';
    const char newline = '\n';
    const char equal = '=';
    const char hyphen = '-';
    const char underscore = '_';
    const char carriage = '\r';
    const char dquote = '"';
    const char squote = '\'';
    const char semicolon = ';';

    inline bool is_blank(char c) {
      return c == space || c == tab;
    }

    inline bool is_newline(char c) {
      return c == newline;
    }

    inline bool is_letter(char c) {
      return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    inline bool is_digit(char c) {
      return c >= '0' && c <= '9';
    }

    inline bool is_alpha(char c) {
      return is_digit(c) || is_letter(c);
    }

    inline bool is_ident(char c) {
      return is_alpha(c) || c == hyphen || c == underscore;
    }

    inline bool is_comment(char c) {
      return c == pound || c == semicolon;
    }
  }

  enum token_type {eof, header, ident, literal, number, boolean, assign, comment};

  inline std::string to_string(token_type tt) {
    switch (tt) {
      case eof: return "eof";
      case header: return "header";
      case ident: return "ident";
      case number: return "number";
      case boolean: return "boolean";
      case literal: return "literal";
      case comment: return "comment";
      case assign: return "assign";
      default: return "unknown";
    }
  }

  struct position {
    int line;
    int column;
    std::string to_string() {
      std::ostringstream str;
      str << line;
      str << ":";
      str << column;

      return str.str();
    }
    bool is_valid() {
      return line > 0;
    }
  };

  struct token {
    token_type type;
    std::string literal;
    position pos;
  };

  class scanner_exception: public std::exception {
  private:
    std::string where;
    std::string reason;
  public:
    scanner_exception(std::string w, std::string r): where(w), reason(r) {}
    virtual ~scanner_exception() throw() {}
    virtual const char* what() const throw() {
      std::ostringstream str;
      str << "scanning " << where << ": " << reason << "!!!";
      return str.str().c_str();
    }
  };

  class scanner {
  public:
    scanner(std::string file);
    token scan();
    bool done();
    position where();

  private:
    std::stringstream buffer;
    int line;
    int column;
    int length;

    char last;
    bool value;

    void scan_header(token& tok);
    void scan_ident(token& tok);
    void scan_value(token& tok);
    void scan_quote(token& tok, char quote);
    void scan_comment(token& tok);

    char get();
    char peek();
    void unget();

    void skip_lines();
    void skip_blanks();
  };
}

#endif // SCAN_H

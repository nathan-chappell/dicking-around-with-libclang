//parse_options.h
#pragma once

#include <list>
#include <string>

std::string newlines_on_size(const std::string &str, size_t width = 80);

struct Options {
  bool recurse;
  std::list<std::string> chosen_attributes;
  std::string source;
  size_t line;
  size_t col;

  Options();
  std::string dump() const;
  static std::string help(const std::string &name);
};

struct OptionTriple {
  std::string short_opt;
  std::string long_opt;
  std::string message;

  OptionTriple(const std::string &s, const std::string &l, const std::string &m);
};

std::string get_initial_opt(const OptionTriple &triple);
std::list<std::string> get_message_split(const OptionTriple &triple);
std::string get_key_from_option(const std::string &option);
void add_opt_line(std::string &help, const OptionTriple &triple);
bool parse_options(Options &options, const std::list<std::string>&, int argc, char *argv[]);


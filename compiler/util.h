#pragma once
#include <string>
#include <vector>
#include <stdexcept>

struct CompileError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

static inline bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static inline bool is_digit(char c) {
  return (c >= '0' && c <= '9');
}
static inline bool is_alnum(char c) {
  return is_alpha(c) || is_digit(c);
}

std::string read_file(const std::string& path);

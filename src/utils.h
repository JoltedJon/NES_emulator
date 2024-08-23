#pragma once

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

class NotImplemented : public std::logic_error {
 private:
  std::string _text;

  NotImplemented(const char* message, const char* file, int line,
                 const char* function)
      : std::logic_error("Not Implemented") {
    _text += file;
    _text += ":";
    _text += std::to_string(line).c_str();
    _text += " ";
    _text += function;
    _text += " : ";
    _text = message;
  };

 public:
  NotImplemented()
      : NotImplemented("Not Implemented", __FILE__, __LINE__, __FUNCTION__) {}

  NotImplemented(const char* message)
      : NotImplemented(message, __FILE__, __LINE__, __FUNCTION__) {}

  const char* what() const throw() { return _text.c_str(); }
};

inline void __info(const char* message) { std::cerr << message << std::endl; }
inline void __warning(const char* message) {
  std::cerr << "\033[33m" << message << "\033[0m" << std::endl;
}
inline void __error(const char* file, size_t line, const char* message) {
  std::cerr << "\033[31m" << file << ":" << line << "\t" << message << "\033[0m"
            << std::endl;
  abort();
}

#define info(message) __info(message)
#define warning(message) __warning(message)
#define error(message) __error(__FILE__, __LINE__, message)

template <typename First, typename... T>
bool is_in(First&& first, T&&... t) {
  return ((first == t) || ...);
}

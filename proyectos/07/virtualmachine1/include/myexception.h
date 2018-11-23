/*Code taken from:
 https://stackoverflow.com/questions/17438863/c-exceptions-with-message/17438895 
*/
#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H

#include <string>

class MyException: public std::exception {
 private:
  std::string message_;
 public:
  explicit MyException(const std::string& message);
  
  virtual const char* what() const throw() {
    return message_.c_str();
  }
};

MyException::MyException(const std::string& message) : message_(message) {}

#endif

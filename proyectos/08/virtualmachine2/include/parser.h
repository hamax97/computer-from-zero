#ifndef PARSER_H
#define PARSER_H

#include "commandtype.h"
#include <fstream>
#include <string>

using std::ifstream;
using std::string;

struct Messages{
  string error1 = "Error openning the file.\n";
  string endOfFile = "END OF FILE.\n";
};

class Parser{
 private:
  /* Reperesents the file to be analized and parsed */
  ifstream fileToParse;

  /* Represents the current command to be analized */
  string currentCommand;
  
  /* number of characters in the command */
  int instructionSize;
	 
 public:
  /* constructor that initializes de file to be parsed */
  Parser(string& file);
  
  /* destructor of the object that also closes the file */
  ~Parser();
  
  /* read the next command from the input and makes it the current command */
  bool advance();
  
  /* return the type of the current command: C_ARITHMETIC, C_PUSH, ... */
  CommandType getCommandType() const;

  /* returns the first argument of the current command */
  string getArg1(CommandType& type);

  /* return the second argument of the current command */
  int getArg2() const;
}; 

#endif


#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>

using std::ifstream;
using std::string;

/* Enumeration that represents the type of command */
enum CommandType{
  A_COMMAND = 0, L_COMMAND = 1, C_COMMAND = 2
};

struct Messages{
  string error1 = "Error openning the file.\n";
  string error2 = "Syntax error: Expected: @ | ( | D | A | M | 0 | - | ! | '|' | &. Found: ";
  string error3 = "Syntax error: Expected: JGT | JEQ | JGE | JLT | JNE | JLE | JMP. Found: ";
  string error4 = "Syntax error: Expected: ). Found: ";
  string error5 = "Syntax error: Expected: A | D | M | 0 | 1 | + | - | ! | & | '|'. Found: ";
  string error6 = "Syntax error: Expected: A | D | M. Or the destiny already have the destiny. Found: ";
  string error7 = "Invalid computation: Found: ";
  string endOfFile = "END OF FILE.\n";
};

class Parser{
 private:
  /* Reperesents the file to be analized and parsed */
  ifstream fileToParse;
  /* contains the current command to parse */
  string currentCommand;
  /* number of characters in the command */
  int instructionSize;
	 
 public:
  /* constructor that initializes de file to be parsed */
  Parser(string& file);
  /* destructor of the object that closes the file */
  ~Parser();
  /* read the next command from the input and makes it the current command */
  bool advance();
  /* return the type of the command: A(0), C(1), L(2) command */
  CommandType getCommandType() const;
  /* returns the symbol or decimal of the current command */
  string symbol(CommandType& aOrL) const;
  /* returns the destiny in the current C-command */
  string dest() const;
  /* returns the instruction to compute in the current C-command */
  string comp() const;
  /* returns the jump to execute in the current C-command */
  string jump() const;
  /* function to back to the top of the file */
  void backToTop();
};

#endif


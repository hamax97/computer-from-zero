#include "../include/parser.h"
#include "../include/myexception.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::stringstream;

Messages messages;

Parser::Parser(string& file){
  try{
    fileToParse.open(file);
    if(fileToParse.is_open())
      cout << "File correctly opened" << endl;
    else
      throw MyException(messages.error1);
  }
  catch(MyException& e){
    cerr << e.what() << endl;
  }
}

Parser::~Parser(){
  fileToParse.close();
}

bool Parser::advance(){
  
  currentCommand.erase(); //restores the current command to ""

  bool commentFound = false;
  bool instructionFound = false;
  char currentChar;
  while(fileToParse.get(currentChar)){
    if(currentChar == '\r'){ //Windows eol '\r\n'
      fileToParse.get(currentChar);
      if(currentChar == '\n'){
	// if reached eol and the line is not a comment is a valid line
	if(instructionFound){
	  instructionSize = currentCommand.size();
	  return true;
	}else{
	  commentFound = false;
	  continue;
	}
      }
    }
    else if(currentChar == '\n'){
      // if reached eol and the line is not a comment is a valid line
      if(instructionFound){
	instructionSize = currentCommand.size();
	return true;
      }else{
	commentFound = false;
	continue;
      }
    }
    else if(commentFound) //if the current line is a comment just continue until eol
      continue;
    else if(currentChar == '/'){
      commentFound = true;
      continue;
    }
    else{
      instructionFound = true;

      //if the current character is an space and the next character is an space too
      //dont add the character to the command
      if((currentChar == ' ' || currentChar == '\t') && fileToParse.get(currentChar))
	if(currentChar == ' ' || currentChar == '\t' || currentChar == '/')
	  continue;
	else{
	  currentCommand.push_back(' ');
	  currentCommand.push_back(currentChar);
	  continue;
	}
      
      currentCommand.push_back(currentChar);
    }
  }

  return false;
}

CommandType Parser::getCommandType() const{

  CommandType t;
  switch (currentCommand[0]){
  case 'p':
    if(currentCommand[1] == 'u')
      t = C_PUSH;
    else
      t = C_POP;
    break;
  case 'a':
    t = C_ARITHMETIC;
    break;
  case 's':
    t = C_ARITHMETIC;
    break;
  case 'n':
    t = C_ARITHMETIC;
    break;
  case 'e':
    t = C_ARITHMETIC;
    break;
  case 'g':
    if(currentCommand[1] == 't')
      t = C_ARITHMETIC;
    else
      t = C_GOTO;
    break;
  case 'l':
    if(currentCommand[1] == 't')
      t = C_ARITHMETIC;
    else
      t = C_LABEL;
    break;
  case 'o':
    t = C_ARITHMETIC;
    break;
  case 'i':
    t = C_IF;
    break;
  case 'f':
    t = C_FUNCTION;
    break;
  case 'c':
    t = C_CALL;
    break;
  case 'r':
    t = C_RETURN;
    break;
  }

  return t;
}

string Parser::getArg1(CommandType& t){
  if(t == C_ARITHMETIC){
    //return itself
    return currentCommand;
  }else{ //Any other type of command
    stringstream command(currentCommand);
    string arg2;

    command >> arg2;
    command >> arg2;
    return arg2;
  }
}

int Parser::getArg2() const{
  for(int i = instructionSize - 1; i >= 0; --i)
    if(currentCommand[i] == ' ' || currentCommand[i] == '\t')
      return std::stoi(currentCommand.substr(i+1, instructionSize - i));
    
  
  return -777; //it never gonna happen
}

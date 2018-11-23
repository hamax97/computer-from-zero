#include "../include/parser.h"
#include "../include/myexception.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::ios;

Messages messages;

Parser::Parser(string& file){
  try{
    this->fileToParse.open(file);
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
  this->fileToParse.close();
}

bool Parser::advance(){
  
  currentCommand.erase(); //restores the current command to ""

  bool commentFound = false;
  char currentChar;
  while(fileToParse.get(currentChar)){
    
    if(currentChar == '\r'){ //Windows eol '\r\n'
      fileToParse.get(currentChar);
      if(currentChar == '\n'){
	// if reached eol and the line is not a comment is a valid line
	if(!commentFound){
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
      if(!commentFound){
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
    else
      currentCommand.push_back(currentChar);
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
    //cout << "found push or pop command type in getArg1" << endl;
    int positionSaved;
    bool foundSecondArgument = false;
    for(int i = 0; i < instructionSize; ++i){
      //cout << currentCommand[i];
      if(foundSecondArgument && (currentCommand[i] == ' ' || currentCommand[i] == '\t' || currentCommand[i] == '\n')){
	return currentCommand.substr(positionSaved, i - positionSaved);
      }
      if(currentCommand[i] == ' ' || currentCommand[i] == '\t'){
	foundSecondArgument = true;
	positionSaved = i+1;
      }
    }
  }
  return "error"; //it never gonna happen
}

int Parser::getArg2() const{
  for(int i = instructionSize - 1; i >= 0; --i)
    if(currentCommand[i] == ' ' || currentCommand[i] == '\t')
      return std::stoi(currentCommand.substr(i, instructionSize - 1));
  
  return -777; //it never gonna happen
}

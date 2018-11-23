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

/* if the line readed from the string is valid return true */
bool isValidLine(string& line){
  if(line[0] == '@' || line[0] == '(' || line[0] == 'M' ||
     line[0] == 'D' || line[0] == 'A' || line[0] == '0' ||
     line[0] == '1' || line[0] == '-' || line[0] == '!')
    return true;
  return false;
}

bool Parser::advance(){
  currentCommand.erase();

  bool commentFound = false;
  char currentChar;
  while(fileToParse.get(currentChar)){
    if(currentChar == '\r'){ //Windows eol '\r\n'
      fileToParse.get(currentChar);
      if(currentChar == '\n'){
	commentFound = false;
	if(isValidLine(currentCommand)){
	  instructionSize = currentCommand.size();
	  return true;
	}
	else continue;
      }
    }
    else if(currentChar == '\n'){
      commentFound = false;
      if(isValidLine(currentCommand)){
	instructionSize = currentCommand.size();
	return true;
      }
      else continue;
    }else if(commentFound)
      continue;
    else if(currentChar == '/'){
      commentFound = true;
      if(isValidLine(currentCommand)){
	instructionSize = currentCommand.size();
	return true;
      }
      else continue;
    }
    else if(currentChar == ' ' || currentChar == '\t')
      continue;
    else
      currentCommand.push_back(currentChar);
  }
  return false;
}

CommandType Parser::getCommandType() const{
  CommandType t;
  try{
    switch (currentCommand[0]){
    case '@':
      t = A_COMMAND;
      break;
    case '(':
      t = L_COMMAND;
      break;
    case 'D':
      t = C_COMMAND;
      break;
    case 'M':
      t = C_COMMAND;
      break;
    case 'A':
      t = C_COMMAND;
      break;
    case '0':
      t = C_COMMAND;
      break;
    case '1':
      t = C_COMMAND;
      break;
    case '-':
      t = C_COMMAND;
      break;
    case '!':
      t = C_COMMAND;
      break;
    default:
      throw MyException(messages.error2 + currentCommand[0]);
  }
  }catch(MyException& e){
    cerr << e.what() << endl;
  }
  return t;
}

string Parser::symbol(CommandType& AL) const{
  string sym;

  /* if A command or L command just gets the symbol */
  switch (AL){
  case 0: //A command @xxx
    sym = currentCommand.substr(1, instructionSize - 1);
    break;
    
  case 1: //L command (xxx)
    try{
      if(currentCommand[instructionSize - 1] != ')')
	throw MyException(messages.error4 + currentCommand[instructionSize - 1]);
      else
	sym = currentCommand.substr(1, instructionSize - 2);
    }catch(MyException& e){
      cerr << e.what() << endl;
    }
    break;
  }
  return sym;
}

/* return true if the character is valid destiny character */
bool validChar(char& c){
  if(c == 'A' || c == 'D' || c == 'M') return true;
  else return false;
}

/* return true is the string 'destiny' has the char 'element' */
bool contains(string& destiny, char& element){
  for(int i = 0; i < destiny.size(); ++i)
    if(destiny[i] == element)
      return true;
  return false;
}

string Parser::dest() const{
  string destiny;
  bool destinyFound = false;

  for(int i = 0; i < instructionSize; ++i){
    if(currentCommand[i] == '='){
      destinyFound = true;
      break;
    }
  }
  if(destinyFound){
    try{
      /* travel through the first 4 characters of the command 
	 if it doesnt find an equals sign is because there is no destiny */
      for(int i = 0; i < 4; ++i){
	char aux = currentCommand[i];
	if(currentCommand[i] == '='){
	  destinyFound = true;
	  break;
	}
	if(validChar(aux) && !contains(destiny, aux))
	  destiny.push_back(currentCommand[i]);
	else throw MyException(messages.error6 + currentCommand[i]);
      }
    }catch(MyException& e){
      cerr << e.what() << endl;
    }
    return destiny;
  }else{
    destiny = "null";
    return destiny;
  }
}

/* reverse a string passed by reference */
void reverse(string& s){
  char aux;
  int size = s.size();
  for(int i = 0; i < size/2; ++i){
    aux = s[i];
    s[i] = s[size - (i+1)];
    s[size - 1] = aux;
  }
}

/* Return true if the computation as parameter is a valid computation */
bool validComputation(string& computation){
  int size = computation.size();
  if(size == 1){ //computations of size 1
    if(computation[0] == '1' || computation[0] == '0' || computation[0] == 'A' || computation[0] == 'D' || computation[0] == 'M') return true;
  }else if(size == 2){ //computations of size 2
    if(computation[0] == '-'){
      if(computation[1] == '1' || computation[1] == 'D' || computation[1] == 'A' || computation[1] == 'M')
	return true;
      else return false;
    }else if(computation[0] == '!'){
      if(computation[1] == 'D' || computation[1] == 'A' || computation[1] == 'M')
	return true;
      else return false;
    }
  }else if(size == 3){ //computations of size 3
    if(computation[0] == 'D'){
      if(computation[1] == '+' || computation[1] == '-' || computation[1] == '&' || computation[1] == '|')
	if(computation[2] == '1' || computation[2] == 'A' || computation[2] == 'M')
	  return true;
	else return false;
      else return false;
    }else if(computation[0] == 'A'){
      if(computation[1] == '+' && computation[2] == '1') return true;
      else if(computation[1] == '-' && (computation[2] == 'D' || computation[2] == '1')) return true;
      else return false;
    }else if(computation[0] == 'M'){
      if(computation[1] == '+' && computation[2] == '1') return true;
      else if(computation[1] == '-' && (computation[2] == 'D' || computation[2] == '1')) return true;
      else return false;
    }
  }
  return false;
}

string Parser::comp() const{
  string computation;
  bool hasJump = false;

  /* if the command size is grather than 5 then verifies if there's a jump */
  if(instructionSize >= 5)
    if(currentCommand[instructionSize - 4] == ';')
      hasJump = true;
    
  if(hasJump){
    /* travels the command but in reversed order */
    for(int i = 5; i <= instructionSize; ++i){
      char aux = currentCommand[instructionSize - i];
      if(aux == '=')
	break;
      else
	computation.push_back(aux);
    }
  }else{
    /* travels the command but in reversed order
       note that in only can go through 3 positions (A+1, A+D, ...) */
    for(int i = instructionSize - 1; i >= instructionSize - 3; --i){
      char aux = currentCommand[i];
      if(aux == '=')
	break;
      else
	computation.push_back(aux);
    }
  }
  /* reverse the order of the string to get the correct answer */
  reverse(computation);

  try{
    if(!validComputation(computation)) throw MyException(messages.error7 + computation);
  }catch(MyException& e){
    cerr << e.what() << endl;
  }
  return computation;
}


/* return true if the mnemonic of the jump is valid */
bool validJumpType(string& jump){
  if(jump == "null" || jump == "JGT" || jump == "JEQ" || jump == "JGE" ||
     jump == "JLT" || jump == "JNE" || jump == "JLE" || jump == "JMP") return true;
  else return false;
}

string Parser::jump() const{
  string jumpType;

  /* if the command size is grather than 5 then verifies if there's a jump */
  if(instructionSize >= 5)
    if(currentCommand[instructionSize - 4] == ';'){
      //so the last 3 characters of the string are equivalent to the jump
      jumpType = currentCommand.substr((instructionSize - 4)+1, 3);
    }else{ //no jump
      jumpType = "null";
    }
  else
    jumpType = "null";

  try{
    if(!validJumpType(jumpType)) throw MyException(messages.error3 + jumpType);
  }catch(MyException& e){
    cerr << e.what() << endl;
  }
  return jumpType;
}

void Parser::backToTop(){
  // code taken from:
  // http://www.cplusplus.com/forum/beginner/30644/
  try{
    fileToParse.clear();
    fileToParse.seekg(0, ios::beg);
  }catch(...){}
}

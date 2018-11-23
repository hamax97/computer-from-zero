#include "../include/parser.h"
#include "../include/code.h"
#include <iostream>
#include <unordered_map>

using std::cout;
using std::endl;
using std::cin;
using std::cerr;

int
main(int argc, char *argv[])
{
  //if there are a different number of arguments then error
  if(argc != 2){
    cerr << "Error introducing file to parse as program argument: " << argv[0] << "<file to parse>" << endl;
    return 1;
  }
  //argument 1 should be the file name
  string fileName(argv[1]);
  Parser parser(fileName);
  Code decodificator;
  CommandType commandType;

  // creating and naming output file
  decodificator.setFileName(fileName);

  while(parser.advance()){
    commandType = parser.getCommandType();
    
    if(commandType == C_ARITHMETIC){
      decodificator.writeArithmetic(parser.getArg1(commandType));
      
    }else{ // if(commandType == C_PUSH || commandType == C_POP)
      decodificator.writePushPop(commandType, parser.getArg1(commandType), parser.getArg2());
    }
  }
  decodificator.closeFile();
  return 0;
}

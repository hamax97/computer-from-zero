#include "../include/parser.h"
#include "../include/code.h"
#include <iostream>
#include <unordered_map>

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::ofstream;
using std::unordered_map;

/* return true if the character 'a' represents a digit (0..9) */
bool isDigit(char& a);
/* initializes the symbol table with the default values */
void initializeSymbolTable(unordered_map <string, int>& symbolTable);

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
  Code code;
  CommandType commandType;

  // creating and naming output file
  string nameFileOut;
  ofstream fileOut;
  if(fileName.size() < 5){
    cerr << "Wrong file name: " << fileName << endl;
  }else{
    nameFileOut = fileName.substr(0, fileName.size() - 4);
    nameFileOut += ".hack";
  }
  fileOut.open(nameFileOut);

  //hash map to represent the symbolTable
  unordered_map <string, int> symbolTable;
  initializeSymbolTable(symbolTable);

  int ROM = 0;
  while(parser.advance()){
    commandType = parser.getCommandType(); 
    if(commandType == A_COMMAND){
      //nothing in the first pass
      ++ROM;
    }else if(commandType == L_COMMAND){
      string symbolAux = parser.symbol(commandType);
      symbolTable[symbolAux] = ROM;
    }else if(commandType == C_COMMAND){ // c_COMMAND
      //nothing in the first pass
      ++ROM;
    }
  }
  
  parser.backToTop();
  int symCounter = 0; //variable that counts the number of A-command symbols
  while(parser.advance()){
    commandType = parser.getCommandType();
    
    if(commandType == A_COMMAND){
      string symbolAux = parser.symbol(commandType);
      if(isDigit(symbolAux[0])){
	int aReg = std::stoi(symbolAux);
	fileOut << code.intToBinary(aReg) << endl;
      }else{

	//iterator to look for the symbol found
	unordered_map<string, int>::const_iterator search = symbolTable.find(symbolAux);
	if(search == symbolTable.end()){ //if the symbol table has not the symbol then adds it in a new direction
	  symbolTable[symbolAux] = 16 + symCounter;
	  ++symCounter;
	}
	
	int aReg = symbolTable[symbolAux];
	fileOut << code.intToBinary(aReg) << endl;
      }
    }else if(commandType == C_COMMAND){ // c_COMMAND
      fileOut << "111" << code.transComp(parser.comp())
	      << code.transDest(parser.dest())
	      << code.transJump(parser.jump()) << endl;
    }
  }

  fileOut.close();
  return 0;
}

bool isDigit(char& a){
  if(a == '0' || a == '1' || a == '2' || a == '3' ||
     a == '4' || a == '5' || a == '6' || a == '7' ||
     a == '8' || a == '9')
    return true;
  else return false;
}

void initializeSymbolTable(unordered_map <string, int>& symbolTable){
  symbolTable["SP"] = 0;
  symbolTable["LCL"] = 1;
  symbolTable["ARG"] = 2;
  symbolTable["THIS"] = 3;
  symbolTable["THAT"] = 4;
  symbolTable["R0"] = 0;
  symbolTable["R1"] = 1;
  symbolTable["R2"] = 2;
  symbolTable["R3"] = 3;
  symbolTable["R4"] = 4;
  symbolTable["R5"] = 5;
  symbolTable["R6"] = 6;
  symbolTable["R7"] = 7;
  symbolTable["R8"] = 8;
  symbolTable["R9"] = 9;
  symbolTable["R10"] = 10;
  symbolTable["R11"] = 11;
  symbolTable["R12"] = 12;
  symbolTable["R13"] = 13;
  symbolTable["R14"] = 14;
  symbolTable["R15"] = 15;
  symbolTable["SCREEN"] = 16384;
  symbolTable["KBD"] = 24576;
}

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;

enum Kind{
  STATIC_K = 0, FIELD_K = 1, ARGUMENT_K = 2, VAR_K = 3, NONE_K = -1
};

class IdentifierInformation{
 public:
  IdentifierInformation();
  IdentifierInformation(string type, Kind kind, int identifier);
  
  string getType();
  Kind getKind();
  int getIdentifier();
  void setType(string type);
  void setKind(Kind kind);
  void setIdentifier(int identifier);

 private:
  string type;
  Kind kind;
  int identifier;
};

typedef IdentifierInformation *IdInfo;
typedef unordered_map<string, IdInfo> symbolTableInfo;
typedef unordered_map<string, symbolTableInfo> symbolTable;

class SymbolTable{
 public:
  SymbolTable();

  void resetNumberOfFields();
  void startSubroutine();
  void declareIdentifier(string &classOrSubroutine, string varName, string &type, Kind kind);
  int getNumOfVars(string &classOrSubroutine, Kind kind);
  Kind kindOf(string &className, string &subroutineName, string &varName);
  string typeOf(string &classOrSubroutine, string &varName);
  int indexOf(string &classOrSubroutine, string &varName);
  
  void addToVariableList(string varName);
  int getVariableListSize() const;
  string getVariable(int index) const;
  void clearVariableList();

  //Class ST's
  bool isClassType(string &className);
  void addStandardClass(string className);
  
  //Subroutines ST's
  bool isValidSubroutine(string subroutineName);
  string getSubroutineType(string subroutineName);
  void setSubroutineType(string subroutineName, string type);
  void addStandardSubroutine(string subroutineName, string subroutineType, int nArgs);
  
  void printCurrentST() const;

 private:
  int numOfStatics;
  int numOfFields;
  int currentNumOfArgs;
  int currentNumOfVars;

  symbolTable classSymbolTableList;
  symbolTable subroutineSymbolTableList;
  unordered_map<string, string> subroutineType;
  vector<string> currentVariableList;
};

#endif

#include "../include/symboltable.h"

IdentifierInformation::IdentifierInformation():
  type(""),
  kind(NONE_K),
  identifier(-1)
{}

IdentifierInformation::IdentifierInformation(string type, Kind kind, int identifier):
  type( type ),
  kind( kind ),
  identifier( identifier )
{}

string IdentifierInformation::getType(){ return this->type;}

Kind IdentifierInformation::getKind(){ return this->kind;}

int IdentifierInformation::getIdentifier(){ return this->identifier;}

void IdentifierInformation::setType(string type){
  this -> type = type;
}

void IdentifierInformation::setKind(Kind kind){
  this -> kind = kind;
}

void IdentifierInformation::setIdentifier(int identifier){
  this -> identifier = identifier;
}

//------------------------------------------------

SymbolTable::SymbolTable(){
  numOfStatics = 0;
  numOfFields = 0;
  currentNumOfArgs = 0;
  currentNumOfVars = 0;

  //Math library
  this -> addStandardClass("Math");
  this -> addStandardSubroutine("Math.abs", "function", 1);
  this -> addStandardSubroutine("Math.multiply", "function", 2);
  this -> addStandardSubroutine("Math.divide", "function", 2);
  this -> addStandardSubroutine("Math.min", "function", 2);
  this -> addStandardSubroutine("Math.max", "function", 2);
  this -> addStandardSubroutine("Math.sqrt", "function", 1);
  
  //String libary
  this -> addStandardClass("string");
  this -> addStandardSubroutine("String.new", "constructor", 1);
  this -> addStandardSubroutine("String.dispose", "method", 1);
  this -> addStandardSubroutine("String.length", "method", 1);
  this -> addStandardSubroutine("String.charAt", "method", 2);
  this -> addStandardSubroutine("String.setChar", "method", 3);
  this -> addStandardSubroutine("String.appendChar", "method", 2);
  this -> addStandardSubroutine("String.eraseLastChar", "method", 1);
  this -> addStandardSubroutine("String.intValue", "method", 1);
  this -> addStandardSubroutine("String.setInt", "method", 2);
  this -> addStandardSubroutine("String.backSpace", "function", 0);
  this -> addStandardSubroutine("String.doubleQuote", "function", 0);
  this -> addStandardSubroutine("String.newLine", "function", 0);

  //Array library
  this -> addStandardClass("Array");
  this -> addStandardSubroutine("Array.new", "function", 1);
  this -> addStandardSubroutine("Array.dispose", "method", 1);

  //Output library
  this -> addStandardClass("Output");
  this -> addStandardSubroutine("Output.moveCursor", "function", 2);
  this -> addStandardSubroutine("Output.printChar", "function", 1);
  this -> addStandardSubroutine("Output.printString", "function", 1);
  this -> addStandardSubroutine("Output.printInt", "function", 1);
  this -> addStandardSubroutine("Output.println", "function", 0);
  this -> addStandardSubroutine("Output.backSpace", "function", 0);

  //Screen library
  this -> addStandardClass("Screen");
  this -> addStandardSubroutine("Screen.clearScreen", "function", 0);
  this -> addStandardSubroutine("Screen.setColor", "function", 1);
  this -> addStandardSubroutine("Screen.drawPixel", "function", 2);
  this -> addStandardSubroutine("Screen.drawLine", "function", 4);
  this -> addStandardSubroutine("Screen.drawRectangle", "function", 4);
  this -> addStandardSubroutine("Screen.drawCircle", "function", 3);
  this -> addStandardSubroutine("Screen.drawCircle", "function", 3);

  //Keyboard library
  this -> addStandardClass("Keyboard");
  this -> addStandardSubroutine("Keyboard.keyPressed", "function", 0);
  this -> addStandardSubroutine("Keyboard.readChar", "function", 0);
  this -> addStandardSubroutine("Keyboard.readLine", "function", 1);
  this -> addStandardSubroutine("Keyboard.readInt", "function", 1);

  //Memory library
  this -> addStandardClass("Memory");
  this -> addStandardSubroutine("Memory.peek", "function", 1);
  this -> addStandardSubroutine("Memory.poke", "function", 2);
  this -> addStandardSubroutine("Memory.alloc", "function", 1);
  this -> addStandardSubroutine("Memory.deAlloc", "function", 1);

  //Sys library
  this -> addStandardClass("Sys");
  this -> addStandardSubroutine("Sys.init", "function", 0);
  this -> addStandardSubroutine("Sys.halt", "function", 0);
  this -> addStandardSubroutine("Sys.error", "function", 0);
  this -> addStandardSubroutine("Sys.wait", "function", 1);
}

void
SymbolTable::resetNumberOfFields(){ numOfFields = 0; }

void
SymbolTable::startSubroutine(){
  currentNumOfArgs = 0;
  currentNumOfVars = 0;
}

void
SymbolTable::declareIdentifier(string &classOrSubroutine, string varName, string &type, Kind kind){
  
  switch(kind){
  case STATIC_K:
    classSymbolTableList[classOrSubroutine][varName] = new IdentifierInformation(type, kind, numOfStatics);
    ++numOfStatics;
    break;
    
  case FIELD_K:
    classSymbolTableList[classOrSubroutine][varName] = new IdentifierInformation(type, kind, numOfFields);
    ++numOfFields;
    break;

  case ARGUMENT_K:
    subroutineSymbolTableList[classOrSubroutine][varName] = new IdentifierInformation(type, kind, currentNumOfArgs);
    ++currentNumOfArgs;
    break;
    
  case VAR_K:
    subroutineSymbolTableList[classOrSubroutine][varName] = new IdentifierInformation(type, kind, currentNumOfVars);
    ++currentNumOfVars;
    break;
    
  default:
    cout << "Unrecognized kind of identifier" << endl;
    break;
  }
}

int
SymbolTable::getNumOfVars(string &classOrSubroutine, Kind kind){

  symbolTableInfo innerMap;

  if((int)classOrSubroutine.find('.') < classOrSubroutine.size())
    innerMap = subroutineSymbolTableList[classOrSubroutine];
  else
    innerMap = classSymbolTableList[classOrSubroutine];
  
  int vars = 0;
  for(auto it : innerMap)
    if(it.second -> getKind() == kind)
      ++vars;
  return vars;
}

Kind
SymbolTable::kindOf(string &className, string &subroutineName, string &varName){
  auto innerMap = subroutineSymbolTableList[subroutineName];
  
  if(innerMap.find(varName) != innerMap.end()){
    return subroutineSymbolTableList[subroutineName][varName] -> getKind();
  }

  innerMap = classSymbolTableList[className];

  if(innerMap.find(varName) != innerMap.end()){
    return classSymbolTableList[className][varName] -> getKind();
  }

  return NONE_K;
}

string
SymbolTable::typeOf(string &classOrSubroutine, string &varName){
  if((int)classOrSubroutine.find('.') < classOrSubroutine.size())
    return subroutineSymbolTableList[classOrSubroutine][varName] -> getType();

  return classSymbolTableList[classOrSubroutine][varName] -> getType();
}

int
SymbolTable::indexOf(string &classOrSubroutine, string &varName){

  if((int)classOrSubroutine.find('.') < classOrSubroutine.size())
    return subroutineSymbolTableList[classOrSubroutine][varName] -> getIdentifier();

  return classSymbolTableList[classOrSubroutine][varName] -> getIdentifier();
}

void
SymbolTable::addToVariableList(string varName){ currentVariableList.push_back(varName); }

int
SymbolTable::getVariableListSize() const{ return currentVariableList.size(); }

string
SymbolTable::getVariable(int index) const{ return currentVariableList[index]; }

void
SymbolTable::clearVariableList(){ currentVariableList.clear(); }

//----------------Classes--------------------------------

bool
SymbolTable::isClassType(string &className){
  return classSymbolTableList.find(className) != classSymbolTableList.end();
}

void
SymbolTable::addStandardClass(string className){
  classSymbolTableList[className];
}

//------------------------------------------------------
//----------------Subroutines----------------------------
bool
SymbolTable::isValidSubroutine(string subroutineName){
  return subroutineSymbolTableList.find(subroutineName) != subroutineSymbolTableList.end();
}

string
SymbolTable::getSubroutineType(string subroutineName){
  return subroutineType[subroutineName];
}

void
SymbolTable::setSubroutineType(string subroutineName, string type){
  subroutineType[subroutineName] = type;
}

void
SymbolTable::addStandardSubroutine(string subroutineName, string subroutineType, int nArgs){
  if(nArgs == 0)
    subroutineSymbolTableList[subroutineName]["none"] = new IdentifierInformation("IDK", NONE_K, 0);
  else{
    for(int i = 0; i < nArgs; ++i){
      string argName = "arg" + std::to_string(i);
      subroutineSymbolTableList[subroutineName][argName] = new IdentifierInformation("IDK", ARGUMENT_K, i);
    }
  }
  this -> setSubroutineType(subroutineName, subroutineType);
}

//-------------------------------------------------------

void
SymbolTable::printCurrentST() const{

  for(auto it : classSymbolTableList){
    cout << "CLASS: " << it.first << endl;
    cout << "Name  :  Type  :  Kind  :  #" << endl;
    for(auto it2 : it.second){
      cout << it2.first << "   ";
      cout << it2.second -> getType() << "   ";
      cout << it2.second -> getKind() << "   ";
      cout << it2.second -> getIdentifier() << "   ";
      cout << endl;
    }

    for(auto it2 : subroutineSymbolTableList){
      cout << "SUBROUTINE: " << it2.first << endl;
      cout << "Name  :  Type  :  Kind  :  #" << endl;
      for(auto it3 : it2.second){
	cout << it3.first << "   ";
	cout << it3.second -> getType() << "   ";
	cout << it3.second -> getKind() << "   ";
	cout << it3.second -> getIdentifier() << "   ";
	cout << endl;
      }
    }
  }
}



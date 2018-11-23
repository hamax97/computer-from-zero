#include "../include/code.h"
#include <iostream>

using std::endl;
using std::cout;
using std::to_string;

Code::Code(){
  //when starts a new file the rom resets its counter to 0
  ROM = 0;
  //erases all elements in the hash map
  ROMsymbolTable.clear();
}

void Code::setFileName(string& fileName){
  fileName = fileName.substr(0, fileName.size() - 3);
  fileName += ".asm";
  output.open(fileName);
}

void Code::writeArithmetic(string commandToTranslate){
  switch(commandToTranslate[0]){
  case 'a': //add
    if(commandToTranslate[1] == 'd')
      addSubAndOrTemplate("D=D+M");
    else //and
      addSubAndOrTemplate("D=D&M");
    break;
    
  case 'o': //or
    addSubAndOrTemplate("D=D|M");
    break;

  case 's': //sub
    addSubAndOrTemplate("D=M-D");
    break;
    
  case 'n': //neg
    if(commandToTranslate[1] == 'e'){
      output << "@SP" << endl; ++ROM;
      output << "A=M-1" << endl; ++ROM;
      output << "D=!M" << endl; ++ROM;
      output << "D=D+1" << endl; ++ROM;
      output << "M=D" << endl; ++ROM;
    }else{ //not
      output << "@SP" << endl; ++ROM;
      output << "A=M-1" << endl; ++ROM;
      output << "M=!M" << endl; ++ROM;
    }
    break;
    
  case 'e': //eq
    eqGtLtTemplate("D;JEQ");
    break;
    
  case 'g': //gt
    eqGtLtTemplate("D;JGT");
    break;
    
  case 'l': //lt
    eqGtLtTemplate("D;JLT");
    break;
  }
}

void Code::addSubAndOrTemplate(string computation){
  output << "@SP" << endl; ++ROM;
  output << "AM=M-1" << endl; ++ROM;
  output << "D=M" << endl; ++ROM;
  output << "A=A-1" << endl; ++ROM;
  output << computation << endl; ++ROM;
  output << "M=D" << endl; ++ROM;
}

void Code::eqGtLtTemplate(string jumpType){
  ROMsymbolTable["TRUE"] = ROM + 12;
  ROMsymbolTable["END"] = ROM + 15;
  output << "@SP" << endl; ++ROM;
  output << "AM=M-1" << endl; ++ROM;
  output << "D=M" << endl; ++ROM;
  output << "A=A-1" << endl; ++ROM;
  output << "D=M-D" << endl; ++ROM;
  output << "@" << to_string(ROMsymbolTable["TRUE"]) << endl; ++ROM;
  output << jumpType << endl; ++ROM;
  output << "@SP" << endl; ++ROM;
  output << "A=M-1" << endl; ++ROM;
  output << "M=0" << endl; ++ROM;
  output << "@" << to_string(ROMsymbolTable["END"]) << endl; ++ROM;
  output << "0;JMP" << endl; ++ROM;
  output << "@SP" << endl; ++ROM;
  output << "A=M-1" << endl; ++ROM;
  output << "M=-1" << endl; ++ROM;
}

void Code::writePushPop(CommandType& t, string memorySegment, int index){
  if(t == C_PUSH){
    switch(memorySegment[0]){
    case 'c': //constant
      constStaticPUSHTemplate("CONST", index);
      break;
    case 's': //static
      constStaticPUSHTemplate("STATIC", index);
      break;
    case 'l': //local
      localThisThatArgPointPUSHTemplate("LCL", index);
      break;
    case 't':
      if(memorySegment[2] == 'i') //this
	localThisThatArgPointPUSHTemplate("THIS", index);
      else if(memorySegment[2] == 'a') //that
	localThisThatArgPointPUSHTemplate("THAT", index);
      else //temp
	localThisThatArgPointPUSHTemplate("5", index);
      break;
      
    case 'p': //pointer
      localThisThatArgPointPUSHTemplate("3", index);
      break;
      
    case 'a': //argument
      localThisThatArgPointPUSHTemplate("ARG", index);
      break;
    }
  }else{ // t is C_POP
    switch(memorySegment[0]){
    case 's': //static
      output << "@SP" << endl; ++ROM;
      output << "AM=M-1" << endl; ++ROM;
      output << "D=M" << endl; ++ROM;
      output << "@" << to_string(16 + index) << endl; ++ROM;
      output << "M=D" << endl; ++ROM; 
      break;
      
    case 'l': //local
      localThisThatArgTempPointPOPTemplate("LCL", index);
      break;
      
    case 't':
      if(memorySegment[2] == 'i') //this
	localThisThatArgTempPointPOPTemplate("THIS", index);
      else if(memorySegment[2] == 'a') //that
	localThisThatArgTempPointPOPTemplate("THAT", index);      
      else //temp
	localThisThatArgTempPointPOPTemplate("5", index);
      break;

    case 'p': //pointer
      localThisThatArgTempPointPOPTemplate("3", index);
      break;
      
    case 'a': //argument
      localThisThatArgTempPointPOPTemplate("ARG", index);
      break;
    }
  }
}

void Code::localThisThatArgTempPointPOPTemplate(string segment, int index){
  output << "@" << to_string(index) << endl; ++ROM;
  output << "D=A" << endl; ++ROM;
  output << "@" << segment << endl; ++ROM;
  if(segment == "5" || segment == "3"){ //temp or pointer
    output << "D=D+A" << endl; ++ROM;
  }
  else
    output << "D=D+M" << endl; ++ROM;
  output << "@R13" << endl; ++ROM;
  output << "M=D" << endl; ++ROM;
  output << "@SP" << endl; ++ROM;
  output << "AM=M-1" << endl; ++ROM;
  output << "D=M" << endl; ++ROM;
  output << "@R13" << endl; ++ROM;
  output << "A=M" << endl; ++ROM;
  output << "M=D" << endl; ++ROM;
}

void Code::constStaticPUSHTemplate(string segment, int index){
  if(segment == "STATIC"){ //static
    output << "@" << to_string(index + 16) << endl; ++ROM;
    output << "D=M" << endl; ++ROM;
  }else{ //constant
    output << "@" << to_string(index) << endl; ++ROM;
    output << "D=A" << endl; ++ROM;
  }
  output << "@SP" << endl; ++ROM;
  output << "A=M" << endl; ++ROM;
  output << "M=D" << endl; ++ROM;
  output << "@SP" << endl; ++ROM;
  output << "M=M+1" << endl; ++ROM;
}

void Code::localThisThatArgPointPUSHTemplate(string segment, int index){
  output << "@" << to_string(index) << endl; ++ROM;
  output << "D=A" << endl; ++ROM;
  output << "@" << segment << endl; ++ROM;
  if(segment == "5" || segment == "3"){ //temp or pointer segments
    output << "A=D+A" << endl; ++ROM;
  }
  else
    output << "A=D+M" << endl; ++ROM;
  output << "D=M" << endl; ++ROM;
  output << "@SP" << endl; ++ROM;
  output << "A=M" << endl; ++ROM;
  output << "M=D" << endl; ++ROM;
  output << "@SP" << endl; ++ROM;
  output << "M=M+1" << endl; ++ROM;
}

void Code::closeFile(){
  output.close();
}

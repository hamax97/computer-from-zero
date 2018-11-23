#include "../include/code.h"
#include <iostream>

using std::endl;
using std::cout;
using std::to_string;

int numberOfReturns = 0;
int numberOfTrues = 0;

Code::Code(){
  currentFunctionName = "";
}

void Code::getCurrentVMFileName(string fileName){
  int start = 0;
  for(int i = fileName.size() - 4; i >= 0; --i){
    if(fileName[i] == '\\' || fileName[i] == '/'){
      start = i+1;
      break;
    }
  }
  currentFileName = fileName.substr(start, fileName.size() - start - 3);
}

void Code::setOutputFileName(string fileName){

  //in case that fileName will be only one file
  if(fileName.substr(fileName.size() - 3, 3) == ".vm"){

    fileName = fileName.substr(0, fileName.size() - 3);
  }else{
    string auxName;

    if(fileName.back() != '/')
      fileName.push_back('/');
    
    for(int i = fileName.size() - 2; i >= 0; --i){
      if(fileName[i] == '/' || fileName[i] == '\\' || i == 0){
	auxName = fileName.substr(i+1, (fileName.size() - 2) - i);
	break;
      }
    }
    fileName += auxName;
  }
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
      output << "//NEG" << endl;
      output << "@SP" << endl;  
      output << "A=M-1" << endl;  
      output << "D=!M" << endl;  
      output << "D=D+1" << endl;  
      output << "M=D" << endl;  
    }else{ //not
      output << "//NOT" << endl;
      output << "@SP" << endl;  
      output << "A=M-1" << endl;  
      output << "M=!M" << endl;  
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
  output << "//AND SUB ADD OR" << endl;
  output << "@SP" << endl;  
  output << "AM=M-1" << endl;  
  output << "D=M" << endl;  
  output << "A=A-1" << endl;  
  output << computation << endl;  
  output << "M=D" << endl;  
}

void Code::eqGtLtTemplate(string jumpType){
  output << "//EQ GT LT" << endl;
  output << "@SP" << endl;  
  output << "AM=M-1" << endl;  
  output << "D=M" << endl;  
  output << "A=A-1" << endl;  
  output << "D=M-D" << endl;  
  output << "@TRUE" << numberOfTrues << endl;  
  output << jumpType << endl;  
  output << "@SP" << endl;  
  output << "A=M-1" << endl;  
  output << "M=0" << endl;  
  output << "@END" << endl;  
  output << "0;JMP" << endl;  
  output << "(TRUE" << numberOfTrues << ")" << endl;
  ++numberOfTrues;
  output << "@SP" << endl;  
  output << "A=M-1" << endl;  
  output << "M=-1" << endl;  
  output << "(END)" << endl; 
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
      output << "//static POP" << endl;
      output << "@SP" << endl;  
      output << "AM=M-1" << endl;  
      output << "D=M" << endl;  
      output << "@" << currentFileName << "." << index << endl;  
      output << "M=D" << endl;   
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
  output << "//local this that arg POP" << endl;
  output << "@" << to_string(index) << endl;  
  output << "D=A" << endl;  
  output << "@" << segment << endl;  
  if(segment == "5" || segment == "3"){ //temp or pointer
    output << "D=D+A" << endl;  
  }
  else
    output << "D=D+M" << endl;  
  output << "@R13" << endl;  
  output << "M=D" << endl;  
  output << "@SP" << endl;  
  output << "AM=M-1" << endl;  
  output << "D=M" << endl;  
  output << "@R13" << endl;  
  output << "A=M" << endl;  
  output << "M=D" << endl;  
}

void Code::constStaticPUSHTemplate(string segment, int index){
  if(segment == "STATIC"){ //static
    output << "//static PUSH" << endl;
    output << "@" << currentFileName << "." << index <<endl;  
    output << "D=M" << endl;  
  }else{ //constant
    output << "//constant PUSH" << endl;
    output << "@" << to_string(index) << endl;  
    output << "D=A" << endl;  
  }
  output << "@SP" << endl;  
  output << "A=M" << endl;  
  output << "M=D" << endl;  
  output << "@SP" << endl;  
  output << "M=M+1" << endl;  
}

void Code::localThisThatArgPointPUSHTemplate(string segment, int index){
  output << "//local this that arg point PUSH" << endl;
  output << "@" << to_string(index) << endl;  
  output << "D=A" << endl;  
  output << "@" << segment << endl;  
  if(segment == "5" || segment == "3"){ //temp or pointer segments
    output << "A=D+A" << endl;  
  }
  else
    output << "A=D+M" << endl;  
  output << "D=M" << endl;  
  output << "@SP" << endl;  
  output << "A=M" << endl;  
  output << "M=D" << endl;  
  output << "@SP" << endl;  
  output << "M=M+1" << endl;  
}

void Code::closeFile(){
  output.close();
}

void Code::writeInit(){
  //SP=256
  output << "@256" << endl;  
  output << "D=A" << endl;  
  output << "@SP" << endl;  
  output << "M=D" <<  endl; 

  //call Sys.init
  writeCall("Sys.init", 0);
}

void Code::writeLabel(string label){
  output << "(" << currentFunctionName << "$" << label << ")" << endl; 
}

void Code::writeGoto(string label){
  output << "@" << currentFunctionName << "$" << label << endl;  
  output << "0;JMP" << endl;  
}

void Code::writeIf(string label){
  output << "//IF-GOTO" << endl;
  output << "@SP" << endl;  
  output << "AM=M-1" << endl;  
  output << "D=M" << endl;  
  output << "@" << currentFunctionName << "$" << label << endl;  
  output << "D;JNE" << endl;   
}

void Code::writeCall(string functionName, int numArgs){
  output << "//CALL" << endl;
  //save return address
  output << "@" << "return-address." << numberOfReturns << endl;  
  output << "D=A" << endl;  
  output << "@SP" << endl;  
  output << "A=M" << endl;  
  output << "M=D" << endl;  
  output << "@SP" << endl;  
  output << "M=M+1" << endl;  

  //save caller LCL
  saveSegment("LCL");

  //save caller ARG
  saveSegment("ARG");

  //save caller THIS
  saveSegment("THIS");

  //save caller THAT
  saveSegment("THAT");

  //new ARG: ARG = SP - numArgs - 5
  // @SP was declared in the previous instructions
  output << "D=M" << endl;  
  output << "@" << to_string(numArgs) << endl;  
  output << "D=D-A" << endl;  
  output << "@5" << endl;  
  output << "D=D-A" << endl;  
  output << "@ARG" << endl;  
  output << "M=D" << endl;  

  //new LCL: LCL = SP
  output << "@SP" << endl;  
  output << "D=M" << endl;  
  output << "@LCL" << endl;  
  output << "M=D" << endl;  

  //goto f
  output << "@" << functionName << endl;
  output << "0;JMP" << endl; 
  //return-address label
  output << "(return-address." << numberOfReturns << ")" << endl;
  ++numberOfReturns;
}

void Code::saveSegment(string segment){
  output << "@" << segment << endl;  
  output << "D=M" << endl;  
  output << "@SP" << endl;  
  output << "A=M" << endl;  
  output << "M=D" << endl;  
  output << "@SP" << endl;  
  output << "M=M+1" << endl; 
}

void Code::writeReturn(){ //ERROR HERE
  output << "@LCL    //RETURN" << endl;  
  output << "D=M" << endl;  
  //temporary variable
  output << "@" << currentFileName << ".FRAME" << endl;  
  output << "M=D" << endl;   //FRAME = LCL

  //RET = *(FRAME-5)
  restoreSegment("RET", 5);

  output << "@SP" << endl;  
  output << "AM=M-1" << endl;  
  output << "D=M" << endl;  
  output << "@ARG" << endl;  
  output << "A=M" << endl;  
  output << "M=D" << endl;   //*ARG = pop()

  output << "@ARG" << endl;  
  output << "D=M+1" << endl;  
  output << "@SP" << endl;  
  output << "M=D" << endl;  //SP = ARG+1

  //restore THAT: THAT = *(FRAME-1)
  restoreSegment("THAT", 1);
  //restore THIS: THIS = *(FRAME-2)
  restoreSegment("THIS", 2);
  //restore ARG: ARG = *(FRAME-3)
  restoreSegment("ARG", 3);
  //restore LCL: LCL = *(FRAME-4)
  restoreSegment("LCL", 4);

  output << "@" << currentFileName << ".RET" << endl;  
  output << "A=M" << endl;  
  output << "0;JMP" << endl;  
}

void Code::restoreSegment(string segmentOrVariable, int index){
  output << "@" << currentFileName << ".FRAME" << "    //RESTORE SEG" << endl;  
  output << "D=M" << endl; 
  output << "@" << to_string(index) << endl;  
  output << "D=D-A" << endl; 
  output << "A=D" << endl; 
  output << "D=M" << endl; 
  
  if(segmentOrVariable == "RET"){
    output << "@" << currentFileName << "." << segmentOrVariable << endl; 
  }else{
    output << "@" << segmentOrVariable << endl; 
  }
  
  output << "M=D" << endl;  
}

void Code::writeFunction(string functionName, int numLocals){
  output << "(" << functionName << ")" << endl;
  currentFunctionName = functionName;

  //initializing local variables
  for(int i = 0; i < numLocals; ++i)
    constStaticPUSHTemplate("CONSTANT", 0);
}

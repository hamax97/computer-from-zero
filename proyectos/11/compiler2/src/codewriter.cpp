#include "../include/codewriter.h"

extern ofstream parserFileOut;

CodeWriter::CodeWriter(){ }

void CodeWriter::writePush(string segment, int index){
  parserFileOut << "push " << segment << " " << index << endl;
}

void CodeWriter::writePop(string segment, int index){
  parserFileOut << "pop " << segment << " " << index << endl;
}

void CodeWriter::writeArithmetic(string function){
  parserFileOut << function << endl;
}

void CodeWriter::writeLabel(string label){
  parserFileOut << "label " << label << endl;
}

void CodeWriter::writeGoto(string label){
  parserFileOut << "goto " << label << endl;
}

void CodeWriter::writeIf(string label){
  parserFileOut << "if-goto " << label << endl;
}

void CodeWriter::writeCall(string functionName, int nArgs){
  parserFileOut << "call " << functionName << " " << nArgs << endl;
}

void CodeWriter::writeFunction(string functionName, int nLocals){
  parserFileOut << "function " << functionName << " " << nLocals << endl;
}

void CodeWriter::writeReturn(){
  parserFileOut << "return" << endl;
}

#ifndef CODE_H
#define CODE_H

#include "commandtype.h"
#include <string>
#include <fstream>
#include <unordered_map>

using std::string;
using std::ofstream;
using std::unordered_map;

class Code{

 private:
  /* stream where the assembler code is going to stay */
  ofstream output;

  /* integer to control the position of the instruction in the ROM */
  int ROM;

  /* map to take control of the labels */
  unordered_map <string, int> ROMsymbolTable;
  
 public:
  /* constructor of the class */
  Code();

  /* set the name of the output file */
  void setFileName(string& name);

  /* translates the given command into hack assembler and writes it into the output file */
  void writeArithmetic(string commandToTranslate);

  /* a template for add, sub 
     and, or operations that avoids the code repetition */
  void addSubAndOrTemplate(string computation);

  /* a template for eq, gt, lt operations that
     avoid the code repetition */
  void eqGtLtTemplate(string jumpType);

  /* translates the given command into hack assembler and writes it into the output file */
  void writePushPop(CommandType& t, string memorySegment, int index);

  /* a template for push local | this | that | argument | 
     pointer | temp template that avoids repetition of code */
  void localThisThatArgPointPUSHTemplate(string segment, int index);

  /* a template for push constant | static template that avoids repetition of code */
  void constStaticPUSHTemplate(string segment, int index);

  /* a template for pop local | this | that | argument | temp 
     | pointer template that avoids repetition of code */
  void localThisThatArgTempPointPOPTemplate(string segment, int index);
    
  /* closes the output file */
  void closeFile();
  
};

#endif

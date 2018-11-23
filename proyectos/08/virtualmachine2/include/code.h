#ifndef CODE_H
#define CODE_H

#include "commandtype.h"
#include <string>
#include <fstream>

using std::string;
using std::ofstream;

class Code{

 private:
  /* stream where the assembler code is going to stay */
  ofstream output;

  /* contains only the current file name, but it does not
     contain its route */
  string currentFileName;

  string currentFunctionName;
  
 public:
  /* constructor of the class */
  Code();

  void getCurrentVMFileName(string file);
  
  /* set the name of the output file */
  void setOutputFileName(string name);

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

  /* writes SP=256 and call Sys.init at the beginning of the file
     if and onnly if the Sys.vm file exists */
  void writeInit();

  void writeLabel(string label);

  void writeGoto(string label);

  void writeIf(string label);

  void writeCall(string functionName, int numArgs);
  
  /* auxiliary function to push(save) the segment pointer specified */
  void saveSegment(string segment);

  void writeReturn();

  /* restores the segment specified or saves it in an spcified variable */
  void restoreSegment(string segmentOrVariable, int index);

  void writeFunction(string functionName, int numLocals);
    
  /* closes the output file */
  void closeFile();
  
};

#endif

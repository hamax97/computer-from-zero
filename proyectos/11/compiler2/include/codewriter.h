#include "symboltable.h"
#include <fstream>

using std::ofstream;

class CodeWriter{
 public:
  CodeWriter();

  void writePush(string segment, int index);
  void writePop(string segment, int index);
  void writeArithmetic(string function);
  void writeLabel(string label);
  void writeGoto(string label);
  void writeIf(string label);
  void writeCall(string functionName, int nArgs);
  void writeFunction(string functionName, int nLocals);
  void writeReturn();

};

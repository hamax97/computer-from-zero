#ifndef CODE_H
#define CODE_H

#include <string>

using std::string;

class Code{
 public:
  /* constructor of the class */
  Code();
  /* translates the mnemonic of destiny to a binary code  */
  string transDest(string destiny) const;
  /* translates the mnemonic of computation to a binary code  */
  string transComp(string computation) const;
  /* translates the mnemonic of jump to a binary code  */
  string transJump(string jump) const;
  /* converts an integer into binary */
  string intToBinary(int& number) const;
};

#endif

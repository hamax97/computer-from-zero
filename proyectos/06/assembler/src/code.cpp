#include "../include/code.h"
#include <bitset>

using std::bitset;

Code::Code(){}

string Code::transDest(string destiny) const{
  string bin("000");
  
  for(int i = 0; i < destiny.size(); ++i)
    switch(destiny[i]){
    case 'M':
      bin[2] = '1';
      break;
    case 'D':
      bin[1] = '1';
      break;
    case 'A':
      bin[0] = '1';
      break;
    }
  
  return bin;
}

string Code::transComp(string computation) const{
  int compSize = computation.size();
  string bin("0000000");
  bool flagAoN = false;

  if(compSize <= 2)
    switch(computation[0]){
    case '0': //-----------
      bin[1] = '1';
      bin[3] = '1';
      bin[5] = '1';
      break;
    case '1': //-----------
      for(int i = 1; i < 7; ++i)
	bin[i] = '1';
      break;
    case '-': //-----------
      if(computation[1] == '1'){
	bin[1] = '1';
	bin[2] = '1';
	bin[3] = '1';
	bin[5] = '1';
      }
      else if(computation[1] == 'D'){
	bin[3] = '1';
	bin[4] = '1';
	bin[5] = '1';
	bin[6] = '1';
      }
      else if(computation[1] == 'A'){
	bin[1] = '1';
	bin[2] = '1';
	bin[5] = '1';
	bin[6] = '1';
      }
      else if(computation[1] == 'M'){
	bin[1] = '1';
	bin[2] = '1';
	bin[5] = '1';
	bin[6] = '1';
	flagAoN = true;
      }
      break;
    case 'D': //-----------
      bin[3] = '1';
      bin[4] = '1';
      break;
    case 'A': //-----------
      bin[1] = '1';
      bin[2] = '1';
      break;
    case 'M': //-----------
      bin[1] = '1';
      bin[2] = '1';
      flagAoN = true;
      break;
    case '!': //-----------
      if(computation[1] == 'D'){
	bin[3] = '1';
	bin[4] = '1';
	bin[6] = '1';
      }
      else if(computation[1] == 'A'){
	bin[1] = '1';
	bin[2] = '1';
	bin[6] = '1';
      }
      else if(computation[1] == 'M'){
	bin[1] = '1';
	bin[2] = '1';
	bin[6] = '1';
	flagAoN = true;
      }
      break;
    }
  else{
    switch(computation[0]){
    case 'D': //-----------
      if(computation[1] == '+'){
	if(computation[2] == '1'){
	  bin[2] = '1';
	  bin[3] = '1';
	  bin[4] = '1';
	  bin[5] = '1';
	  bin[6] = '1';
	}else if(computation[2] == 'A' || computation[2] == 'M'){
	  bin[5] = '1';
	}
      }else if(computation[1] == '-'){
	if(computation[2] == '1'){
	  bin[3] = '1';
	  bin[4] = '1';
	  bin[5] = '1';
	}else if(computation[2] == 'A' || computation[2] == 'M'){
	  bin[2] = '1';
	  bin[5] = '1';
	  bin[6] = '1';
	}
      }else if(computation[1] == '&'){
      }else if(computation[1] == '|'){
	bin[2] = '1';
	bin[4] = '1';
	bin[6] = '1';
      }
      if(computation[2] == 'M') flagAoN = true;
      break;
      
    case 'A': //-----------
      if(computation[1] == '+'){
	bin[1] = '1';
	bin[2] = '1';
	bin[4] = '1';
	bin[5] = '1';
	bin[6] = '1';
      }else if(computation[1] == '-')
	if(computation[2] == '1'){
	  bin[1] = '1';
	  bin[2] = '1';
	  bin[5] = '1';  
	}else{ //computation[2] = D
	  bin[4] = '1';
	  bin[5] = '1';
	  bin[6] = '1';  
	} 
      break;
    case 'M': //-----------
      if(computation[1] == '+'){
	bin[1] = '1';
	bin[2] = '1';
	bin[4] = '1';
	bin[5] = '1';
	bin[6] = '1';
      }else if(computation[1] == '-')
	if(computation[2] == '1'){
	  bin[1] = '1';
	  bin[2] = '1';
	  bin[5] = '1';  
	}else{ //computation[2] = D
	  bin[4] = '1';
	  bin[5] = '1';
	  bin[6] = '1';  
	} 
      break;
    }
    if(computation[0] == 'M' || computation[2] == 'M') flagAoN = true;
  }
  if(flagAoN) bin[0] = '1';
  
  return bin;
}

string Code::transJump(string jump) const{
  string bin("000");

  switch(jump[1]){
  case 'G':
    if(jump[2] == 'T')
      bin[2] = '1';//JGT
    else{ //JGE
      bin[1] = '1';
      bin[2] = '1';
    }
    break;
  case 'E': //JEQ
    bin[1] = '1';
    break;
  case 'L':
    if(jump[2] == 'T')
      bin[0] = '1';//JLT
    else{ //JLE
      bin[0] = '1';
      bin[1] = '1';
    }
    break;
  case 'N': //JNE
    bin[0] = '1';
    bin[2] = '1';
    break;
  case 'M': //JMP
    bin[0] = '1';
    bin[1] = '1';
    bin[2] = '1';
    break;
  }
  return bin;
}

string Code::intToBinary(int& number) const{
  string bin = bitset<16>(number).to_string();
  return bin;
}

#include "../include/parser.h"
#include "../include/code.h"
#include <iostream>
#include <vector>
#include <dirent.h>

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::vector;

/* parses and decodificate a .vm file into assembler */
void decodificateFile(Code& decodificator, string fileName);

string concatenateFileAndRoute(string route, string &file);

int
main(int argc, char *argv[])
{
  //if there are a different number of arguments then error
  if(argc != 2){
    cerr << "Error introducing file to parse as program argument: " << argv[0] << "<file to parse>" << endl;
    return 1;
  }
  //argument 1 should be the file name or the directory with the files
  string fileS(argv[1]);

  //translator of the vm code to assembler code
  Code decodificator;

  //if the programm input is a file just parse and decodificate it
  if(fileS.size() >= 4 && fileS.substr(fileS.size() - 3, 3) == ".vm"){
    
    // creating and naming output file
    decodificator.setOutputFileName(fileS);
    
    decodificator.getCurrentVMFileName(fileS);
    if(fileS.substr(fileS.size() - 6, 6) == "Sys.vm")
      decodificator.writeInit();
    decodificateFile(decodificator, fileS);
    
  }else{// else go inside the directory and parse file by file
    
    DIR *dir;
    struct dirent *entry;

    const char *fileSaux = fileS.c_str();
    cout << "Folder: ";
    for(int i = 0; i < fileS.size(); ++i)
      cout << *(fileSaux + i);
    cout << endl;

    //if the directory exists
    if(( dir = opendir(argv[1]) ) != NULL) {

      //Array of strings that contains the name of the .vm files inside the directory
      vector< string > setOfFiles;
      setOfFiles.resize(0);
      
      // creating and naming output file
      decodificator.setOutputFileName(fileS);

      /* print all the files and directories within directory */
      while((entry = readdir(dir)) != NULL){
	
	setOfFiles.push_back(entry -> d_name);

	string currentFile = setOfFiles.back();

	if(currentFile.size() < 3){
	  setOfFiles.pop_back();
	  continue;
	}
	
	//if it is a .vm file add it to the list else deletes it
	if(currentFile.substr( currentFile.size() - 3, 3) != ".vm" ){
	  setOfFiles.pop_back(); //deletes it
	}
	else if(currentFile == "Sys.vm"){
	  decodificator.writeInit();
	}
	
      }

      for(int i = 0; i < setOfFiles.size(); ++i){
	decodificator.getCurrentVMFileName(setOfFiles[i]);
	decodificateFile(decodificator, concatenateFileAndRoute(fileS, setOfFiles[i]));
      }
      
      closedir(dir);
      
    } else {// print error if the directory does not exist
      /* could not open directory */
      cerr << "Directory: " << fileS << "NOT FOUND" << endl;
    }
  }
  decodificator.closeFile();
  return 0;
}

void decodificateFile(Code& decodificator, string fileName){
  Parser parser(fileName);
  CommandType commandType;
  
  while(parser.advance()){
    commandType = parser.getCommandType();
    if(commandType == C_ARITHMETIC){
      decodificator.writeArithmetic(parser.getArg1(commandType));
      
    }else if(commandType == C_PUSH || commandType == C_POP){
      decodificator.writePushPop(commandType, parser.getArg1(commandType), parser.getArg2());
      
    }else if(commandType == C_LABEL){
      decodificator.writeLabel(parser.getArg1(commandType));
      
    }else if(commandType == C_GOTO){
      decodificator.writeGoto(parser.getArg1(commandType));
      
    }else if(commandType == C_IF){
      decodificator.writeIf(parser.getArg1(commandType));
      
    }else if(commandType == C_FUNCTION){
      decodificator.writeFunction(parser.getArg1(commandType), parser.getArg2());
      
    }else if(commandType == C_CALL){
      decodificator.writeCall(parser.getArg1(commandType), parser.getArg2());
      
    }else if(commandType == C_RETURN){
       decodificator.writeReturn();
    }
  }
}

string concatenateFileAndRoute(string route, string &file){
  if(route.back() != '/')
    route.push_back('/');
  return route + file;
}

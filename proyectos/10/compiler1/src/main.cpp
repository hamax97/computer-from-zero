#include "lex.yy.c"

#include <dirent.h>

string currentFileDebug;
ofstream parserFileOut;

bool isJackFile(string &fileName){
  if(fileName.size() >= 6 && fileName.substr(fileName.size() - 5, 5) == ".jack")
    return true;
  return false;
}

void parseFile(string &fileName){
  currentFileDebug = fileName;
  //------
  string scannerFileName(fileName.substr(0, fileName.size() - 5));
  scannerFileName += "T.xml";
  scannerFileOut.open(scannerFileName);
  //------
  
  FILE *file = fopen(fileName.c_str(), "r");
  yyin = file;

  cout << "Compiling file: " << fileName << endl;
  yyparse();

  if(noError){
    string parserFileName(fileName.substr(0, fileName.size() - 5));
    parserFileName += ".xml";
    parserFileOut.open(parserFileName);

    prog -> visit();
    parserFileOut.close();
  }
  
  scannerFileOut.close();
  if(!fclose(file)) //return 0 if all is correct
    cout << "Closing file: " << fileName << endl;
}

int main(int argc, char *argv[]){

  if(argc != 2){
    cerr << "Error introducing file to parse as program argument: " << argv[0] << "<file to parse>" << endl;
    return 1;
  }
  
  string fileS(argv[1]);

  //only one file
  if(isJackFile(fileS)){
    parseFile(fileS);
    
  }else{// a directory
    
    DIR *dir;
    struct dirent *entry;

    const char *fileSaux = fileS.c_str();
    cout << "Folder: ";
    for(int i = 0; i < fileS.size(); ++i)
      cout << *(fileSaux + i);
    cout << endl;

    //if the directory exists
    if(( dir = opendir(argv[1]) ) != NULL) {

      while((entry = readdir(dir)) != NULL){

	string currentFile(fileSaux);
	string aux(entry -> d_name);
	currentFile += aux;

	if(isJackFile(currentFile))
	  parseFile(currentFile);
      }
      
      closedir(dir);
      
    } else {// print error if the directory does not exist
      /* could not open directory */
      cerr << "Directory: " << fileS << " NOT FOUND!!" << endl;
    }
  }
  
  return 0;
}

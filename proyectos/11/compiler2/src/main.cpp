#include "lex.yy.c"

#include <dirent.h>

string currentFileDebug; //variable that helps to print debug information
ofstream parserFileOut;

//Variables to construct the symbol table

SymbolTable *st = new SymbolTable();
bool symbolTableConstructed = false;

//-------------------

bool isJackFile(string &fileName){
  if(fileName.size() >= 6 && fileName.substr(fileName.size() - 5, 5) == ".jack" && fileName[fileName.size() - 1] == 'k')
    return true;
  return false;
}

void parseFile(string &fileName){
  yylineno = 0;
  currentFileDebug = fileName;
  //------
  
  FILE *file = fopen(fileName.c_str(), "r");
  yyin = file;

  //cout << "Compiling file: " << fileName << endl;

  yyparse();

  if(noError){
    string parserFileName(fileName.substr(0, fileName.size() - 5));
    parserFileName += ".vm";
    parserFileOut.open(parserFileName);

    prog -> visit();
    parserFileOut.close();
  }
  
  if(!fclose(file)) //return 0 if all is correct
    ;//cout << "Closing file: " << fileName << endl;
  yylineno = 0;
}

int main(int argc, char *argv[]){

  if(argc != 2){
    cerr << "Error introducing file to parse as program argument: " << argv[0] << "<file to parse>" << endl;
    return 1;
  }
  
  string fileS(argv[1]);

  //only one file
  if(isJackFile(fileS)){
    int i = 0;
    while(i < 2){
      parseFile(fileS);
      if(!symbolTableConstructed)
	symbolTableConstructed = true;
      ++i;
    }
    
  }else{// a directory
    
    DIR *dir;
    struct dirent *entry;

    const char *fileSaux = fileS.c_str();
    cout << "Compiling folder: ";
    for(int i = 0; i < fileS.size(); ++i)
      cout << *(fileSaux + i);
    cout << endl;

    int i = 0;
    while(i < 2){
      //if the directory exists
      if(( dir = opendir(argv[1]) ) != NULL) {
	
	while((entry = readdir(dir)) != NULL){
	  
	  string currentFile(fileSaux);
	  string aux(entry -> d_name);
	  currentFile += aux;
	  
	  if(isJackFile(currentFile))
	    parseFile(currentFile);
	}
	
	if(!symbolTableConstructed){
	  symbolTableConstructed = true;
	  //st -> printCurrentST();
	}
	
	closedir(dir);

	++i;
      
      } else {// print error if the directory does not exist
	/* could not open directory */
	cerr << "Directory: " << fileS << " NOT FOUND!!" << endl;
      }
    }
  }

  //st -> printCurrentST();
  
  delete st;

  cout << "Compilation finished" << endl;
  return 0;
}

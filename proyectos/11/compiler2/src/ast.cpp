#include "../include/ast.h"
#include "../include/codewriter.h"
#include "../include/errors.h"

//-----------------------------------------
//UTILITIES

extern void errorFound(Error errorType, string var = "");

//--------- Symbol Table --------

extern SymbolTable *st;
extern bool symbolTableConstructed;
string currentClassName;
string currentSubroutineName;

//-------------------------------
//-------- Code Writer ----------

CodeWriter *codeWriter = new CodeWriter();
vector<int> trueLabel;

//-------------------------------

//Uncomment for debug information
//#define debug

//-----------------------------------------

AST::AST(){}
AST::~AST(){}

//-----------------------------------------

ProgNode::ProgNode(AST* classNode):
  AST(),
  classNode(classNode)
{}

ProgNode::~ProgNode()
{
#ifdef debug
  cout << "In ProgNode desctructor!" << endl;
#endif
  try{
    delete classNode;
  }catch(...){}
}

void ProgNode::visit(){

  trueLabel.resize(0);
  trueLabel.push_back(0);

  if(symbolTableConstructed)
    ;//st-> printCurrentST();
  
  classNode -> visit();
}

//-----------------------------------------

ClassNode::ClassNode(string classKeyword, string classIdentifier, char lbSymbol, AST* listClassVarDecNode, AST* subroutineDecNode, char rbSymbol):
  AST(),
  classKeyword( classKeyword ),
  classIdentifier( classIdentifier ),
  lbSymbol( lbSymbol ),
  listClassVarDecNode( listClassVarDecNode ),
  subroutineDecNode( subroutineDecNode ),
  rbSymbol( rbSymbol )
{}

ClassNode::~ClassNode(){
#ifdef debug
  cout << "In ClassNode desctructor!" << endl;
#endif
  try{
    delete listClassVarDecNode;
    delete subroutineDecNode;
  }catch(...){}
}

void ClassNode::visit(){

  currentClassName = classIdentifier;

  if(!symbolTableConstructed)
    st -> addStandardClass(currentClassName);
  
  if(!symbolTableConstructed)
    st -> resetNumberOfFields();
  
  if(listClassVarDecNode != NULL)
    listClassVarDecNode -> visit();
  if(subroutineDecNode != NULL)
    subroutineDecNode -> visit();

}

//-----------------------------------------

ListClassVarDecNode::ListClassVarDecNode(AST* classVarDecNode, AST* listClassVarDecNode):
  classVarDecNode( classVarDecNode ),
  listClassVarDecNode( listClassVarDecNode)
{}

ListClassVarDecNode::~ListClassVarDecNode(){
#ifdef debug
  cout << "In ListClassVarDecNode desctructor!" << endl;
#endif
  
  try{
    delete classVarDecNode;
    delete listClassVarDecNode;
  }catch(...){}
}

void ListClassVarDecNode::visit(){
  if(listClassVarDecNode != NULL)
    listClassVarDecNode -> visit();
  classVarDecNode -> visit();
}

ClassVarDecNode::ClassVarDecNode(string staticOrFieldKeyword, string classVarType, AST* listVarNameNode, char semicolon):
  AST(),
  staticOrFieldKeyword( staticOrFieldKeyword ),
  classVarType( classVarType ),
  listVarNameNode( listVarNameNode ),
  semicolon( semicolon )
{}

ClassVarDecNode::~ClassVarDecNode(){
#ifdef debug
  cout << "In ClassVarDecNode desctructor!" << endl;
#endif
  
  try{
    delete listVarNameNode;
  }catch(...){}
}

void ClassVarDecNode::visit(){
  
  listVarNameNode -> visit();

  if(!symbolTableConstructed){
    Kind classKind;
    if(staticOrFieldKeyword == "static")
      classKind = STATIC_K;
    else
      classKind = FIELD_K;
    
    for(int i = 0; i < st -> getVariableListSize(); ++i)
      st -> declareIdentifier( currentClassName, st -> getVariable(i), classVarType, classKind);
    
    //reset the state of the list of names to get the names of the next line
    st -> clearVariableList();
  }
}

//-----------------------------------------

ListVarNameNode::ListVarNameNode(string varName, char commaSymbol, string varName2, AST* listVarNameNode):
  AST(),
  varName( varName ),
  commaSymbol( commaSymbol ),
  varName2( varName2 ),
  listVarNameNode( listVarNameNode )
{}

ListVarNameNode::~ListVarNameNode(){
#ifdef debug
  cout << "In ListVarNameNode desctructor!" << endl;
#endif
  
  try{
    delete listVarNameNode;
  }catch(...){}
}

void ListVarNameNode::visit(){
  if(listVarNameNode != NULL)
    listVarNameNode -> visit();

  if(varName != NULLSTRING){

    if(!symbolTableConstructed)
      st -> addToVariableList(varName);
  }
  if(commaSymbol != NULLCHAR){

    if(!symbolTableConstructed)
      st -> addToVariableList(varName2);
  }
}

//-----------------------------------------

ListSubroutineDecNode::ListSubroutineDecNode(AST* subroutineDecNode, AST* listSubroutineDecNode):
  subroutineDecNode( subroutineDecNode ),
  listSubroutineDecNode( listSubroutineDecNode )
{}

ListSubroutineDecNode::~ListSubroutineDecNode(){
  try{
    delete listSubroutineDecNode;
    delete subroutineDecNode;
  }catch(...){}
}

void ListSubroutineDecNode::visit(){
  if(listSubroutineDecNode != NULL)
    listSubroutineDecNode -> visit();
  subroutineDecNode -> visit();
}

SubroutineDecNode::SubroutineDecNode(string procedureType, string returnType, string subroutineName, char lpSymbol, AST* parameterListNode, char rpSymbol, AST* subroutineBodyNode):
  AST(),
  procedureType( procedureType ),
  returnType( returnType ),
  subroutineName( subroutineName ),
  lpSymbol( lpSymbol ),
  parameterListNode( parameterListNode ),
  rpSymbol( rpSymbol ),
  subroutineBodyNode( subroutineBodyNode )
{}

SubroutineDecNode::~SubroutineDecNode(){
#ifdef debug
  cout << "In SubroutineDecNode desctructor!" << endl;
#endif
  
  try{
    delete parameterListNode;
  }catch(...){}
}

void SubroutineDecNode::visit(){

  currentSubroutineName = (currentClassName + "." + subroutineName);
  if(!symbolTableConstructed){
    st -> setSubroutineType(currentSubroutineName, procedureType);
  }
  
  if(!symbolTableConstructed)
    st -> startSubroutine();
  else{
    codeWriter -> writeFunction(currentSubroutineName, st -> getNumOfVars(currentSubroutineName, VAR_K));
    if(procedureType == "constructor"){
      codeWriter -> writePush("constant", st -> getNumOfVars(currentClassName, FIELD_K));
      codeWriter -> writeCall("Memory.alloc", 1);
      codeWriter -> writePop("pointer", 0);
      
    }else if(procedureType == "method"){
      codeWriter -> writePush("argument", 0);
      codeWriter -> writePop("pointer", 0);
    }
  }

  if(!symbolTableConstructed && procedureType == "method"){
    //First argument for each subroutine
    st -> declareIdentifier(currentSubroutineName, "this", currentClassName, ARGUMENT_K);
  }else{//constructor or function
    st -> addStandardSubroutine(currentSubroutineName, procedureType, 0);
  }
  
  if(parameterListNode != NULL){
    parameterListNode -> visit();
  }
  
  subroutineBodyNode -> visit();
  
}

//-----------------------------------------

ParameterListNode::ParameterListNode(string varType, string varName, char commaSymbol, AST* parameterListNode):
  varType( varType ),
  varName( varName ),
  commaSymbol( commaSymbol ),
  parameterListNode( parameterListNode )
{}

ParameterListNode::~ParameterListNode(){
#ifdef debug
  cout << "In ParameterListNode desctructor!" << endl;
#endif
  
  try{
    delete parameterListNode;
  }catch(...){}
}

void ParameterListNode::visit(){
  if(parameterListNode != NULL)
    parameterListNode -> visit();

  if(!symbolTableConstructed)
    st -> declareIdentifier(currentSubroutineName, varName, varType, ARGUMENT_K);
  
}

//-----------------------------------------

SubroutineBodyNode::SubroutineBodyNode(char lbSymbol, AST* listVarDecNode, AST* statementsNode, char rbSymbol):
  AST(),
  lbSymbol( lbSymbol ),
  listVarDecNode( listVarDecNode ),
  statementsNode( statementsNode ),
  rbSymbol( rbSymbol )
{}

SubroutineBodyNode::~SubroutineBodyNode(){
#ifdef debug
  cout << "In SubroutineBodyNode desctructor!" << endl;
#endif

  try{
    delete listVarDecNode;
  }catch(...){}
}

void SubroutineBodyNode::visit(){
  
  if(listVarDecNode != NULL)
    listVarDecNode -> visit();

  if(statementsNode != NULL)
    statementsNode -> visit();

}

//-----------------------------------------

ListVarDecNode::ListVarDecNode(AST* varDecNode, AST* listVarDecNode):
  AST(),
  varDecNode( varDecNode ),
  listVarDecNode( listVarDecNode )
{}

ListVarDecNode::~ListVarDecNode(){
#ifdef debug
  cout << "In ListVarDecNode desctructor!" << endl;
#endif
  
  try{
    delete varDecNode;
    delete listVarDecNode;
  }catch(...){}
}

void ListVarDecNode::visit(){
  if(listVarDecNode != NULL)
    listVarDecNode -> visit();

  varDecNode -> visit();
}

//-----------------------------------------

VarDecNode::VarDecNode(string varKeyword, string varType, AST* listVarNameNode, char semicolonSymbol):
  AST(),
  varKeyword( varKeyword ),
  varType( varType ),
  listVarNameNode( listVarNameNode ),
  semicolonSymbol( semicolonSymbol )
{}

VarDecNode::~VarDecNode(){
#ifdef debug
  cout << "In VarDecNode desctructor!" << endl;
#endif
  
  try{
    delete listVarNameNode;
  }catch(...){}
}

void VarDecNode::visit(){

  listVarNameNode -> visit();

  if(!symbolTableConstructed){
    
    for(int i = 0; i < st -> getVariableListSize(); ++i)
      st -> declareIdentifier(currentSubroutineName, st -> getVariable(i), varType, VAR_K);
    
    //reset the state of the list of names to get the names of the next line
    st -> clearVariableList();
  }
}

//-----------------------------------------

ListOfStatementsNode::ListOfStatementsNode(AST* statementsNode):
  AST(),
  statementsNode( statementsNode )
{}

ListOfStatementsNode::~ListOfStatementsNode(){
#ifdef debug
  cout << "In ListOfStatementsNode desctructor!" << endl;
#endif
  try{
    delete statementsNode;
  }catch(...){}
}

void ListOfStatementsNode::visit(){

  statementsNode -> visit();
}

StatementsNode::StatementsNode(AST* singleStatementNode, AST* statementsNode):
  AST(),
  singleStatementNode( singleStatementNode ),
  statementsNode( statementsNode )
{}

StatementsNode::~StatementsNode(){
#ifdef debug
  cout << "In StatementsNode desctructor!" << endl;
#endif
  
  try{
    delete singleStatementNode;
    delete statementsNode;
  }catch(...){}
}

void StatementsNode::visit(){
  if(statementsNode != NULL)
    statementsNode -> visit();
  
  singleStatementNode -> visit();
}

//-----------------------------------------

SingleStatementNode::SingleStatementNode(AST* letStatementNode, AST* ifStatementNode, AST* whileStatementNode, AST* doStatementNode, AST* returnStatementNode):
  AST(),
  letStatementNode( letStatementNode ),
  ifStatementNode( ifStatementNode ),
  whileStatementNode( whileStatementNode ),
  doStatementNode( doStatementNode ),
  returnStatementNode( returnStatementNode )
{}

SingleStatementNode::~SingleStatementNode(){
#ifdef debug
  cout << "In SingleStatementNode desctructor!" << endl;
#endif

  try{
    delete letStatementNode;
    delete ifStatementNode;
    delete whileStatementNode;
    delete doStatementNode;
    delete returnStatementNode;
  }catch(...){}
}

void SingleStatementNode::visit(){
  
  if(letStatementNode != NULL)
    letStatementNode -> visit();
  else if(ifStatementNode != NULL)
    ifStatementNode -> visit();
  else if(whileStatementNode != NULL)
    whileStatementNode -> visit();
  else if(doStatementNode != NULL)
    doStatementNode -> visit();
  else if(returnStatementNode != NULL)
    returnStatementNode -> visit();
}

//-----------------------------------------

LetStatementNode::LetStatementNode(string letKeyword, string varNameIdentifier, char lb2Symbol, AST* expressionNode, char rb2Symbol, char equalSymbol, AST* expressionNode2, char semicolonSymbol):
  AST(),
  letKeyword( letKeyword ),
  varNameIdentifier( varNameIdentifier ),
  lb2Symbol( lb2Symbol ),
  expressionNode( expressionNode ),
  rb2Symbol( rb2Symbol), 
  equalSymbol( equalSymbol ),
  expressionNode2( expressionNode2 ),
  semicolonSymbol( semicolonSymbol )
{}

LetStatementNode::~LetStatementNode(){
#ifdef debug
  cout << "In LetStatementNode desctructor!" << endl;
#endif

  try{
    delete expressionNode;
    delete expressionNode2;
  }catch(...){}
}

void LetStatementNode::visit(){

  Kind varKind;
  if(symbolTableConstructed){
    varKind = st -> kindOf(currentClassName, currentSubroutineName, varNameIdentifier);
  }
  string varType;

  if(lb2Symbol != NULLCHAR){
    expressionNode -> visit();

    if(symbolTableConstructed){

      switch(varKind){
      case ARGUMENT_K:
	varType = st -> typeOf(currentSubroutineName, varNameIdentifier);
	codeWriter -> writePush("argument", st -> indexOf(currentSubroutineName, varNameIdentifier));
	break;
	
      case VAR_K:
	varType = st -> typeOf(currentSubroutineName, varNameIdentifier);
	codeWriter -> writePush("local", st -> indexOf(currentSubroutineName, varNameIdentifier));
	break;
	
      case STATIC_K:
	varType = st -> typeOf(currentClassName, varNameIdentifier);
	codeWriter -> writePush("static", st -> indexOf(currentClassName, varNameIdentifier));
	break;
	
      case FIELD_K:
	{
	  string srType = st -> getSubroutineType(currentSubroutineName);
	  
	  if(srType == "method" || srType == "constructor"){
	    varType = st -> typeOf(currentClassName, varNameIdentifier);
	    codeWriter -> writePush("this", st -> indexOf(currentClassName, varNameIdentifier));
	    
	  }else{
	    errorFound(FIELD_IN_STATIC, varNameIdentifier);
	  }
	  break;
      }
	
      case NONE_K:
	errorFound(OUT_OF_SCOPE, varNameIdentifier);
      }
      codeWriter -> writeArithmetic("add");
    }
  }

  expressionNode2 -> visit();
  
  if(symbolTableConstructed){
    if(lb2Symbol != NULLCHAR){
      if((varType != "int") && (varType != "char") && (varType != "boolean")){
	codeWriter -> writePop("temp", 0);
	codeWriter -> writePop("pointer", 1);
	codeWriter -> writePush("temp", 0);
	codeWriter -> writePop("that", 0);
      }else{
	errorFound(NON_ARRAY_VARIABLE, varNameIdentifier);
      }
    }else{

      switch(varKind){
      case ARGUMENT_K:
	codeWriter -> writePop("argument", st -> indexOf(currentSubroutineName, varNameIdentifier));
	break;
	
      case VAR_K:
	codeWriter -> writePop("local", st -> indexOf(currentSubroutineName, varNameIdentifier));
	break;
	
      case STATIC_K:
	codeWriter -> writePop("static", st -> indexOf(currentClassName, varNameIdentifier));
	break;
	
      case FIELD_K:
	
	codeWriter -> writePop("this", st -> indexOf(currentClassName, varNameIdentifier));
	break;
      }
    }
  }
}

//--------

IfStatementNode::IfStatementNode(string ifKeyword, char lpSymbol, AST* expressionNode, char rpSymbol, char lbSymbol, AST* listOfStatementsNode, char rbSymbol, string elseKeyword, char lbSymbol2, AST* listOfStatementsNode2, char rbSymbol2):
  AST(),
  ifKeyword( ifKeyword ),
  lpSymbol( lpSymbol ),
  expressionNode( expressionNode ),
  rpSymbol( rpSymbol ),
  lbSymbol( lbSymbol ),
  listOfStatementsNode( listOfStatementsNode ),
  rbSymbol( rbSymbol ),
  elseKeyword( elseKeyword ),
  lbSymbol2( lbSymbol2 ),
  listOfStatementsNode2( listOfStatementsNode2 ),
  rbSymbol2( rbSymbol2 )
{}


IfStatementNode::~IfStatementNode(){
#ifdef debug
  cout << "In IfStatementNode desctructor!" << endl;
#endif

  try{
    delete listOfStatementsNode;
    delete listOfStatementsNode2;
    delete expressionNode;
  }catch(...){}
}

void IfStatementNode::visit(){

  int indexLabel;
  if(symbolTableConstructed){
    indexLabel = trueLabel.back() + 1;
    trueLabel.push_back(indexLabel);
  }

  expressionNode -> visit();

  if(symbolTableConstructed){
    codeWriter -> writeIf("IF_TRUE" + std::to_string(indexLabel));
    codeWriter -> writeGoto("IF_FALSE" + std::to_string(indexLabel));
    codeWriter -> writeLabel("IF_TRUE" + std::to_string(indexLabel));
  }

  listOfStatementsNode -> visit();

  if(symbolTableConstructed){
    if(elseKeyword != NULLSTRING)
      codeWriter -> writeGoto("TRUE_EXECUTED" + std::to_string(indexLabel));
    codeWriter -> writeLabel("IF_FALSE" + std::to_string(indexLabel));
  }

  if(elseKeyword != NULLSTRING){
    listOfStatementsNode2 -> visit();

    if(symbolTableConstructed)
      codeWriter -> writeLabel("TRUE_EXECUTED" + std::to_string(indexLabel));
  }
}

//------

WhileStatementNode::WhileStatementNode(string whileKeyword, char lpSymbol, AST* expressionNode, char rpSymbol, char lbSymbol, AST* listOfStatements, char rbSymbol):
  AST(),
  whileKeyword( whileKeyword ),
  lpSymbol( lpSymbol ),
  expressionNode( expressionNode ), 
  rpSymbol( rpSymbol ),
  lbSymbol( lbSymbol ),
  listOfStatements( listOfStatements ),
  rbSymbol( rbSymbol )
{}

WhileStatementNode::~WhileStatementNode(){
#ifdef debug
  cout << "In WhileStatementNode desctructor!" << endl;
#endif

  try{
    delete listOfStatements;
    delete expressionNode;
  }catch(...){}
}

void WhileStatementNode::visit(){

  int indexLabel;
  
  if(symbolTableConstructed){
    indexLabel = trueLabel.back() + 1;
    trueLabel.push_back(indexLabel);
  }

  if(symbolTableConstructed)
    codeWriter -> writeLabel("WHILE_TRUE" + std::to_string(indexLabel));
  
  expressionNode -> visit();

  if(symbolTableConstructed){
    codeWriter -> writeIf("WHILE_CODE" + std::to_string(indexLabel));
    codeWriter -> writeGoto("WHILE_FALSE" + std::to_string(indexLabel));
    codeWriter -> writeLabel("WHILE_CODE" + std::to_string(indexLabel));
  }

  listOfStatements -> visit();

  if(symbolTableConstructed){
    codeWriter -> writeGoto("WHILE_TRUE" + std::to_string(indexLabel));
    codeWriter -> writeLabel("WHILE_FALSE" + std::to_string(indexLabel));
  }
}

//--------

DoStatementNode::DoStatementNode(string doKeyword, AST* subroutineCallNode, char semicolonSymbol):
  AST(),
  doKeyword( doKeyword ),
  subroutineCallNode( subroutineCallNode ),
  semicolonSymbol( semicolonSymbol )
{}

DoStatementNode::~DoStatementNode(){
#ifdef debug
  cout << "In DoStatementNode desctructor!" << endl;
#endif

  try{
    delete subroutineCallNode;
  }catch(...){}
}

void DoStatementNode::visit(){

  subroutineCallNode -> visit();

  if(symbolTableConstructed)
    codeWriter -> writePop("temp", 0);
}

//------

ReturnStatementNode::ReturnStatementNode(string returnKeyword, AST* expressionNode, char semicolonSymbol):
  AST(),
  returnKeyword( returnKeyword ),
  expressionNode( expressionNode ),
  semicolonSymbol( semicolonSymbol )
{}

ReturnStatementNode::~ReturnStatementNode(){
#ifdef debug
  cout << "In ReturnStatementNode desctructor!" << endl;
#endif

  try{
    delete expressionNode;
  }catch(...){}
}

void ReturnStatementNode::visit(){

  if(expressionNode != NULL)
    expressionNode -> visit();
  else{
    if(symbolTableConstructed)
      codeWriter -> writePush("constant", 0);
  }

  if(symbolTableConstructed)
    codeWriter -> writeReturn();
}

//-----------------------------------------

ExpressionNode::ExpressionNode(AST* operatorListNode):
  AST(),
  operatorListNode( operatorListNode )
{}

ExpressionNode::~ExpressionNode(){
#ifdef debug
  cout << "In ExpressionNode desctructor!" << endl;
#endif
  
  try{
    delete operatorListNode;
  }catch(...){}
}

void ExpressionNode::visit(){
  operatorListNode -> visit();
}

//--------

TermNode::TermNode(int intConst, string stringConst, string keywordConst, string varNameIdentifier, char lb2Symbol, AST* expressionNode, char rb2Symbol, AST* subroutineCallNode, char lpSymbol, AST* expressionNode2, char rpSymbol, char unaryOpSymbol, AST* termNode):
  AST(),
  intConst( intConst ),
  stringConst( stringConst ),
  keywordConst( keywordConst ),
  varNameIdentifier( varNameIdentifier ),
  lb2Symbol( lb2Symbol ),
  expressionNode( expressionNode ),
  rb2Symbol( rb2Symbol ),
  subroutineCallNode( subroutineCallNode ),
  lpSymbol( lpSymbol ),
  expressionNode2( expressionNode2 ),
  rpSymbol( rpSymbol ),
  unaryOpSymbol( unaryOpSymbol ),
  termNode( termNode )
{}

TermNode::~TermNode(){
#ifdef debug
  cout << "In TermNode desctructor!" << endl;
#endif

  try{
    delete expressionNode;
    delete termNode;
  }catch(...){}
}

void TermNode::visit(){

  if(unaryOpSymbol != NULLCHAR){
    termNode -> visit();

    if(symbolTableConstructed){
      if(unaryOpSymbol == '~')
	codeWriter -> writeArithmetic("not");
      else
	codeWriter -> writeArithmetic("neg");
    }
    
  }else if(intConst != NULLINT){

    if(symbolTableConstructed)
      codeWriter -> writePush("constant", intConst);
  }
  else if(stringConst != NULLSTRING){

    if(symbolTableConstructed){
      codeWriter -> writePush("constant", stringConst.size());
      codeWriter -> writeCall("String.new", 1);
      
      for(int i = 0; i < stringConst.size(); ++i){
	codeWriter -> writePush("constant", (int)stringConst.at(i));
	codeWriter -> writeCall("String.appendChar", 2);
      }
    }
  }
  else if(keywordConst != NULLSTRING){

    if(symbolTableConstructed){
      if(keywordConst == "true"){
	//push -1
	codeWriter -> writePush("constant", 1);
	codeWriter -> writeArithmetic("neg");
	
      }else if(keywordConst == "null" || keywordConst == "false")
	codeWriter -> writePush("constant", 0);
      else //keywordConst = this
	codeWriter -> writePush("pointer", 0);
    }
  }
  else if(varNameIdentifier != NULLSTRING){

    if(lb2Symbol != NULLCHAR)
      expressionNode -> visit();

    if(symbolTableConstructed){
      
      Kind varKind = st -> kindOf(currentClassName, currentSubroutineName, varNameIdentifier);
      
      switch(varKind){
      case ARGUMENT_K:
	codeWriter -> writePush("argument", st -> indexOf(currentSubroutineName, varNameIdentifier));
	break;
	
      case VAR_K:
	codeWriter -> writePush("local", st -> indexOf(currentSubroutineName, varNameIdentifier));
	break;
	
      case STATIC_K:
	codeWriter -> writePush("static", st -> indexOf(currentClassName, varNameIdentifier));
	break;
	
      case FIELD_K:
	{
	  string srType = st -> getSubroutineType(currentSubroutineName);
	  if(srType == "method" || srType == "constructor"){
	    //codeWriter -> writePush("argument", 0);
	    //codeWriter -> writePop("pointer", 0);
	    codeWriter -> writePush("this", st -> indexOf(currentClassName, varNameIdentifier));
	  }else{
	    errorFound(FIELD_IN_STATIC, varNameIdentifier);}
	  break;
	}
      case NONE_K:
	errorFound(OUT_OF_SCOPE, varNameIdentifier);
      }

      if(lb2Symbol != NULLCHAR){ //if there is a varName[expression]
	if(symbolTableConstructed){
	  string varType;
	  if(varKind == STATIC_K || varKind == FIELD_K){
	    varType = st -> typeOf(currentClassName, varNameIdentifier);
	  }else{
	    varType = st -> typeOf(currentSubroutineName, varNameIdentifier);	  
	  }
	  
	  if(varType != "Array")
	    errorFound(NON_ARRAY_VARIABLE, varNameIdentifier);
	  else{
	    codeWriter -> writeArithmetic("add");
	    codeWriter -> writePop("pointer", 1);
	    codeWriter -> writePush("that", 0);
	  }
	}
      }
    }
    
  }else if(subroutineCallNode != NULL)
    subroutineCallNode -> visit();
    
  else if(lpSymbol != NULLCHAR)
    expressionNode2 -> visit();
  
}

//--------

OperatorListNode::OperatorListNode(char operatorSymbol, AST* termNode, AST* operatorListNode):
  AST(),
  operatorSymbol( operatorSymbol ),
  termNode( termNode ),
  operatorListNode( operatorListNode )
{}

OperatorListNode::~OperatorListNode(){
#ifdef debug
  cout << "In OperatorListNode desctructor!" << endl;
#endif

  try{
    delete termNode;
    delete operatorListNode;
  }catch(...){}
}

void OperatorListNode::visit(){
  if(operatorListNode != NULL)
    operatorListNode -> visit();

  termNode -> visit();
  
  if(operatorSymbol != NULLCHAR){

    if(symbolTableConstructed){
      switch(operatorSymbol){
      case '+':
	codeWriter -> writeArithmetic("add");
	break;
      case '-':
	codeWriter -> writeArithmetic("sub");
	break;
      case '*':
	codeWriter -> writeCall("Math.multiply", 2);
	break;
      case '/':
	codeWriter -> writeCall("Math.divide", 2);
	break;
      case '|':
	codeWriter -> writeArithmetic("or");
	break;
      case '=':
	codeWriter -> writeArithmetic("eq");
	break;
      case '&':
	codeWriter -> writeArithmetic("and");
	break;
      case '<':
	codeWriter -> writeArithmetic("lt");
	break;
      case '>':
	codeWriter -> writeArithmetic("gt");
	break;
      }
    }
  }
}

//-----------------------------------------

SubroutineCallNode::SubroutineCallNode(string varName, char pointSymbol, string subroutineName, char lpSymbol, AST* expressionListNode, char rpSymbol):
  AST(),
  varName( varName ),
  pointSymbol( pointSymbol ),
  subroutineName( subroutineName ),
  lpSymbol( lpSymbol ),
  expressionListNode( expressionListNode ),
  rpSymbol( rpSymbol )
{}

SubroutineCallNode::~SubroutineCallNode(){
#ifdef debug
  cout << "In SubroutineCallNode desctructor!" << endl;
#endif

  try{
    delete expressionListNode;
  }catch(...){}
}

void SubroutineCallNode::visit(){

  bool hasVarName = false; //you erase it later
  
  string varType;

  if(symbolTableConstructed){

    if(varName != NULLSTRING){
      Kind varKind;

      if(!(st -> isClassType(varName))){
	varKind = st -> kindOf(currentClassName, currentSubroutineName, varName);
	
	switch(varKind){
	  
	case STATIC_K:
	  varType = st -> typeOf(currentClassName, varName);
	  break;
	  
	case FIELD_K:
	  {
	    string srType = st -> getSubroutineType(currentSubroutineName);
	    if(srType == "method" || srType == "constructor")
	      varType = st -> typeOf(currentClassName, varName);
	    else
	      errorFound(FIELD_IN_STATIC, varName);
	    break;
	  }
	  
	case VAR_K:
	  varType = st -> typeOf(currentSubroutineName, varName);
	  break;
	  
	case ARGUMENT_K:
	  varType = st -> typeOf(currentSubroutineName, varName);
	  break;
	  
	case NONE_K:
	  errorFound(OUT_OF_SCOPE, varName);
	}
      
	string srName = varType + "." + subroutineName;
      
	if((varType != "int") && (varType != "char") && (varType != "boolean")){
	  if(st -> isValidSubroutine( srName )){
	    if(st -> getSubroutineType( srName ) == "method"){
	      switch(varKind){
		
	      case STATIC_K:
		codeWriter -> writePush("static", st -> indexOf(currentClassName, varName));
		break;
		
	      case FIELD_K:
		//codeWriter -> writePush("argument", 0);
		//codeWriter -> writePop("pointer", 0);
		codeWriter -> writePush("this", st -> indexOf(currentClassName, varName));
		break;
		
	      case VAR_K:
		codeWriter -> writePush("local", st -> indexOf(currentSubroutineName, varName));
		break;
		
	      case ARGUMENT_K:
		codeWriter -> writePush("argument", st -> indexOf(currentSubroutineName, varName));
		break;
	      }
	    }
	  }else{
	    errorFound(SB_NOT_FOUND, srName);
	  }
	}else{
	  errorFound( IT_IN_SB_CALL, srName );      
	}
      }else
	varType = varName; //varName is = to a className
     
    }else{//calling a function inside the same class
      if(st -> isValidSubroutine(currentClassName + "." + subroutineName))
	codeWriter -> writePush("pointer", 0);
      else{
	errorFound(SB_NOT_FOUND, currentClassName + "." + subroutineName);
      }
    }    
  }
    
  if(expressionListNode != NULL)
      expressionListNode -> visit();
  else{
    //printTag("expressionList");
    //printTag("/expressionList");
  }
  
  if(varName != NULLSTRING){
    hasVarName = true;

    if(symbolTableConstructed){
      
      string srName = varType + "." + subroutineName;

      if(st -> isValidSubroutine(srName)){

	string srType = st -> getSubroutineType(srName);
	if(st -> isClassType(varName)){
	  if(srType == "function" || srType == "constructor")
	    codeWriter -> writeCall(srName, st -> getNumOfVars(srName, ARGUMENT_K));
	  else{
	    errorFound(WSB_CALL, srName);
	  }
	  
	}else{

	  if(srType == "method"){
	    codeWriter -> writeCall( srName, st -> getNumOfVars( srName, ARGUMENT_K) );
	  }else{
	    errorFound(WSB_CALL2, srName);
	  }
	}
      }else{
	errorFound(SB_NOT_FOUND, srName);
      }
    } 
  }

  if(symbolTableConstructed){
    if(!hasVarName){
      string srName = currentClassName + "." + subroutineName;
      if(st -> isValidSubroutine( srName )){
  	codeWriter -> writeCall(srName, st -> getNumOfVars(srName, ARGUMENT_K));
      }else{
	errorFound(SB_NOT_FOUND, srName);
      }
    }
  }
}

//-----------------------------------------------------

ExpressionListNodeAux::ExpressionListNodeAux(AST* expressionListNode):
  AST(),
  expressionListNode( expressionListNode )
{}

ExpressionListNodeAux::~ExpressionListNodeAux(){
#ifdef debug
  cout << "In ExpressionListNodeAux desctructor!" << endl;
#endif

  try{
    delete expressionListNode;
  }catch(...){}
}

void ExpressionListNodeAux::visit(){

  expressionListNode -> visit();
}

ExpressionListNode::ExpressionListNode(char commaSymbol, AST* expressionNode, AST* expressionListNode):
  AST(),
  commaSymbol( commaSymbol ),
  expressionNode( expressionNode ),
  expressionListNode( expressionListNode )
{}

ExpressionListNode::~ExpressionListNode(){
#ifdef debug
  cout << "In ExpressionListNode desctructor!" << endl;
#endif

  try{
    delete expressionNode;
    delete expressionListNode;
  }catch(...){}
}

void ExpressionListNode::visit(){
  if(expressionListNode != NULL){
    expressionListNode -> visit();
    expressionNode -> visit();
  }else
    expressionNode -> visit();
}

//-----------------------------------------------------

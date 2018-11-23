#include "../include/ast.h"

//-----------------------------------------
//UTILITIES

int numOfTabs = 0;

void printToken(string tokenType, string token){
  for(int i = 0; i < numOfTabs; ++i)
    parserFileOut << "  ";
  
  parserFileOut << "<" << tokenType << "> ";
  parserFileOut << token;
  parserFileOut << " </" << tokenType << ">" << endl;
}

void printTag(string tag){
  for(int i = 0; i < numOfTabs; ++i)
    parserFileOut << "  ";
  parserFileOut << "<" << tag << ">" << endl;
}

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
  printTag("class");
  
  ++numOfTabs;
  
  classNode -> visit();
  printTag("/class");
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
  printToken("keyword", classKeyword);
  printToken("identifier", classIdentifier);
  printToken("symbol", string(1, lbSymbol));
  if(listClassVarDecNode != NULL)
    listClassVarDecNode -> visit();
  if(subroutineDecNode != NULL)
    subroutineDecNode -> visit();
  printToken("symbol", string(1, rbSymbol));
  
  --numOfTabs;
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
  printTag("classVarDec");
  ++numOfTabs;
  
  printToken("keyword", staticOrFieldKeyword);
  if(classVarType == "int" || classVarType == "char" || classVarType == "boolean")
    printToken("keyword", classVarType);
  else
    printToken("identifier", classVarType);
  listVarNameNode -> visit();
  printToken("symbol", string(1, semicolon));

  --numOfTabs;
  printTag("/classVarDec");
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

  if(varName != NULLSTRING)
    printToken("identifier", varName);
  if(commaSymbol != NULLCHAR){
    printToken("symbol", string(1, commaSymbol));
    printToken("identifier", varName2);
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
  printTag("subroutineDec");
  ++numOfTabs;
  
  printToken("keyword", procedureType);
  if(returnType == "int" || returnType == "char" || returnType == "boolean" || returnType == "void")
    printToken("keyword", returnType);
  else
    printToken("identifier", returnType);
  printToken("identifier", subroutineName);
  printToken("symbol", string(1,lpSymbol));

  printTag("parameterList");
  if(parameterListNode != NULL){
    ++numOfTabs;
    parameterListNode -> visit();
    --numOfTabs;
  }
  printTag("/parameterList");
  
  printToken("symbol", string(1, rpSymbol));
  subroutineBodyNode -> visit();

  --numOfTabs;
  printTag("/subroutineDec");
  
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
  if(commaSymbol != NULLCHAR)
    printToken("symbol", string(1, commaSymbol));
  
  if(varType == "int" || varType == "char" || varType == "boolean")
    printToken("keyword", varType);
  else
    printToken("identifier", varType);
  
    printToken("identifier", varName);
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
  printTag("subroutineBody");
  ++numOfTabs;

  printToken("symbol", string(1, lbSymbol));
  if(listVarDecNode != NULL)
    listVarDecNode -> visit();

  if(statementsNode != NULL)
    statementsNode -> visit();

  printToken("symbol", string(1, rbSymbol));

  --numOfTabs;
  printTag("/subroutineBody");
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
  printTag("varDec");
  ++numOfTabs;
  
  printToken("keyword", varKeyword);
  if(varType == "int" || varType == "boolean" || varType == "char")
    printToken("keyword", varType);
  else
    printToken("identifier", varType);

  listVarNameNode -> visit();
  printToken("symbol", string(1, semicolonSymbol));

  --numOfTabs;
  printTag("/varDec");
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
  printTag("statements");
  ++numOfTabs;

  statementsNode -> visit();

  --numOfTabs;
  printTag("/statements");
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
  printTag("letStatement");
  ++numOfTabs;

  printToken("keyword", letKeyword);
  printToken("identifier", varNameIdentifier);

  if(lb2Symbol != NULLCHAR){
    printToken("symbol", string(1, lb2Symbol));
    expressionNode -> visit();
    printToken("symbol", string(1, rb2Symbol));
  }

  printToken("symbol", string(1, equalSymbol));
  expressionNode2 -> visit();
  printToken("symbol", string(1, semicolonSymbol));

  --numOfTabs;
  printTag("/letStatement");
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
  printTag("ifStatement");
  ++numOfTabs;

  printToken("keyword", ifKeyword);
  printToken("symbol", string(1, lpSymbol));
  expressionNode -> visit();
  printToken("symbol", string(1, rpSymbol));
  printToken("symbol", string(1, lbSymbol));

  listOfStatementsNode -> visit();

  printToken("symbol", string(1, rbSymbol));

  if(elseKeyword != NULLSTRING){
    printToken("keyword", elseKeyword);
    printToken("symbol", string(1, lbSymbol2));
    listOfStatementsNode2 -> visit();
    printToken("symbol", string(1, rbSymbol2));
  }

  --numOfTabs;
  printTag("/ifStatement");
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
  printTag("whileStatement");
  ++numOfTabs;

  printToken("keyword", whileKeyword);
  printToken("symbol", string(1, lpSymbol));
  expressionNode -> visit();
  printToken("symbol", string(1, rpSymbol));
  printToken("symbol", string(1, lbSymbol));

  listOfStatements -> visit();

  printToken("symbol", string(1, rbSymbol));

  --numOfTabs;
  printTag("/whileStatement");
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
    //delete subroutineCallNode;
  }catch(...){}
}

void DoStatementNode::visit(){
  printTag("doStatement");
  ++numOfTabs;

  printToken("keyword", doKeyword);
  subroutineCallNode -> visit();

  printToken("symbol", string(1, semicolonSymbol));

  --numOfTabs;
  printTag("/doStatement");
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
    //delete expressionNode;
  }catch(...){}
}

void ReturnStatementNode::visit(){
  printTag("returnStatement");
  ++numOfTabs;

  printToken("keyword", returnKeyword);

  if(expressionNode != NULL)
    expressionNode -> visit();
  printToken("symbol", string(1, semicolonSymbol));

  --numOfTabs;
  printTag("/returnStatement");
}

//-----------------------------------------

ExpressionNode::ExpressionNode(AST* operatorListNode):
  AST(),
  //termNode( termNode ),
  operatorListNode( operatorListNode )
{}

ExpressionNode::~ExpressionNode(){
#ifdef debug
  cout << "In ExpressionNode desctructor!" << endl;
#endif
  
  try{
    //delete termNode;
    delete operatorListNode;
  }catch(...){}
}

void ExpressionNode::visit(){
  printTag("expression");
  ++numOfTabs;
  
  operatorListNode -> visit();

  --numOfTabs;
  printTag("/expression");
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
  printTag("term");
  ++numOfTabs;

  if(unaryOpSymbol != NULLCHAR){
    printToken("symbol", string(1, unaryOpSymbol));
    termNode -> visit();
  }else if(intConst != NULLINT)
    printToken("integerConstant", std::to_string(intConst));
  else if(stringConst != NULLSTRING)
    printToken("stringConstant", stringConst);
  else if(keywordConst != NULLSTRING)
    printToken("keyword", keywordConst);
  else if(varNameIdentifier != NULLSTRING){
    printToken("identifier", varNameIdentifier);
    if(lb2Symbol != NULLCHAR){
      printToken("symbol", string(1, lb2Symbol));
      expressionNode -> visit();
      printToken("symbol", string(1, rb2Symbol));      
    }
  }else if(subroutineCallNode != NULL){
    subroutineCallNode -> visit();
  }else if(lpSymbol != NULLCHAR){
    printToken("symbol", string(1, lpSymbol));
    expressionNode2 -> visit();
    printToken("symbol", string(1, rpSymbol));
  }
  
  --numOfTabs;
  printTag("/term");
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
  
  if(operatorSymbol != NULLCHAR)
    if(operatorSymbol == '<')
      printToken("symbol", "&lt;");
    else if(operatorSymbol == '>')
      printToken("symbol", "&gt;");
    else if(operatorSymbol == '&')
      printToken("symbol", "&amp;");
    else
      printToken("symbol", string(1, operatorSymbol));
  termNode -> visit();
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
  if(varName != NULLSTRING){
    printToken("identifier", varName);
    printToken("symbol", string(1, pointSymbol));
  }
  printToken("identifier", subroutineName);
  printToken("symbol", string(1, lpSymbol));
  if(expressionListNode != NULL)
    expressionListNode -> visit();
  else{
    printTag("expressionList");
    printTag("/expressionList");
  }
  printToken("symbol", string(1, rpSymbol));
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
  printTag("expressionList");
  ++numOfTabs;

  expressionListNode -> visit();

  --numOfTabs;
  printTag("/expressionList");
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
    printToken("symbol", string(1, commaSymbol));
    expressionNode -> visit();
  }else
    expressionNode -> visit();
}

//-----------------------------------------------------

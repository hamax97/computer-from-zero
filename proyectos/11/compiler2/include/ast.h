#ifndef AST_H
#define AST_H

#include <string>
#include <iostream>
#include <fstream>
#include "../include/symboltable.h"

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::ofstream;

#define NULLSTRING ""
#define NULLCHAR char(0)
#define NULLINT 777777

extern ofstream parserFileOut;

//-----------------------------------------
class AST{
 public:
  AST();
  virtual ~AST() = 0;
  virtual void visit() = 0;
};
//-----------------------------------------

class ProgNode : public AST{
 public:
  ProgNode(AST* classNode);
  ~ProgNode();
  void visit();

 private:
  AST* classNode;
};

//-----------------------------------------

class ClassNode : public AST{
 public:
  ClassNode(string classKeyword, string classIdentifier, char lbSymbol, AST* listClassVarDecNode, AST* subroutineDecNode, char rbSymbol);
  ~ClassNode();
  void visit();

 private:
  string classKeyword;
  string classIdentifier;
  char lbSymbol;
  AST* listClassVarDecNode;
  AST* subroutineDecNode;
  char rbSymbol;
};

//-----------------------------------------

class ListClassVarDecNode : public AST{
 public:
  ListClassVarDecNode(AST* classVarDecNode, AST* listClassVarDecNode);
  ~ListClassVarDecNode();
  void visit();

 private:
  AST* classVarDecNode;
  AST* listClassVarDecNode;
};

class ClassVarDecNode : public AST{
 public:
  ClassVarDecNode(string staticOrFieldKeyword, string classVarType, AST* listVarNameNode, char semicolon);
  ~ClassVarDecNode();
  void visit();

 private:
  string staticOrFieldKeyword;
  string classVarType;
  AST* listVarNameNode;
  char semicolon;
};

//-----------------------------------------

class ListVarNameNode : public AST{
 public:
  ListVarNameNode(string varName, char commaSymbol, string varName2, AST* listVarNameNode);
  ~ListVarNameNode();
  void visit();

 private:
  string varName;
  char commaSymbol;
  string varName2;
  AST* listVarNameNode;
};

//-----------------------------------------

class ListSubroutineDecNode : public AST{
 public:
  ListSubroutineDecNode(AST* subroutineDecNode, AST* listClassVarDecNode);
  ~ListSubroutineDecNode();
  void visit();

 private:
  AST* subroutineDecNode;
  AST* listSubroutineDecNode;
};

class SubroutineDecNode : public AST {
 public:
  SubroutineDecNode(string procedureType, string returnType, string subroutineName, char lpSymbol, AST* parameterListNode, char rpSymbol, AST* subroutineBodyNode);
  ~SubroutineDecNode();
  void visit();

 private:
  string procedureType;
  string returnType;
  string subroutineName;
  char lpSymbol;
  AST* parameterListNode;
  char rpSymbol;
  AST* subroutineBodyNode;
};

//-----------------------------------------

class ParameterListNode : public AST{
 public:
  ParameterListNode(string varType, string varName, char commaSymbol, AST* parameterListNode);
  ~ParameterListNode();
  void visit();

 private:
  string varType;
  string varName;
  char commaSymbol;
  AST* parameterListNode;
};

//-----------------------------------------

class SubroutineBodyNode : public AST{
 public:
  SubroutineBodyNode(char lbSymbol, AST* listVarDecNode, AST* statementsNode, char rbSymbol);
  ~SubroutineBodyNode();
  void visit();

 private:
  char lbSymbol;
  AST* listVarDecNode;
  AST* statementsNode;
  char rbSymbol;
};

//-----------------------------------------

class ListVarDecNode : public AST{
 public:
  ListVarDecNode(AST* varDecNode, AST* listVarDecNode);
  ~ListVarDecNode();
  void visit();

 private:
  AST* varDecNode;
  AST* listVarDecNode;
};

//-----------------------------------------

class VarDecNode : public AST{
 public:
  VarDecNode(string varKeyword, string varType, AST* listVarNameNode, char semicolonSymbol);
  ~VarDecNode();
  void visit();

 private:
  string varKeyword;
  string varType;
  AST* listVarNameNode;
  char semicolonSymbol;
};

//-----------------------------------------

class ListOfStatementsNode : public AST{
 public:
  ListOfStatementsNode(AST* statementsNode);
  ~ListOfStatementsNode();
  void visit();

 private:
  AST* statementsNode;
};

class StatementsNode : public AST{
 public:
  StatementsNode(AST* singleStatementNode, AST* statementsNode);
  ~StatementsNode();
  void visit();

 private:
  AST* singleStatementNode;
  AST* statementsNode;
};

class SingleStatementNode : public AST{
 public:
  SingleStatementNode(AST* letStatementNode, AST* ifStatementNode, AST* whileStatementNode, AST* doStatementNode, AST* returnStatementNode);
  ~SingleStatementNode();
  void visit();

 private:
  AST* letStatementNode;
  AST* ifStatementNode;
  AST* whileStatementNode;
  AST* doStatementNode;
  AST* returnStatementNode;
};

//-----------------------------------------

class LetStatementNode : public AST{
 public:
  LetStatementNode(string letKeyword, string varNameIdentifier, char lb2Symbol, AST* expressionNode, char rb2Symbol, char equalSymbol, AST* expressionNode2, char semicolonSymbol);
  ~LetStatementNode();
  void visit();

 private:
  string letKeyword;
  string varNameIdentifier;
  char lb2Symbol;
  AST* expressionNode;
  char rb2Symbol;
  char equalSymbol;
  AST* expressionNode2;
  char semicolonSymbol;
};

class IfStatementNode : public AST{
 public:
  IfStatementNode(string ifKeyword, char lpSymbol, AST*expressionNode, char rpSymbol, char lbSymbol, AST* listOfStatementsNode, char rbSymbol, string elseKeyword, char lbSymbol2, AST* listOfStatementsNode2, char rbSymbol2);
  ~IfStatementNode();
  void visit();

 private:
  string ifKeyword;
  char lpSymbol;
  AST* expressionNode;
  char rpSymbol;
  char lbSymbol;
  AST* listOfStatementsNode;
  char rbSymbol;
  string elseKeyword;
  char lbSymbol2;
  AST* listOfStatementsNode2;
  char rbSymbol2;
};

class WhileStatementNode : public AST{
 public:
  WhileStatementNode(string whileKeyword, char lpSymbol, AST* expressionNode, char rpSymbol, char lbSymbol, AST* listOfStatements, char rbSymbol);
  ~WhileStatementNode();
  void visit();

 private:
  string whileKeyword;
  char lpSymbol;
  AST* expressionNode;
  char rpSymbol;
  char lbSymbol;
  AST* listOfStatements;
  char rbSymbol;
};

class DoStatementNode : public AST{
 public:
  DoStatementNode(string doKeyword, AST* subroutineCallNode, char semicolonSymbol);
  ~DoStatementNode();
  void visit();

 private:
  string doKeyword;
  AST* subroutineCallNode;
  char semicolonSymbol;
};

class ReturnStatementNode : public AST{
 public:
  ReturnStatementNode(string returnKeyword, AST* expressionNode, char semicolonSymbol);
  ~ReturnStatementNode();
  void visit();

 private:
  string returnKeyword;
  AST* expressionNode;
  char semicolonSymbol;
};

//-----------------------------------------

class ExpressionNode : public AST{
 public:
  ExpressionNode(AST* operatorListNode);
  ~ExpressionNode();
  void visit();

 private:
  AST* operatorListNode;
};

class TermNode : public AST{
 public:
  TermNode(int intConst, string stringConst, string keywordConst, string varNameIdentifier, char lb2Symbol, AST* expressionNode, char rb2Symbol, AST* subroutineCallNode, char lpSymbol, AST* expressionNode2, char rpSymbol, char unaryOpSymbol, AST* termNode);
  ~TermNode();
  void visit();

 private:
  int intConst;
  string stringConst;
  string keywordConst;
  string varNameIdentifier;
  char lb2Symbol;
  AST* expressionNode;
  char rb2Symbol;
  AST* subroutineCallNode;
  char lpSymbol;
  AST* expressionNode2;
  char rpSymbol;
  char unaryOpSymbol;
  AST* termNode;
};

class OperatorListNode : public AST{
 public:
  OperatorListNode(char operatorSymbol, AST* termNode, AST* operatorListNode);
  ~OperatorListNode();
  void visit();

 private:
  char operatorSymbol;
  AST* termNode;
  AST* operatorListNode;
};

//-----------------------------------------------------

class SubroutineCallNode : public AST{
 public:
  SubroutineCallNode(string varName, char pointSymbol, string subroutineName, char lpSymbol, AST* expressionListNode, char rpSymbol);
  ~SubroutineCallNode();
  void visit();

 private:
  string varName;
  char pointSymbol;
  string subroutineName;
  char lpSymbol;
  AST* expressionListNode;
  char rpSymbol;
};

//-----------------------------------------------------

class ExpressionListNodeAux : public AST{
 public:
  ExpressionListNodeAux(AST* expressionListNode);
  ~ExpressionListNodeAux();
  void visit();

 private:
  AST* expressionListNode;
};

class ExpressionListNode : public AST{
 public:
  ExpressionListNode(char commaSymbol, AST* expressionNode, AST* expressionListNode);
  ~ExpressionListNode();
  void visit();

 private:
  char commaSymbol;
  AST* expressionNode;
  AST* expressionListNode;
};

//-----------------------------------------
#endif

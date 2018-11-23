%{
  #include "../include/errors.h"
  #include "../include/ast.h"
  
  extern "C" int yylex();
  extern "C" int yyparse();

  extern string currentFileDebug;

  //variable used in the main function to control the creation of a parsed file
  bool noError = true;
  
  void yyerror (char const *s){
    cerr << s << ". Found: " << yytext
	 << ". In file: " << currentFileDebug
	 << ". At line " << yylineno << endl;
    exit(1);
    noError = false;
  }

  void errorFound(Error errorType, string var = ""){
    cerr << "Error: ";
    switch(errorType){
    case INT_OVERFLOW:
      cerr << "Integer Overflow!. In file: " << currentFileDebug
	   << ". At line: " << yylineno << "." <<endl;
      break;
    case OUT_OF_SCOPE:
      cerr << "In File: " << currentFileDebug
	   << ". Variable: " << var
	   << " -> was not declared in this scope." << endl;
      break;
    case NON_ARRAY_VARIABLE:
      cerr << "In File: " << currentFileDebug
	   << ". Variable: " << var << " -> has not the operator "
	   << "[], because is not of type Array." << endl;
      break;
    case SB_NOT_FOUND:
      cerr << "In File: " << currentFileDebug
	   << ". Subroutine: " << var << " -> does not exist." << endl;
      break;
    case IT_IN_SB_CALL:
      cerr << "In File: " << currentFileDebug
	   << ". <int | char | boolean> is a primitive type." << endl;
      break;
    case WSB_CALL:
      cerr << "In File: " << currentFileDebug
	   << ". Calling subroutine: " << var
	   << " -> as a function/constructor." << endl;
      break;
    case WSB_CALL2:
      cerr << "In File: " << currentFileDebug
	   << ". Calling subroutine: " << var
	   << " -> as a method." << endl;
      break;
    case FIELD_IN_STATIC:
      cerr << "In File: " << currentFileDebug
	   << ". Using field attribute: " << var
	   << " -> in a non constructor/method procedure." << endl;
      break;
    }

    noError = false;
    exit(1);
  }

  AST* prog;
  
%}

%union{
  char *identifier;
  char *keyword;
  char *stringConst;
  int intConst;
  char symbol;
  AST* ast;
}

 /* declare the type of the tokens */
%token <symbol> LB RB LP RP LB2 RB2 POINT COMMA SEMICOLON PLUS MINUS TIMES DIVIDE AND OR LT GT EQUAL NEG
%token <identifier> CLASS CONSTRUCTOR FUNCTION METHOD FIELD STATIC VAR INT CHAR BOOLEAN VOID TRUE FALSE nULL THIS LET DO IF ELSE WHILE RETURN IDENTIFIER ABC
%token <stringConst> STRING_CONST
%token <intConst> INT_CONST

 /* declare the return type of the non terminals */
%type <identifier> CLASS_TOKEN VAR_TOKEN VOID_TOKEN LET_TOKEN DO_TOKEN IF_TOKEN ELSE_TOKEN WHILE_TOKEN RETURN_TOKEN TYPE PROCEDURE_TYPE SUBROUTINE_NAME CLASS_NAME STATIC_TOKEN FIELD_TOKEN VAR_NAME

%type <keyword> KEYWORD_CONST

%type <symbol> LEFT_BRACKET RIGHT_BRACKET LEFT_PAREN RIGHT_PAREN LEFT_BRACKET2 RIGHT_BRACKET2 POINT_SYM EQUAL_SYM OPERATOR UNARY_OP COMMA_SYM SEMICOLON_SYM 

%type <ast> PROG CLASS_VAR_DEC LIST_VAR_NAME SUBROUTINE_DEC PARAMETER_LIST PARAMS_LIST_AUX LIST_CLASS_VAR_DEC LIST_SUBROUTINE_DEC LIST_VAR_DEC VAR_DEC SUBROUTINE_BODY STATEMENTS STATEMENT LET_STATEMENT IF_STATEMENT WHILE_STATEMENT DO_STATEMENT RETURN_STATEMENT LIST_OF_STATEMENTS TERM OP_LIST EXPRESSION SUBROUTINE_CALL EXPRESSION_LIST EXPRESSION_LIST_AUX

%token END_OF_FILE 0

%start JACK

%%

JACK:
	PROG END_OF_FILE
	{
	  prog = new ProgNode($1);
	}
	;

PROG:
	CLASS_TOKEN CLASS_NAME LEFT_BRACKET LIST_CLASS_VAR_DEC LIST_SUBROUTINE_DEC RIGHT_BRACKET
	{
	  $$ = new ClassNode($1, $2, $3, $4, $5, $6);
	}
	;

LIST_CLASS_VAR_DEC:
        %empty
        {
	  $$ = NULL;
	}
	| LIST_CLASS_VAR_DEC CLASS_VAR_DEC
	{
	  $$ = new ListClassVarDecNode($2, $1);
	}
	;

CLASS_VAR_DEC:
        STATIC_TOKEN TYPE LIST_VAR_NAME SEMICOLON_SYM
	{
	  $$ = new ClassVarDecNode($1, $2, $3, $4);
	}
	| FIELD_TOKEN TYPE LIST_VAR_NAME SEMICOLON_SYM
	{
	  $$ = new ClassVarDecNode($1, $2, $3, $4);
	}
	;

LIST_VAR_NAME:
	VAR_NAME
	{
	  $$ = new ListVarNameNode($1, NULLCHAR, NULLSTRING, NULL);
	}
	| LIST_VAR_NAME COMMA_SYM VAR_NAME
	{
	  $$ = new ListVarNameNode(NULLSTRING, $2, $3, $1);
	}
	;

TYPE:
	INT
	{ 
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	| CHAR
	{ 
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
 	}
	| BOOLEAN
	{ 
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
 	}
	| CLASS_NAME
	{ 
	  $$ = $1;
 	}
	;

LIST_SUBROUTINE_DEC:
	%empty
        {$$ = NULL;}
	| LIST_SUBROUTINE_DEC SUBROUTINE_DEC
	{
	  $$ = new ListSubroutineDecNode($2, $1);
	}
	;

SUBROUTINE_DEC:
	PROCEDURE_TYPE VOID_TOKEN SUBROUTINE_NAME LEFT_PAREN PARAMETER_LIST RIGHT_PAREN SUBROUTINE_BODY
        {
	  $$ = new SubroutineDecNode($1, $2, $3, $4, $5, $6, $7);
	}
	| PROCEDURE_TYPE TYPE SUBROUTINE_NAME LEFT_PAREN PARAMETER_LIST RIGHT_PAREN SUBROUTINE_BODY
        {
	  $$ = new SubroutineDecNode($1, $2, $3, $4, $5, $6, $7);
	}
	;

PROCEDURE_TYPE:
	CONSTRUCTOR
	{ 
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
 	}
	| METHOD
	{ 
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
 	}
	| FUNCTION
	{ 
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
 	}
	;

PARAMETER_LIST:
	%empty
        {
	  $$ = NULL;
	}
	| PARAMS_LIST_AUX
	{
	  $$ = $1;
	}
	;

PARAMS_LIST_AUX:
	TYPE VAR_NAME
	{
	  $$ = new ParameterListNode($1, $2, NULLCHAR, NULL);
	}
	| PARAMS_LIST_AUX COMMA_SYM TYPE VAR_NAME
	{
	  $$ = new ParameterListNode($3, $4, $2, $1);
	}
	;

SUBROUTINE_BODY:
	LEFT_BRACKET LIST_VAR_DEC LIST_OF_STATEMENTS RIGHT_BRACKET
	{
	  $$ = new SubroutineBodyNode($1, $2, $3, $4);
	}
	;

LIST_VAR_DEC:
	%empty
        {
	  $$ = NULL;
	}
	| LIST_VAR_DEC VAR_DEC
	{
	  $$ = new ListVarDecNode($2, $1);
	}
	;
VAR_DEC:
	VAR_TOKEN TYPE LIST_VAR_NAME SEMICOLON_SYM
	{
	  $$ = new VarDecNode($1, $2, $3, $4);
	}
	;

VAR_NAME:
	IDENTIFIER
	{	    
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	;

CLASS_NAME:
	IDENTIFIER
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	;

SUBROUTINE_NAME:
	IDENTIFIER
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
 	}
	;

LIST_OF_STATEMENTS:
        STATEMENTS
	{
	  if($1 == NULL)
	    $$ = NULL;
	  else
	    $$ = new ListOfStatementsNode($1);
	}
	;

STATEMENTS:
	%empty
        {
	  $$ = NULL;
	}
	| STATEMENTS STATEMENT
	{
	  $$ = new StatementsNode($2, $1);
	}
	;

STATEMENT:
	LET_STATEMENT
	{
	  $$ = new SingleStatementNode($1, NULL, NULL, NULL, NULL);
	}
	| IF_STATEMENT
	{
	  $$ = $1;
	}
	| WHILE_STATEMENT
	{
	  $$ = $1;
	}
	| DO_STATEMENT
	{
	  $$ = $1;
	}
	| RETURN_STATEMENT
	{
	  $$ = $1;
	}
	;

LET_STATEMENT:
	LET_TOKEN VAR_NAME EQUAL_SYM EXPRESSION SEMICOLON_SYM
	{
	  $$ = new LetStatementNode($1, $2, NULLCHAR, NULL, NULLCHAR, $3, $4, $5);
	}
	| LET_TOKEN VAR_NAME LEFT_BRACKET2 EXPRESSION RIGHT_BRACKET2 EQUAL_SYM EXPRESSION SEMICOLON_SYM
	{
	  $$ = new LetStatementNode($1, $2, $3, $4, $5, $6, $7, $8);
	}
	;

IF_STATEMENT:
	IF_TOKEN LEFT_PAREN EXPRESSION RIGHT_PAREN LEFT_BRACKET LIST_OF_STATEMENTS RIGHT_BRACKET
	{
	  $$ = new IfStatementNode($1, $2, $3, $4, $5, $6, $7, NULLSTRING, NULLCHAR, NULL, NULLCHAR);
	}
	| IF_TOKEN LEFT_PAREN EXPRESSION RIGHT_PAREN LEFT_BRACKET LIST_OF_STATEMENTS RIGHT_BRACKET ELSE_TOKEN LEFT_BRACKET LIST_OF_STATEMENTS RIGHT_BRACKET
	{
	  $$ = new IfStatementNode($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
	}
	;

WHILE_STATEMENT:
	WHILE_TOKEN LEFT_PAREN EXPRESSION RIGHT_PAREN LEFT_BRACKET LIST_OF_STATEMENTS RIGHT_BRACKET
	{
	  $$ = new WhileStatementNode($1, $2, $3, $4, $5, $6, $7);
	}
	;

DO_STATEMENT:
	DO_TOKEN SUBROUTINE_CALL SEMICOLON_SYM
	{
	  $$ = new DoStatementNode($1, $2, $3);
	}
	;

RETURN_STATEMENT:
	RETURN_TOKEN SEMICOLON_SYM
	{
	  $$ = new ReturnStatementNode($1, NULL, $2);
	}
	| RETURN_TOKEN EXPRESSION SEMICOLON_SYM
	{
	  $$ = new ReturnStatementNode($1, $2, $3);
	}
	;

EXPRESSION: 
        OP_LIST
	{
	  $$ = new ExpressionNode($1);
	}
	;

OP_LIST:
        TERM
        {
	  $$ = new OperatorListNode(NULLCHAR, $1, NULL);
	}
        | OP_LIST OPERATOR TERM
        {
	  $$ = new OperatorListNode($2, $3, $1);
	}
	;

TERM:
	INT_CONST
	{
	  string aux(yytext);
	  $1 = std::stoi(aux);
	  if($1 > 32767){
	    errorFound(INT_OVERFLOW);
	  }else
	    $$ = new TermNode($1, NULLSTRING, NULLSTRING, NULLSTRING, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULLCHAR, NULL);
	}
	| STRING_CONST
	{
	  string aux(yytext);
	  aux.erase(aux.begin());
	  aux.erase(aux.size() - 1);
	  $$ = new TermNode(NULLINT, aux, NULLSTRING, NULLSTRING, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULLCHAR, NULL);
	}
	| KEYWORD_CONST
	{
	  $$ = new TermNode(NULLINT, NULLSTRING, $1, NULLSTRING, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULLCHAR, NULL);
	}
        | VAR_NAME
	{
	  $$ = new TermNode(NULLINT, NULLSTRING, NULLSTRING, $1, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULLCHAR, NULL);
	}
	| VAR_NAME LEFT_BRACKET2 EXPRESSION RIGHT_BRACKET2
	{
	  $$ = new TermNode(NULLINT, NULLSTRING, NULLSTRING, $1, $2, $3, $4, NULL, NULLCHAR, NULL, NULLCHAR, NULLCHAR, NULL);
	}
	| SUBROUTINE_CALL
	{
	  $$ = new TermNode(NULLINT, NULLSTRING, NULLSTRING, NULLSTRING, NULLCHAR, NULL, NULLCHAR, $1, NULLCHAR, NULL, NULLCHAR, NULLCHAR, NULL);
	}
	| LEFT_PAREN EXPRESSION RIGHT_PAREN
	{
	  $$ = new TermNode(NULLINT, NULLSTRING, NULLSTRING, NULLSTRING, NULLCHAR, NULL, NULLCHAR, NULL, $1, $2, $3, NULLCHAR, NULL);
	}
	| UNARY_OP TERM
	{
	  $$ = new TermNode(NULLINT, NULLSTRING, NULLSTRING, NULLSTRING, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, NULL, NULLCHAR, $1, $2);
	}
	;

SUBROUTINE_CALL:
        VAR_NAME LEFT_PAREN EXPRESSION_LIST RIGHT_PAREN
	{//if you put SUBROUTINE_NAME instead of VAR_NAME, you will get the error of the skipped identifier D:
	  $$ = new SubroutineCallNode(NULLSTRING, NULLCHAR, $1, $2, $3, $4);
	}
	| VAR_NAME POINT_SYM SUBROUTINE_NAME LEFT_PAREN EXPRESSION_LIST RIGHT_PAREN
	{
	  $$ = new SubroutineCallNode($1, $2, $3, $4, $5, $6);
	}
	;

EXPRESSION_LIST:
	%empty
        {
	  $$ = NULL;
	}
	| EXPRESSION_LIST_AUX
	{
	  $$ = new ExpressionListNodeAux($1);
	}
	;

EXPRESSION_LIST_AUX:
	EXPRESSION
	{
	  $$ = new ExpressionListNode(NULLCHAR, $1, NULL);
	}
	| EXPRESSION_LIST_AUX COMMA_SYM EXPRESSION
	{
	  $$ = new ExpressionListNode($2, $3, $1);
	}
	;

OPERATOR:
	PLUS
	{$$ = *yytext;}
        | MINUS
	{$$ = *yytext;}
	| TIMES
	{$$ = *yytext;}
	| DIVIDE
	{$$ = *yytext;}
	| AND
	{$$ = *yytext;}
	| OR
	{$$ = *yytext;}
	| LT
	{$$ = *yytext;}
	| GT
	{$$ = *yytext;}
	| EQUAL_SYM
	{$$ = *yytext;}
	;

UNARY_OP:
	MINUS
	{$$ = *yytext;}
	| NEG
	{$$ = *yytext;}
	;

KEYWORD_CONST:
	TRUE
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	| FALSE
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	| nULL
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	| THIS
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
	;

CLASS_TOKEN:
        CLASS
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;
FIELD_TOKEN:
        FIELD
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

STATIC_TOKEN:
        STATIC
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

VAR_TOKEN:
        VAR
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

VOID_TOKEN:
        VOID
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

LET_TOKEN:
        LET
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

DO_TOKEN:
        DO
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

IF_TOKEN:
        IF
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

ELSE_TOKEN:
        ELSE
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

WHILE_TOKEN:
        WHILE
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

RETURN_TOKEN:
        RETURN
	{
	  $1 = (char*)malloc(strlen(yytext));
	  $$ = strcpy($1, yytext);
	}
        ;

LEFT_BRACKET:
        LB
	{$$ = *yytext;}
        ;

RIGHT_BRACKET:
        RB
	{$$ = *yytext;}
        ;

LEFT_PAREN:
        LP
	{$$ = *yytext;}
        ;

RIGHT_PAREN:
        RP
	{$$ = *yytext;}
        ;

LEFT_BRACKET2:
        LB2
	{
	  $$ = *yytext;
	}
        ;

RIGHT_BRACKET2:
        RB2
	{$$ = *yytext;}
        ;

POINT_SYM:
        POINT
	{$$ = *yytext;}
        ;

EQUAL_SYM:
        EQUAL
	{$$ = *yytext;}
        ;

COMMA_SYM:
        COMMA
	{$$ = *yytext;}
	;

SEMICOLON_SYM:
        SEMICOLON
	{$$ = *yytext;}
        ;

%%


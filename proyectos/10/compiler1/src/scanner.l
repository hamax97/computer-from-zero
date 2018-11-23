%{

#define YY_DECL extern "C" int yylex()

#include "parser.tab.c"

#include <iostream>

using std::cout;
using std::endl;

void printElement(string e, string type){
  scannerFileOut << "<" << type << "> " << e << " </" << type << ">" << endl;
}

%}

%option noyywrap
%option yylineno

INT_CONST     [0-9]+
STRING_CONST  \"[^\n"]+\"
IDENTIFIER    [a-zA-Z][a-zA-Z_]*
WHITE_SPACE   [ \t]+
EOL           \n|\r|\r\n

%x IN_COMMENT

%%
"class"          {
                  scannerInitialAndEndTag();
		  printElement(yytext, "keyword");
                  return CLASS;
                 }
"static"         {
		  printElement(yytext, "keyword");
                  return STATIC;
		 }
"field"          {
		  printElement(yytext, "keyword");
		  return FIELD;	
		 }
"method"         {
		  printElement(yytext, "keyword");
                  return METHOD;
		 }
"function"       {
		  printElement(yytext, "keyword");
                  return FUNCTION;
		 }
"constructor"    {
		  printElement(yytext, "keyword");
                  return CONSTRUCTOR;
		 }
"int"            {
		  printElement(yytext, "keyword");
                  return INT;
		 }
"boolean"        {
		  printElement(yytext, "keyword");
                  return BOOLEAN;
		 }
"char"           {
		  printElement(yytext, "keyword");
                  return CHAR;
		 }
"void"           {
		  printElement(yytext, "keyword");
                  return VOID;
		 }
"var"            {
		  printElement(yytext, "keyword");
                  return VAR;
		 }
"let"            {
		  printElement(yytext, "keyword");
                  return LET;
		 }
"do"             {
		  printElement(yytext, "keyword");
                  return DO;
		 }
"if"             {
		  printElement(yytext, "keyword");
                  return IF;
		 }
"else"           {
		  printElement(yytext, "keyword");
                  return ELSE;
		 }
"while"          {
		  printElement(yytext, "keyword");
                  return WHILE;
		 }
"return"         {
		  printElement(yytext, "keyword");
                  return RETURN;
		 }
"true"           {
		  printElement(yytext, "keyword");
                  return TRUE;
		 }
"false"          {
		  printElement(yytext, "keyword");
                  return FALSE;
		 }
"null"           {
		  printElement(yytext, "keyword");
                  return nULL;
		 }
"this"           {
		  printElement(yytext, "keyword");
                  return THIS;
		 }
"{"              {
		  printElement(yytext, "symbol");
                  return LB;
		 }
"}"              {
		  printElement(yytext, "symbol");
                  return RB;
		 }
"("              {
		  printElement(yytext, "symbol");
                  return LP;
		 }
")"              {
		  printElement(yytext, "symbol");
                  return RP;
		 }
"["              {
		  printElement(yytext, "symbol");
                  return LB2;
		 }
"]"              {
		  printElement(yytext, "symbol");
                  return RB2;
		 }
"."              {
		  printElement(yytext, "symbol");
                  return POINT;
		 }
","              {
		  printElement(yytext, "symbol");
                  return COMMA;
		 }
";"              {
		  printElement(yytext, "symbol");
                  return SEMICOLON;
		 }
"+"              {
		  printElement(yytext, "symbol");
                  return PLUS;
		 }
"-"              {
		  printElement(yytext, "symbol");
                  return MINUS;
		 }
"*"              {
		  printElement(yytext, "symbol");
                  return TIMES;
		 }
"/"              {
		  printElement(yytext, "symbol");
                  return DIVIDE;
		 }
"&"              {
		  printElement("&amp;", "symbol");
                  return AND;
		 }
"|"              {
		  printElement(yytext, "symbol");
                  return OR;
		 }
"<"              {
		  printElement("&lt;", "symbol");
                  return LT;
		 }
">"              {
		  printElement("&gt;", "symbol");
                  return GT;
		 }
"="              {
		  printElement(yytext, "symbol");
                  return EQUAL;
		 }
"~"              {
		  printElement(yytext, "symbol");
                  return NEG;
		 }

{IDENTIFIER}     {
		  printElement(yytext, "identifier");
                  return IDENTIFIER;
		 }
{STRING_CONST}   {
		  string aux(yytext);
	          aux.erase(aux.begin());	
	          aux.erase(aux.size() - 1);
		  printElement(aux, "stringConstant");
                  return STRING_CONST;
		 }
{INT_CONST}      {
		  printElement(yytext, "integerConstant");
                  return INT_CONST;
		 }

<INITIAL>{
"/*"              BEGIN(IN_COMMENT);
}
<IN_COMMENT>{
"*/"      BEGIN(INITIAL);
[^*\n]+   // eat comment in chunks
"*"       // eat the lone star
\n        yylineno++;
}

"//".*           {/* ignore single line comments */}


{WHITE_SPACE}    {/* ignore white spaces */}
{EOL}            {}
.                {cout << "Unrecognized token: " << yytext << " in line: " << yylineno << endl;}
%%


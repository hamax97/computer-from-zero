%{

#define YY_DECL extern "C" int yylex()

#include "parser.tab.c"

#include <iostream>

using std::cout;
using std::endl;

%}

%option yylineno
%option noyywrap

INT_CONST     [0-9]+
STRING_CONST  \"[^\n"]+\"
IDENTIFIER    [[:alpha:]_][[:alnum:]_]*
WHITE_SPACE   [ \t]+
EOL           \n|\r|\r\n

%x IN_COMMENT

%%
"class"          {
                  return CLASS;
                 }
"static"         {
                  return STATIC;
		 }
"field"          {
		  return FIELD;	
		 }
"method"         {
                  return METHOD;
		 }
"function"       {
                  return FUNCTION;
		 }
"constructor"    {
                  return CONSTRUCTOR;
		 }
"int"            {
                  return INT;
		 }
"boolean"        {
                  return BOOLEAN;
		 }
"char"           {
                  return CHAR;
		 }
"void"           {
                  return VOID;
		 }
"var"            {
                  return VAR;
		 }
"let"            {
                  return LET;
		 }
"do"             {
                  return DO;
		 }
"if"             {
                  return IF;
		 }
"else"           {
                  return ELSE;
		 }
"while"          {
                  return WHILE;
		 }
"return"         {
                  return RETURN;
		 }
"true"           {
                  return TRUE;
		 }
"false"          {
                  return FALSE;
		 }
"null"           {
                  return nULL;
		 }
"this"           {
                  return THIS;
		 }
"{"              {
                  return LB;
		 }
"}"              {
                  return RB;
		 }
"("              {
                  return LP;
		 }
")"              {
                  return RP;
		 }
"["              {
                  return LB2;
		 }
"]"              {
                  return RB2;
		 }
"."              {
                  return POINT;
		 }
","              {
                  return COMMA;
		 }
";"              {
                  return SEMICOLON;
		 }
"+"              {
                  return PLUS;
		 }
"-"              {
                  return MINUS;
		 }
"*"              {
                  return TIMES;
		 }
"/"              {
                  return DIVIDE;
		 }
"&"              {
                  return AND;
		 }
"|"              {
                  return OR;
		 }
"<"              {
                  return LT;
		 }
">"              {
                  return GT;
		 }
"="              {
                  return EQUAL;
		 }
"~"              {
                  return NEG;
		 }

{IDENTIFIER}     {
                  return IDENTIFIER;
		 }
{STRING_CONST}   {
                  return STRING_CONST;
		 }
{INT_CONST}      {
                  return INT_CONST;
		 }

<INITIAL>{
"/*"              BEGIN(IN_COMMENT);
}
<IN_COMMENT>{
"*/"      BEGIN(INITIAL);
[^*\n]+   // eat comment in chunks
"*"       // eat the lone star
\n        //
}

"//".*           {/* ignore single line comments */}


{WHITE_SPACE}    {/* ignore white spaces */}
{EOL}            {}
.                {cout << "Unrecognized token: " << yytext << " in line: " << yylineno << endl;}
%%


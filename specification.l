%{
#include <iostream>
#include <string>
#include <utility>
#include "grammar.hpp"

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

%}

%option yylineno

blank " "|\t|\n
SEMICOLON (";")

PROGRAM ("PROGRAM")
BEGIN_PROG ("BEGIN")
END ("END")




FUNCTION ("FUNCTION")

READ ("READ")

WRITE ("WRITE")

IF ("IF")

ELSE ("ELSE")

ENDIF ("ENDIF")

WHILE ("WHILE")

ENDWHILE ("ENDWHILE")

BREAK ("BREAK")

CONTINUE ("CONTINUE")

RETURN ("RETURN")

INT ("INT")

VOID ("VOID")


FLOAT ("FLOAT")

ASSIGN [:][=]
COMMA [,]
OPENPARAN [\(]
CLOSEPARAN [\)]
PLUS [+]
MINUS [-]
MUL [*]
DIV [/]

LESSTHAN [<]
GREATERTHAN [>]
EQUALTO [=]
NOT [!]

IDENTIFIER ([a-zA-Z])([a-zA-Z0-9]*)

STRINGLITERAL (\")([^\"]*)(\")

FLOATLITERAL ([0-9]*)"."([0-9]+)

INTLITERAL ([0-9])([0-9]*)

COMMENT ("//")(.*)\n


%%



{blank} {}
{COMMENT} {}


{STRINGLITERAL} {
        yylval.str = strdup(yytext);
        return STRINGLITERAL;
}

{END} {return END;}
{PROGRAM} {return PROGRAM;} 
{BEGIN_PROG} {return BEGIN_PROG;}

STRING {return STRING;}
{INT} {
        yylval.str = strdup(yytext);
        return INT;
}
{FLOAT} {
        yylval.str = strdup(yytext);  
        return FLOAT;
}
{VOID} {return VOID;}
{FUNCTION} {return FUNCTION;}
{READ} {return READ;}
{WRITE} {return WRITE;}
{RETURN} {return RETURN;}
{IF} {return IF;}
{ENDIF} {return ENDIF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}
{ENDWHILE} {return ENDWHILE;}
{BREAK} {return BREAK;}
{CONTINUE} {return CONTINUE;}

{ASSIGN} {return ASSIGN;}
{COMMA} {return COMMA;}
{OPENPARAN} {return OPENPARAN;}
{CLOSEPARAN} {return CLOSEPARAN;}
{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{MUL} {return MUL;}
{DIV} {return DIV;}

{LESSTHAN} {return LESSTHAN;}
{GREATERTHAN} {return GREATERTHAN;}
{EQUALTO} {return EQUALTO;}
{NOT} {return NOT;}

{SEMICOLON} {return SEMICOLON;}
{INTLITERAL} {
        yylval.str = strdup(yytext);
        return INTLITERAL;}
{FLOATLITERAL} {
        yylval.str = strdup(yytext);
        return FLOATLITERAL;}

{IDENTIFIER} {
        yylval.identifier = new std::pair<std::string, std::string>;
        (yylval.identifier)->first = std::string(yytext);
        (yylval.identifier)->second = std::string("");
        return IDENTIFIER;
}
. {return 0;}
%%

int yywrap(){
  return 1;
}
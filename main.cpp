#include <iostream>
#include "grammar.hpp"
#include <utility>

extern FILE* yyin;
int yylex();
int yyparse();

int main(int argc, char* argv[])
{
  
  if (argc>1){
  yyin = fopen(argv[1], "r");
  }

    


  if (yyparse() == 0)
  {
    // printf("Accepted\n");
  }
  

  return 0;
}
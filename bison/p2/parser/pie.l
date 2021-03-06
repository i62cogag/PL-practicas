/*!
  \file interpreter.l
  \brief Lexical or scanner file
*/
%{
#include <iostream>

/*********************/
/* NEW in example 4 */
/* atof */
#include <stdlib.h>
/*********************/

/* Macros for the screen */
#include "../includes/macros.hpp"

/*********************/
/*
  NEW in example 16
  AST class
  IMPORTANT: must be written before interpreter.tab.h
*/
#include "../ast/ast.hpp"
/*********************/

/*  interpreter.tab.h contains the numeric values of the tokens produced by the parser */
#include "pie.tab.h"

/********************/
/* NEW in example 3 */
#include "../error/error.hpp"
/********************/


/*********************************************/
/* NEW in example 7 */
/* Table of symbols */
#include "../table/table.hpp"

#include "../table/numericVariable.hpp"

extern lp::Table table; //!< External Table of Symbols
/*********************************************/

extern int lineNumber; //!< External line counter

%}

/*! \name REGULAR DEFINITIONS  */

DIGIT  [0-9]

LETTER [a-zA-Z]

integerNUMBER 					[0-9]+
realNUMBER						[0-9]+\.[0-9]+
scientificNotationNUMBER	 	[0-9]+e[+-]?[0-9]+
cadenas '([^']|"\\\'")*'

IDENTIFIER  {LETTER}({LETTER}|{DIGIT}*(_)?{LETTER}|{DIGIT})*

%x LINE_COMMENT
%x COMMENT

/* NEW in example 3 */
/* STATE  */
%x ERROR

/*! \name REGULAR EXPRESSIONS  */
%%

[ \t] 	{ ; }    /* skip white space and tabular */

\n      {
			/* Line counter */
           	lineNumber++;

			/* MODIFIED in example 3 */
			/* COMMENTED in example 5 */
        	/* return NEWLINE; */
        }

";"		{ return SEMICOLON; }
","		{ return COMMA;}
":=" 	{ return ASSIGNMENT; }		/* NEW in example 7 */
"+"		{ return PLUS;   } 			/* NEW in example 3 */
"-"		{ return MINUS;  } 			/* NEW in example 3 */
"*"	    { return MULTIPLICATION; } 	/* NEW in example 3 */
"/"     { return DIVISION; } 		/* NEW in example 3 */
"%"		{ return MODULO;}
"&"		{ return QUOTIENT;}									/*¿Símbolo de quotient?*/
"**"	{ return POWER; }
"||"    { return CONCAT; }
"("     { return LPAREN; } 			/* NEW in example 3 */
")"	    { return RPAREN; } 			/* NEW in example 3 */
"="	    { return EQUAL; }			/* NEW in example 15 */
"<>"	{ return NOT_EQUAL; }		/* NEW in example 15 */
">="	{ return GREATER_OR_EQUAL; }/* NEW in example 15 */
"<="	{ return LESS_OR_EQUAL; }	/* NEW in example 15 */
">"		{ return GREATER_THAN; }	/* NEW in example 15 */
"<"		{ return LESS_THAN; }		/* NEW in example 15 */
"{"		{ return LETFCURLYBRACKET; } 	/* NEW in example 17 */
"}"		{ return RIGHTCURLYBRACKET; }	/* NEW in example 17 */


{cadenas}	{
				std::string str(yytext);
				std::string def;
				str = str.substr(1, str.size()-2);

				for(unsigned i = 0; i < str.size(); i++){
					if(str[i] == 92 && str[i+1] == 'n'){
						def.push_back(10);
						i++;
					}else if(str[i] == 92 && str[i+1] == 't'){
						def.push_back(9);
						i++;
					}else if(str[i] == 92 && str[i+1] == 39){
						def.push_back(39);
						i++;
					}else{
						def.push_back(str[i]);
					}
				}

        		yylval.cadena =  strdup(def.c_str());
                return CADENA;
	        }

{integerNUMBER}|{realNUMBER}|{scientificNotationNUMBER} 	{
					  /* MODIFIED in example 4 */
					  /* Conversion of type and sending of the numerical value to the parser */
					   yylval.number = atof(yytext);

                       return NUMBER;
                      }


{IDENTIFIER}   		 {

						// No se distinguen mayusculas y minusculas. Siempre se guarda en minusculas
						for (int i = 0; yytext[i] != '\0'; i++)
						{
							yytext[i] = tolower(yytext[i]);
						}

						/* NEW in example 7 */
						std::string identifier(yytext);


						/*
                          strdup() function returns a pointer to a new string
                          which  is a duplicate of the string yytext
                        */
						  yylval.identifier =  strdup(yytext);


						/* If the identifier is not in the table of symbols then it is inserted */
						 if (table.lookupSymbol(identifier) == false)
							{
								/*
								 The identifier is inserted into the symbol table
								 as undefined Variable with value 0.0
								*/

								lp::NumericVariable *n = new lp::NumericVariable(identifier,VARIABLE,UNDEFINED,0.0);

								/* A pointer to the new NumericVariable is inserted into the table of symbols */
								table.installSymbol(n);
								return VARIABLE;
							}

						/* MODIFIED in example 11 */
						/*
							If the identifier is in the table of symbols then its token is returned
								The identifier can be a variable or a numeric constant
						*/
						else
							{
								lp::Symbol	*s = table.getSymbol(identifier);

/*
								std::cout << "lex: "<< s->getName()
										  << "token " << s->getToken()
  										  << std::endl;
*/

								/*  If the identifier is in the table then its token is returned  */
								return s->getToken();
							}
                     }


"@" {
        BEGIN LINE_COMMENT;
    }
<LINE_COMMENT>.* {
                    BEGIN 0;
                 }

"#" {
    yymore();
    BEGIN COMMENT;
    }
<COMMENT>"#" {
            yymore();
            BEGIN(INITIAL);
        }
<COMMENT>.	{yymore();}
<COMMENT>\n	{yymore();}

"?" {printf("\n ERROR: %s simbolo no permitido\n",yytext);}
"¿" {printf("\n ERROR: %s simbolo no permitido\n",yytext);}
"!=" {printf("\n ERROR: %s simbolo no permitido \n Quizás quisite poner <>\n",yytext);}
"&&" {printf("\n ERROR: %s simbolo no permitido \n Quizás quisite poner .y.\n",yytext);}
"==" {printf("\n ERROR: %s simbolo no permitido \n Quizás quisite poner = en la condicion\n",yytext);}
":" {printf("\n ERROR: %s simbolo no permitido \n Quizás quisite poner ;\n",yytext);}

<<EOF>> { /* The interpreter finishes when finds the end of file character */
		/*  PLACE(24,10);
	  	  std::cout <<  BICYAN;
	   	  std::cout <<  ">>>>>>> End of file <<<<<<<";
	 	  std::cout << std::endl;
	   	  std::cout << RESET;
		  PLACE(25,1);
		*/
	  	  return 0;
	}


.								{
									/* Any other character */
									/* MODIFIIED in example 3 */
									/* Change to error state */
									BEGIN(ERROR);

									/*
										The current value of yytext
										will be concatenated with the next character
									*/
									yymore();
								}

<ERROR>[^0-9+\-*/()\^% \t\n\;a-zA-Z=<>!&]	{ /* MODIFIED in examples 5, 7, 15 */
								  /* NEW in example 3 */
								  /*
									The current value of yytext
									will be concatenated with the next character
								  */
									yymore();
								}

<ERROR>(.|\n)                {
								/* NEW in example 3 */
								/* Rewind one position */
								yyless(yyleng-1);

								/* Display the error message */
								warning("Lexical error", yytext);

								/* Restore the initial state */
   								BEGIN(INITIAL);
							}
%%

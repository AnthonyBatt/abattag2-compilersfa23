/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_DECL_H_INCLUDED
# define YY_YY_DECL_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_ARRAY = 258,
    TOKEN_VOID = 259,
    TOKEN_INTEGER = 260,
    TOKEN_FLOAT = 261,
    TOKEN_CHAR = 262,
    TOKEN_STRING = 263,
    TOKEN_BOOLEAN = 264,
    TOKEN_TRUE_LITERAL = 265,
    TOKEN_FALSE_LITERAL = 266,
    TOKEN_IF = 267,
    TOKEN_ELSE = 268,
    TOKEN_FOR = 269,
    TOKEN_WHILE = 270,
    TOKEN_PRINT = 271,
    TOKEN_FUNCTION = 272,
    TOKEN_RETURN = 273,
    TOKEN_AUTO = 274,
    TOKEN_COMMA = 275,
    TOKEN_SEMICOLON = 276,
    TOKEN_COLON = 277,
    TOKEN_PAREN_OPEN = 278,
    TOKEN_PAREN_CLOSE = 279,
    TOKEN_BRACK_OPEN = 280,
    TOKEN_BRACK_CLOSE = 281,
    TOKEN_BRACE_OPEN = 282,
    TOKEN_BRACE_CLOSE = 283,
    TOKEN_EXPO = 284,
    TOKEN_MULTI = 285,
    TOKEN_DIVIDE = 286,
    TOKEN_MODULUS = 287,
    TOKEN_LE = 288,
    TOKEN_LT = 289,
    TOKEN_GE = 290,
    TOKEN_GT = 291,
    TOKEN_EQ = 292,
    TOKEN_NE = 293,
    TOKEN_NOT = 294,
    TOKEN_ASSIGNMENT = 295,
    TOKEN_LOG_OR = 296,
    TOKEN_LOG_AND = 297,
    TOKEN_POST_INC = 298,
    TOKEN_POST_DEC = 299,
    TOKEN_PLUS = 300,
    TOKEN_MINUS = 301,
    TOKEN_ID = 302,
    TOKEN_FLOAT_LITERAL = 303,
    TOKEN_INTEGER_LITERAL = 304,
    TOKEN_CHAR_LITERAL = 305,
    TOKEN_STRING_LITERAL = 306
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DECL_H_INCLUDED  */

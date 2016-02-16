/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    tINT = 258,
    tAnd = 259,
    tOr = 260,
    tEquals = 261,
    tNotEquals = 262,
    tNot = 263,
    tPrint = 264,
    tIf = 265,
    tWhile = 266,
    tReturn = 267,
    tSemi = 268,
    tComa = 269,
    tAffect = 270,
    tPlus = 271,
    tMinus = 272,
    tMult = 273,
    tDiv = 274,
    tPO = 275,
    tPC = 276,
    tAO = 277,
    tAC = 278,
    tNumber = 279,
    tID = 280
  };
#endif
/* Tokens.  */
#define tINT 258
#define tAnd 259
#define tOr 260
#define tEquals 261
#define tNotEquals 262
#define tNot 263
#define tPrint 264
#define tIf 265
#define tWhile 266
#define tReturn 267
#define tSemi 268
#define tComa 269
#define tAffect 270
#define tPlus 271
#define tMinus 272
#define tMult 273
#define tDiv 274
#define tPO 275
#define tPC 276
#define tAO 277
#define tAC 278
#define tNumber 279
#define tID 280

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 25 "src/source.yacc" /* yacc.c:1909  */
	
	int type;
        int number;
        char *string;

#line 110 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

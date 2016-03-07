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
    tCHAR = 259,
    tAnd = 260,
    tOr = 261,
    tEquals = 262,
    tNotEquals = 263,
    tNot = 264,
    tPrint = 265,
    tIf = 266,
    tWhile = 267,
    tReturn = 268,
    tSemi = 269,
    tComa = 270,
    tAffect = 271,
    tPlus = 272,
    tMinus = 273,
    tMult = 274,
    tDiv = 275,
    tAmpersand = 276,
    tPO = 277,
    tPC = 278,
    tAO = 279,
    tAC = 280,
    tCO = 281,
    tCC = 282,
    tNumber = 283,
    tID = 284
  };
#endif
/* Tokens.  */
#define tINT 258
#define tCHAR 259
#define tAnd 260
#define tOr 261
#define tEquals 262
#define tNotEquals 263
#define tNot 264
#define tPrint 265
#define tIf 266
#define tWhile 267
#define tReturn 268
#define tSemi 269
#define tComa 270
#define tAffect 271
#define tPlus 272
#define tMinus 273
#define tMult 274
#define tDiv 275
#define tAmpersand 276
#define tPO 277
#define tPC 278
#define tAO 279
#define tAC 280
#define tCO 281
#define tCC 282
#define tNumber 283
#define tID 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 34 "src/source.yacc" /* yacc.c:1909  */
	
        int number;
        char *string;
	type_t* type;
	expression_t expression;

#line 119 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

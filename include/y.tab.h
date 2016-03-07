/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tINT = 258,
     tCHAR = 259,
     tAnd = 260,
     tOr = 261,
     tEquals = 262,
     tNotEquals = 263,
     tNot = 264,
     tPrint = 265,
     tIf = 266,
     tElse = 267,
     tWhile = 268,
     tReturn = 269,
     tSemi = 270,
     tComa = 271,
     tAffect = 272,
     tPlus = 273,
     tMinus = 274,
     tMult = 275,
     tDiv = 276,
     tAmpersand = 277,
     tPO = 278,
     tPC = 279,
     tAO = 280,
     tAC = 281,
     tNumber = 282,
     tID = 283
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
#define tElse 267
#define tWhile 268
#define tReturn 269
#define tSemi 270
#define tComa 271
#define tAffect 272
#define tPlus 273
#define tMinus 274
#define tMult 275
#define tDiv 276
#define tAmpersand 277
#define tPO 278
#define tPC 279
#define tAO 280
#define tAC 281
#define tNumber 282
#define tID 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 34 "src/source.yacc"
{
        int number;
        char *string;
	type_t* type;
	expression_t expression;
}
/* Line 1529 of yacc.c.  */
#line 112 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;


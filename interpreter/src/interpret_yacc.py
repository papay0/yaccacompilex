from ply import *
from interpreter import *

from interpret_lex import tokens

def p_expression_ADD(p):
    '''expression : ADD NUMBER NUMBER NUMBER
                    | SUB NUMBER NUMBER NUMBER
                    | MUL NUMBER NUMBER NUMBER
                    | DIV NUMBER NUMBER NUMBER
                    | COP NUMBER NUMBER
                    | JMP NUMBER
                    | JMF NUMBER NUMBER
                    | EQ NUMBER NUMBER NUMBER
                    | AFC NUMBER NUMBER
                    '''
    if len(p) == 5:
        interpreter.add(p[1], p[2], p[3], p[4])
    elif len(p) == 4:
        interpreter.add(p[1], p[2], p[3])

def p_expression_NUMBER(p):
    'expression : NUMBER expression expression'

# Error rule for syntax errors
def p_error(p):
    print("Syntax error in input!", p)

# Build the parser
yacc.yacc()


interpreter = Interpreter()

while 1:
   try:
       s = input()
   except EOFError:
       interpreter.printDB()
       break
   if not s: continue
   result = yacc.parse(s)
   #print(result)
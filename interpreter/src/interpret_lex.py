from ply import lex

tokens = [
    'NUMBER',
    'AFC',
    'ADD',
    'SUB',
    'MUL',
    'DIV',
     'COP',
     'EQ',
     'JMF',
     'JMP',
]

# Regular expression rules for simple tokens
t_AFC     = 'AFC'
t_ADD     = 'ADD'
t_DIV     = 'DIV'
t_SUB     = 'SUB'
t_MUL     = 'MUL'
t_COP     = 'COP'
t_EQ      = 'EQ'
t_JMF     = 'JMF'
t_JMP     = 'JMP'

def t_COLON(t):
    '\d+:'

def t_NUMBER(t):
    '\d+'
    try:
        t.value = int(t.value, 0)
    except ValueError:
        print("Line %d: Number %s is too large!" % (t.lineno,t.value))
        t.value = 0
    return t


# Define a rule so we can track line numbers
def t_newline(t):
    r'\n+'
    t.lineno += len(t.value)

# A string containing ignored characters (spaces and tabs)
t_ignore  = ' \t'

# Error handling rule
def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.skip(1)

# Build the lexer
lex.lex()

# Test it out
data = '''
'''

# Give the lexer some input
lex.input(data)

# Tokenize
while 1:
    tok = lex.token()
    if not tok: break      # No more input
    print(tok)
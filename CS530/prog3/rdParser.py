import re
from functools import *

"""

Assignment 3 - 
A LL(1) recursive descent parser for validating simple expressions.

# Part A:
You would need to first write the grammar rules (non-terminals) in EBNF 
according to the token patterns and grammar rules specified in the prompt,
put the rules in a separate PDF file, see prompt. 
(Refer to the EBNF example in Figure 5.15)

# Part B:
You would then write the recursive descent parsing procedures for 
validating expressions according to the rules from Part A. 
(Refer to the recursive descent parsing algorithm in Figure 5.17)

It implements one parsing procedure for each one of the 
non-terminals (grammar rules), starting from the top of the parse tree, 
then drilling into lower hierachical levels.

The procedures work together to handle all combinations of the grammar 
rules, and they automatically handle the nested compositions of terms 
with multi-level priority brackets. 

----------------------------------------------------------------------------
Usage (Refer to the prompt for more examples - both positive and negative cases)

r = recDecsent('7 - 17')
print(r.validate()) # will print True as '7 - 17' is a valid expression

r = recDecsent('7 - ')
print(r.validate()) # will print False as '7 - ' is an invalid expression

"""

class recDescent:

    # IMPORTANT:
    # You MUST NOT change the signatures of 
    # the constructor, lex(self) and validate(self)
    # Otherwise, autograding tests will FAIL

    # constructor to initialize and set class level variables
    def __init__(self, expr = ""):

        # string to be parsed
        self.expr = expr

        # tokens from lexer tokenization of the expression
        self.tokens = []
        self.op = ['and','or','nand','xor','xnor']
        self.relop = ['>','<','>=','<=','==','!=','not']

    # lexer - tokenize the expression into a list of tokens
    # the tokens are stored in an list which can be accessed by self.tokens
    # do NOT edit any piece of code in this function

    def lex(self):
        self.tokens = re.findall("[-\(\)]|[!<>=]=|[<>]|\w+|[^ +]\W+", self.expr)
        # transform tokens to lower case, and filter out possible spaces in the tokens
        self.tokens = list(filter((lambda x: len(x)), 
                           list(map((lambda x: x.strip().lower()), self.tokens))))    
    
    # parser - determine if the input expression is valid or not
    
    # validate() function will return True if the expression is valid, False otherwise 
    # do NOT change the method signature 

    def validate(self):
        # Using the tokens from lex() tokenization,
        # this validate would first call lex() to tokenize the expression,
        # then call the top level parsing procedure for the outermost rule
        # and go from there
        
        self.lex()
        print(self.tokens)
        i = 0
        return self.EXP(self.tokens[0], i)

        # return False # replace with True if the expression is valid! 

    # TODO: Write your parsing procedures corresponding to the grammar rules - follow Figure 5.17

    # Check EXP
    def EXP(self, token, i):
        print(f"i {i}")
        FOUND = False
        print(self.tokens[i])
        if (self.TERM(i)):
            FOUND = True
            while (self.inOP(self.tokens[i]) and FOUND):
                i += 1
                if (not self.TERM(i)):
                    FOUND = False
        
        return FOUND
    
    def TERM(self, i):
        FOUND = False
        if (i < len(self.tokens) and self.tokens[i].isdigit()):
            i += 1
            print("int")
            if (i < len(self.tokens) and self.tokens[i] == '-'):
                i += 1
                print("dash")
                if (i < len(self.tokens) and self.tokens[i].isdigit()):
                    i += 1
                    FOUND = True
                    print("int")
        elif (i < len(self.tokens) and self.inRELOP(self.tokens[i])):
            i += 1
            print("relop")
            if (i < len(self.tokens) and self.tokens[i].isdigit()):
                i += 1
                FOUND = True
                print("int")
        elif (i < len(self.tokens) and self.tokens[i] == '('):
            print("(")
            i += 1
            if (i < len(self.tokens) and self.EXP(self.tokens[i], i)):
                print(f"EXP {self.tokens[i]}")
                if (i < len(self.tokens) and self.tokens[i] == ')'):
                    print(")")
                    FOUND = True
                    i += 1
        return FOUND

    def inOP(self, token):
        for ii in self.op:
            if ii == token:
                return True
        return False

    def inRELOP(self, token):
        for ii in self.relop:
            if ii == token:
                return True
        return False

# sample test code, use examples from the prompt for more testing 
# positive tests: validation of the expression returns True
# negative tests: validation of the expression returns False  

# Valid
r = recDescent('5 - 100') 
print(r.validate()) 
r = recDescent('7 - 17')
print(r.validate())
r = recDescent('> 90')
print(r.validate())
r = recDescent('(1 - 100 and not 50) or > 200')
print(r.validate())
r = recDescent('(7 - 17) or > 90')
print(r.validate())
r = recDescent('> 50 or == 20')
print(r.validate())
r = recDescent('1 - 100 and != 50')
print(r.validate())
r = recDescent('(5 - 100) and (not 50) or (>= 130 or (2 - 4))')
print(r.validate())

# Invalid
r = recDescent('>')
print(r.validate())
r = recDescent('2 - - 4')
print(r.validate())
r = recDescent('- 7')
print(r.validate())
r = recDescent('7 -')
print(r.validate())
r = recDescent('= 6')
print(r.validate())
r = recDescent('(!= 5) and')
print(r.validate())
r = recDescent('2 - 4 and >< 300')
print(r.validate())
r = recDescent('>= 5) xnor < 10')
print(r.validate())


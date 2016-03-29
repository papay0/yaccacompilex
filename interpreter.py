import sys
from optparse import OptionParser
import inspect

if len(sys.argv) < 2:
    print("usage: interpreter.py <asmfile> [<mode>]");
    sys.exit(1)

mode = 0
if len(sys.argv) == 3:
    mode = int(sys.argv[2])

f = open(sys.argv[1], 'r');
instructions = [[v.strip() for v in line.lower().strip('\n').split(' ')] for line in f]
f.close()

def col(n):
    return "\033[0;" + str(n) + "m"

def endcol():
    return "\033[0m"

STATUS_OK = 0
STATUS_ASSERT_FAILED = 1
class Debugger:
    def __init__(self, instructions, mode=0):
        self.ip = 0
        self.mode = mode
        self.instructions = instructions;
        self.memory = [0 for i in range(0, 256)]
        self.args_c = 0
        self.ctx = 0
        self.sp = 0
        self.members = dict()
        self.members.update(inspect.getmembers(self, predicate=inspect.ismethod))
        self.ended = False
        self.return_code = STATUS_OK
        self.run_active = False
        self.breakpoints = []
        self.localvars = [dict()]
        self.area = ""

    def tohex(self, v):
        return format(int(v), "02") 
        
    def addr(self, v):
        if(v.startswith('@')):
            return int(v[1:])

        self.sp = max(self.sp, self.ctx + int(v))
        return int(v) + self.ctx

    def op_add(self, line, params):
        to, o1, o2 = params
        self.memory[self.addr(to)] = self.memory[self.addr(o1)] + self.memory[self.addr(o2)]

    def op_sub(self, line, params):
        to, o1, o2 = params
        self.memory[self.addr(to)] = self.memory[self.addr(o1)] - self.memory[self.addr(o2)]

    
    def op_mul(self, line, params):
        to, o1, o2 = params
        self.memory[self.addr(to)] = self.memory[self.addr(o1)] * self.memory[self.addr(o2)]
 
    def op_cop(self, line, params):
        dst, src = params
        self.memory[self.addr(dst)] = self.memory[self.addr(src)]

    def op_copa(self, line, params):
        dst, src = params
        self.memory[self.addr(dst)] = self.memory[self.addr("@" + str(self.memory[self.addr(src)]))]

    def op_copb(self, line, params):
        dst, src = params
        self.memory[self.memory[self.addr(dst)]] = self.memory[self.addr(src)]

    def op_afc(self, line, params):
        dst, val = params
        if("@" in dst): # hack pour le stack pointeur
            self.sp = max(self.sp, self.addr(dst) + 1);
        self.memory[self.addr(dst)] = int(val)
    	
    def op_ptr(self, line, params):
        dst, val = params
        self.memory[self.addr(dst)] = self.addr(val)

    def op_jmp(self, line, params):
        dst = params[0]
        self.ip = int(dst) - 1

    def op_jmf(self, line, params):
        cond, dst = params[0], params[1]
        if self.memory[self.addr(cond)] == 0:
            self.ip = int(dst) - 1

    def op_asrt(self, line, params):
        val = self.memory[self.addr(params[0])]
        if val == 0:
            print(col(31) + "assert: failed" + endcol())
            self.ended = True
            self.return_code = STATUS_ASSERT_FAILED

    def op_inf(self, line, params):
        dst, op1, op2 = params
        o1 = self.addr(op1)
        o2 = self.addr(op2)
        self.memory[self.addr(dst)] = 1 if self.memory[o1] < self.memory[o2] else 0

    def op_eq(self, line, params):
        dst, op1, op2 = params
        o1 = self.addr(op1)
        o2 = self.addr(op2)
        self.memory[self.addr(dst)] = 1 if self.memory[o1] == self.memory[o2] else 0

    def op_neq(self, line, params):
        dst, op1, op2 = params
        o1 = self.addr(op1)
        o2 = self.addr(op2)
        self.memory[self.addr(dst)] = 1 if self.memory[o1] != self.memory[o2] else 0


    def op_sup(self, line, params):
        dst, op1, op2 = params
        o1 = self.addr(op1)
        o2 = self.addr(op2)
        self.memory[self.addr(dst)] = 1 if self.memory[o1] > self.memory[o2] else 0

    def op_prid(self, line, params):
        val, char = params[0], params[1]
        print("\033[0;34mprint: ", char, " = ", self.memory[self.addr(val)], "\033[0m")

    def op_pri(self, line, params):
        val = params[0]
        print("\033[0;34mprint: ", self.memory[self.addr(val)], "\033[0m")
    
    def op_ret(self, line, params):
        # On positionne le pointeur d'instruction
        self.ip = self.memory[self.ctx-2] - 1

        # On restitue le sp précédent
        self.sp = self.memory[self.ctx-1]
        
        # On restitue le contexte précédent
        self.ctx = self.memory[self.ctx]

        if(self.ip == -1):
            self.ended = True
            print("program terminated.")

        # On pop les variables locales [debug]
        self.localvars.pop()

    def op_call(self, line, params):
        ar_addr = self.sp
        sp_addr = self.sp + 1
        ctx_addr = self.sp + 2
        new_sp = ctx_addr;
        # Arguments et @ de retour déjà sauvegardés
        # on empile le contexte 
        self.memory[ctx_addr] = self.ctx
        self.memory[sp_addr] = self.sp - self.args_c
        self.ctx = ctx_addr
        self.sp = new_sp

        self.ip = self.memory[self.addr(params[0])] - 1
        self.ip -= 1 # ceci est pour tomber sur les macros .function

        # On push les variables locales [debug]
        self.localvars.append(dict())
        #for i in range(0, self.args_c):
        #    self.localvars[-1][self.ctx - 2 - self.args_c + i] = "(arg" + str(i) + ")"

 
    def macro_function(self, line, params):
        self.area = params[0]

    def macro_local(self, line, params):
        self.sp = self.sp + 1
        self.localvars[-1][self.sp] = params[0]

    def macro_stacksize(self, line, params):
        self.sp = self.ctx + int(params[0])
        pass

    def macro_popargs(self, line, params):
        self.args_c = int(params[0]);
        pass
        

    def macro_ra(self, line, params):
        pass

    def print_addr(self, val):
        add = "\t" + col(32)
        if val == self.ctx:
            add += "(ctx) "
        if val == self.ctx - 1:
            add += "(sp) "
        if val == self.ctx - 2:
            add += "(@) "
       
        if val in self.localvars[-1].keys():
            add += "(" + self.localvars[-1][val] + ") "

        
        if val > self.sp and val <= self.sp + self.args_c:
            argn = val - self.sp
            add += "(arg" + str(argn) + ") "

        if val == self.sp:
            add += "<-- sp "

        add += endcol()
        print(col(31) + "@" + self.tohex(val) + endcol() +  ":" + self.tohex(val-self.ctx) + ":", 
            self.tohex(self.memory[val]), add)

    def command(self, command):
        cmds = command.split(' ')
        cmd, args = cmds[0], cmds[1:]
        
        if "ctx" in cmd: print("ctx = ", self.tohex(self.ctx))
        if "sp" in cmd: print("sp = ", self.tohex(self.sp))
        if "frame" in cmd:
            print("[in function", self.area + "]:", "ctx =", self.tohex(self.ctx),
                    " | sp = ", self.tohex(self.sp), "| argc =", self.tohex(self.args_c))
            for i in range(max(self.ctx-5, 0), self.sp+self.args_c+5):
                self.print_addr(i)
        if "mem" in cmd:
            s = self.addr(args[0])
            e = s
            if len(args) == 2:
                e = self.addr(args[1])
            for i in range(s, e+1):
                self.print_addr(i)

        if "run" in cmd:
            self.run_active = True
            return True
        if "b" in cmd:
            self.breakpoints.append(int(cmds[1]))
          

        return command == ""

    def run(self):
        # Mode d'exécution réel
        if(self.mode == 0):
            while(not self.ended and self.ip < len(self.instructions)):
                self.safe_call(self.instructions[self.ip])
                self.ip += 1
        # Mode debug
        elif(self.mode == 1):
             while(not self.ended and self.ip < len(self.instructions)):
                print(self.tohex(self.instructions[self.ip][0][:-1]),
                      ' '.join(self.instructions[self.ip][1:]))
                if self.run_active:
                    if self.ip in self.breakpoints:
                        self.run_active = False

                if not self.run_active:
                    while(not self.command(input(col(36) + "(ycdb) " + endcol()))):
                         pass
                self.safe_call(self.instructions[self.ip])
                self.ip += 1

    def safe_call(self, instruction):
        name, line, params = "op_" + instruction[1], instruction[0], instruction[2:]
        if(name in self.members):
            self.members[name](line, params)
        elif(not "." in name):
            print("instruction ", name, " not implemented");
            self.ended = True
        else: 
            if "." in name:
                name = "macro_" + name[4:]
                if name in self.members:
                    self.members[name](line, params)



s = Debugger(instructions, mode)
try:
    s.run()
except:
    sys.exit(1)

sys.exit(s.return_code)

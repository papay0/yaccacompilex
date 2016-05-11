import sys
import collections
from optparse import OptionParser
import inspect

try:
    import readline
except:
    pass 


def col(n):
    return "\033[0;" + str(n) + "m"

def endcol():
    return "\033[0m"
def col2(s, n):
    return col(n) + s + endcol()


STATUS_OK = 0
STATUS_ASSERT_FAILED = 1
MemSeg = collections.namedtuple('MemSeg', 'addr size')
Func = collections.namedtuple('Func', 'argc')
class Debugger:
    
    def __init__(self, instructions, mode=0):
        self.source = ""
        self.mem_size = 1024
        self.stack_size = 512
        self.ip = 0
        self.mode = mode
        self.instructions = instructions;
        self.areas = self.parse_areas(instructions)
        self.functions = self.parse_functions(instructions)
        self.memory = [0 for i in range(0, self.mem_size)]
        self.dynamic_allocs = [] # tuples (addr, size)
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

    def parse_functions(self, instructions):
        """Associe à chaque fonction certaines métadonnées"""
        functions = dict()
        current = "<unknown>"
        for instruction in instructions:
            name, line, params = instruction[1], instruction[0], instruction[2:]
            
            if(name == ".function"):
                functions[params[0]] = Func(int(params[1]))

        return functions

    def parse_areas(self, instructions):
        """Associe à chaque instruction sa zone de code"""
        areas = []
        current = "<unknown>"
        for instruction in instructions:
            name, line, params = instruction[1], instruction[0], instruction[2:]
            
            if(name == ".area"):
                current = params[0]
            elif(name == ".function"):
                current = params[0]
    
            areas.append(current)

        return areas

    def tohex(self, v):
        return format(int(v), "02") 
        
    def print_instruction(self, i):
        print(self.instruction_to_str(i))

    def instruction_to_str(self, i):
        return self.tohex(self.instructions[i][0][:-1]) + " " +  ' '.join(self.instructions[i][1:])


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

    def op_and(self, line, params):
        to, o1, o2 = params
        v1 = self.memory[self.addr(o1)]
        v2 = self.memory[self.addr(o2)]
        self.memory[self.addr(to)] = 1 if (v1 == True) and (v2 == True) else 0;

    
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

    def get_args(self, ctx, ip):
        args = []
        area = self.areas[ip]
        if area in self.functions.keys():
            argc = self.functions[area].argc
            argn = 0
            for i in range(ctx-argc-2, ctx-2):
                args.append("arg" + str(argn) + "=" + str(self.memory[i]))
                argn += 1
        return args

    def format_trace(self, ip, args, ctx, sp):
            return col2(self.tohex(ip), 34) + " in " + col2(self.areas[ip], 35) + "(" + ", ".join(args) + ")" + col2(" ctx = " + str(ctx) + ", sp = " + str(sp), 32)

    def print_backtrace(self):
        ctx = self.ctx
        print(self.format_trace(self.ip, self.get_args(self.ctx, self.ip), self.ctx, self.sp) + " : ") 
        while(ctx != 0):
            ip = self.memory[ctx-2]
            sp = self.memory[ctx-1]
            ctx = self.memory[ctx]
            area = self.areas[ip]
            
            args = self.get_args(ctx, ip); 
            print("\tfrom " + self.format_trace(ip, args, ctx, sp));


    
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

        # Placement de l'instruction pointeur
        self.ip = self.memory[self.addr(params[0])] - 1
        self.ip -= 1 # ceci est pour tomber sur les macros .function

        # Arguments et @ de retour déjà sauvegardés
        # on empile le contexte 
        self.memory[ctx_addr] = self.ctx
        self.memory[sp_addr] = self.sp - self.args_c
        self.ctx = ctx_addr
        self.sp = new_sp

        
        # On push les variables locales [debug]
        self.localvars.append(dict())
        #for i in range(0, self.args_c):
        #    self.localvars[-1][self.ctx - 2 - self.args_c + i] = "(arg" + str(i) + ")"

    def op_fre(self, line, params):
        addr = self.memory[self.addr(params[0])]
        segment = None
        for seg in self.dynamic_allocs:
            if seg.addr == addr:
                segment = seg
                break

        # Si le segment n'existe pas : erreur
        if segment == None:
            print(col(31) + "free: no memory to free at address " + str(addr) + endcol())
            self.return_code = 1
            self.ended = True
            return

        # Si le segment existe, on le supprime
        self.dynamic_allocs.remove(seg)
        
    def op_mal(self, line, params):
        dst_addr, size_addr = params[0], params[1]
        size = self.memory[self.addr(size_addr)]
        dst = self.addr(dst_addr);

        alloc_addr = self.stack_size
        for seg in self.dynamic_allocs:
            alloc_addr = max(seg.addr+seg.size, alloc_addr)
        
        self.dynamic_allocs.append(MemSeg(alloc_addr, size))
        if alloc_addr + size > self.mem_size:
            print(col(31) + "malloc: not enough memory" + endcol())
            self.return_code = 1
            self.ended = True
            return

        self.memory[dst] = alloc_addr;

 
    def macro_function(self, line, params):
        self.area = params[0]

    def macro_local(self, line, params):
        address = self.ctx + int(params[0])
        name = params[1]
        self.localvars[-1][address] = name

    def macro_array(self, line, params):
        name = params[0]
        address = self.ctx + int(params[1])
        size = int(params[2])
        self.localvars[-1][address] = name
        for i in range(0, size):
            self.localvars[-1][i+address+1] = name + "[" + str(i) + "]";


    def macro_stacksize(self, line, params):
        self.sp = self.ctx + int(params[0])
        pass

    def macro_popargs(self, line, params):
        self.args_c = int(params[0]);
        pass
        

    def macro_ra(self, line, params):
        pass

    def macro_file(self, line, params):
        name = params[0]
        f = open(name, 'r');
        self.source = "".join([line for line in f])
        f.close()


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
        
        area = self.areas[self.ip]
        if area in self.functions.keys():
            argc = self.functions[area].argc
            for i in range(0, argc):
                addr = self.ctx-2-argc+i
                if val == addr:
                    add += "(arg" + str(i) + ") "
        
        if val == self.sp:
            add += "<-- sp "

        add += endcol()
        print(col(31) + "@" + self.tohex(val) + endcol() +  ":" + self.tohex(val-self.ctx) + ":", 
            self.tohex(self.memory[val]), add)

    def command(self, command):
        cmds = command.split(' ')
        cmd, args = cmds[0], cmds[1:]
        
        if "ctx" == cmd: print("ctx = ", self.tohex(self.ctx))
        elif "sp" == cmd: print("sp = ", self.tohex(self.sp))
        elif "frame" == cmd:
            print("[in function", self.area + "]:", "ctx =", self.tohex(self.ctx),
                    " | sp = ", self.tohex(self.sp), "| argc =", self.tohex(self.args_c))
            for i in range(max(self.ctx-5, 0), self.sp+self.args_c+5):
                self.print_addr(i)

        elif "asm" == cmd:
            for inst in range(0, len(self.instructions)):
                self.print_instruction(inst)

        elif "source" == cmd:
            print(self.source)
        
        elif "backtrace" == cmd:
            self.print_backtrace();

        elif "mem" == cmd:
            s = self.addr(args[0])
            e = s
            if len(args) == 2:
                e = self.addr(args[1])
            for i in range(s, e+1):
                self.print_addr(i)

        elif "run" == cmd:
            self.run_active = True
            return True
        elif "b" == cmd:
            line = int(cmds[1])
            if line < 0 or line >= len(self.instructions):
                print(col(31) + "error :" + endcol() + " no instruction at line " + str(line) + ".")  
            elif line in self.breakpoints:
                print("Removed break point at instruction : " + str(line))
                print(col(31) + "_ " + self.instruction_to_str(line) + endcol())
                self.breakpoints.remove(line)
            else:
                print("Added break point at instruction : " + str(line))
                print(col(32) + "b " + self.instruction_to_str(line) + endcol())

                self.breakpoints.append(int(cmds[1]))
            
        elif "help" == cmd:
            print("Commands : ")
            print("asm          : show assembly code")
            print("source       : show c source code")
            print("ctx          : show ctx value")
            print("sp           : show stack pointer value")
            print("frame        : show current frame")
            print("backtrace    : show the backtrace")
            print("mem s        : show the content of the memory at address s")
            print("mem s e      : show the content of the memory from s to e")
            print("Addressing modes : \n\t@xxx : address in memory\n\txxx : address in memory starting at current context")
              
        elif len(cmd.strip()) != 0:
            print(col(31) + "Error :" + endcol() + " unrecognized command '" + cmd + "'")
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
                self.print_instruction(self.ip)
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


# Initialisation du script
if len(sys.argv) < 2:
    print("usage: interpreter.py <asmfile> [<mode>]");
    sys.exit(1)

mode = 0
if len(sys.argv) == 3:
    mode = int(sys.argv[2])

f = open(sys.argv[1], 'r');
instructions = [[v.strip() for v in line.lower().strip('\n').split(' ')] for line in f]
f.close()

s = Debugger(instructions, mode)
try:
    s.run()
except IOError as e:
    sys.exit(1)

sys.exit(s.return_code)

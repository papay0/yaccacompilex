import sys
import subprocess


def col(n):
    return "\033[0;" + str(n) + "m"

def endcol():
    return "\033[0m"

mode = 0
if len(sys.argv)  == 1:
    tests = ["testbench.c", "facto.c", "func.c", "while.c", "ptr.c", "malloc.c", "func_sale.c",
             "if_1.c", "if_2.c", "while2.c", "while3.c", 
             "if_elsif_else.c", "if_else.c", "if_elsif.c", "if_elsif_elsif.c", "if_elsif_elsif_else.c", "if_else_2.c", "complex.c",
             "benoit_funcptr.c", "while_benoit.c", 
            ] #   "errors.c", "syntexerr.c", "assertfalse.c"]
    success_count = 0
    error_count = 0
    for test in tests:
        print(col(4) + "+ Testing " + test + endcol());
        outfile = "build/" + test + ".asm";
        ret = subprocess.call("bin/parser test/" + test + " > /dev/null -o " + outfile, shell=True)
        if ret != 0:
            print("\t..." + col(31) + test + ": Compilation FAILED" + endcol())
            error_count += 1
            continue
        print("\t... " + col(32) + test + ": Compilation SUCCESSFUL" + endcol())

        ret = subprocess.call("python3 interpreter.py " + outfile + " 0 > /dev/null", shell=True)
        if ret == 0:
            print("\t... " + col(32) + test + ": ASSERTIONS OK" + endcol())
            success_count += 1
        else:
            msg = "ASSERTIONS FAILED" if ret == 1 else "EXECUTION FAILED"
            print("\t... " + col(31) + test + ": " + msg + endcol())
            error_count += 1

    print("================================================")
    if(error_count == 0):
        print(col(32) + "Test bench ended with 0 errors." + endcol());
    else:
        print(col(31) + "Test bench ended with " + str(error_count) + " errors." + endcol());

elif len(sys.argv)  >= 2:
    test = sys.argv[1]
    mode = "1" if len(sys.argv) == 2 else sys.argv[2]
    outfile = "build/" + test.replace('/', '_') + ".asm"
    command = "bin/parser " + test +" -o " + outfile + " > /dev/null " + "&& python3 interpreter.py " + outfile + " " + mode;
    print(command);
    subprocess.call(command, shell=True)




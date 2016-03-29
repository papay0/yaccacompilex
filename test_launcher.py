import sys
import subprocess


def col(n):
    return "\033[0;" + str(n) + "m"

def endcol():
    return "\033[0m"

mode = 0
if len(sys.argv)  == 1:
    tests = ["table.c", "facto.c", "func.c", "while.c", "ptr.c"]
    for test in tests:
        print("+ Testing", test);
        ret = subprocess.call("bin/parser < test/" + test + " > bin/parser_out", shell=True)
        if ret != 0:
            print("\t..." + col(31) + test + ": Compilation FAILED" + endcol())
            continue

        ret = subprocess.call("python3 interpreter.py bin/yaccacompilex 0 > bin/interp_out", shell=True)
        if ret == 0:
            print("\t... " + col(32) + test + ": OK" + endcol())
        else:
            print("\t... " + col(31) + test + ": FAILED" + endcol())

elif len(sys.argv)  == 2:
    test = sys.argv[1]
    subprocess.call("bin/parser < " + test + " > bin/parser_out" + "&& python3 interpreter.py bin/yaccacompilex 1", shell=True)
    mode = 1




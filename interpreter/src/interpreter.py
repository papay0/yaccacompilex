class Interpreter:
    def __init__(self):
        self.DB = []

    def add(self, op, arg1, arg2, arg3=None):
        if arg3 == None:
            self.DB.append((op, arg1, arg2))
        else:
            self.DB.append((op, arg1, arg2, arg3))


    def printDB(self):
        for elmt in self.DB:
            print(elmt)



--
int redef(int a);
void redef(char a) { } // redéfinition

int doubledef(int c);
int doubledef(void* d); // redéfinition avec types incompatibles


int f(int a, char b) { return 1; }
int main()
{
    f(56, 565656; // syntax error

    int a;
    char a; // redéfinition

    if(a == 0) { int a; } // OK

    int(*)(int, int) b = f; // affectation de type incompatibles
    f((char)5, (int)5); // mauvais types d'arguments
    f(0);       // mauvais nombre d'arguments
    f(0, 1, 2); // mauvais nombre d'argument
    b(&b, &f); // mauvais types d'arguments
    a(); // appel de non-fonction
    b = f + 5; // opération ADD non valide
    
    implicit = 5; // déclaration implicite de symbole

    c = implicit2 + 5; // déclaration implicite de symbole

    int * ptr;
    int integer;
    ptr = ptr + integer; // OK
}



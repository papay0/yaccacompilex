int global;

--

int f1(int a)
{

}

int f2(int b)
{

}

void f3(void* c)
{

}

int main()
{
    char chr;
    int i;

    f1(chr); // erreur : convert char / int
    f1(*i); // erreur : derefence
    f1(chr, i); // erreur : arguments
    f1();       // erreur : arguments
    f1(implicit); // erreur : implicit declaration
    
    chr = (int)6; // pas erreur
    i = (char)8; // pas erreur
    i = (void)9; // erreur    

    int* ptr;
    int number;
    ptr = ptr + number; // pas erreur
    number = ptr + number; // erreur
    ptr = ptr + ptr; // pas erreur
    
    int*** ptr3;
    int** ptr2;
    ptr2 = ptr3; // erreur

    f3(ptr3);
}

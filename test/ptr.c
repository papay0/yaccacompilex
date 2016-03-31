int global = 5;
--
int swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 0;
}

int main()
{
    // Test de swap
    int a = 5;
    int b = 10;
    swap(&a, &b);
    assert(a == 10);
    assert(b == 5);
    // Opérateur & sur une globale
    assert(&global == (int*)0);
    global = *(&global) + 5;
    assert(global == 10);
    *global = 10;
    // Tableaux
    int value[10];
    // *(value+1) = 4;
    // value[1] = 4;
    int* c = value + 1;
    *c = 50;
    *(value+2) = 51;
    value[3] = 52;
    
    assert(value[1] == 50);
    assert(*(value+1) == 50);
    assert(value[2] == 51);
    assert(value[3] == 52);

}

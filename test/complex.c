int a = 100;
int b = 200;
int c = 300;
int d = 400;
int e = 500;
int garray[10];

--

int main()
{
    int array[10];
    int i = 0;
    int* dinarray = (int*)malloc(10);

    // Vérifie que les addresses sont traitées correctement.
    assert((array < &i) && (array > &e));
    assert((garray < array) && (garray > &e));
    assert(dinarray > (int*)500);
    while(i < 10)
    {
        array[i] = i;
        garray[i] = i;
        dinarray[i] = i;
        // On vérifie que le pointeur correspond à la bonne zone mémoire.
        // On vérifie que array[i] est correct.
        assert(array[i] == i);
        assert(garray[i] == i);
        assert(dinarray[i] == i);
        i = i + 1;
    }
    
    print(i);
}

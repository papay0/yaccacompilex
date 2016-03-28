int a = 10;
int b = 20;
// dé commentair
--

int mult(int* n)
{
    *n = *n * 2;
    return *n;
}

int g2(int x, int y)
{
    return y - x;
}

int g3(int x, int y, int z)
{
    return x + y + z;
}
int main()
{
    int a = 1;
    int t = g3(100, g2(210, 220), 300);
    print(t);
    assert(t == 410);
    //truc(mult(&a), a);
    //print(a); // 4

    //mult(&a);
    //print(a); // 8
    //print(exit, e);
}
/*
int facto(int n, int n2)
{
    n = 80;
    print(n);
    return n+1;
}

int glglgl(int g) { 
    int f;
    int h;
    int i;
}
int main()
{
    int c = 50;
    int d = 60;
    int kode = 70;
    int* e = &c;
    a = facto(5, 3);
    int* b = &a;

    print(b, b);
    print(vb, *b);
    print(a, a);
    print(e, e);

    print(ve, *e);
    print(ac, &c);
    print(c, c);
    print(a, a);
}*/

/*
int ha(int arg0, int arg1)
{ 
    a = 256; 
    arg0 = 70;
    arg1 = 90;
}
int mainiii() {
    a = 55;
    int e = 56;
    int d = 45;
    int e =  = ++ -; // erreur de syntaxe : pas de soucis !
    // utilisation de symboles qui n'existent pas.
    z = y + &x;
}
int pri1(int v)
{
    int b = v;
}
int pri(int v)
{
    int a = v;
    pri1(v + 1);   
}
int main() 
{
    int a = 0;
    pri(a);
    a = a + 1;
    pri(a);
    a = a + 1;
    pri(a);
}
int hoho(int a, int c)
{
    int* g = &a;
    *g = *g + 5;
    // ha(777, *g, 10 + 20 * 30, (4+(1+(3+(9)))), 100, 200);
    ha(g, &g);
    ha();
    ha(1, 2);
    g(56);
    int tt = 5656;
}
*/

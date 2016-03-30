--
int p(int x, char y)
{
    print(x);
    print(y);
    return x;
}

int p2(int x, char y)
{
    print(x+y);
    return x+y;
}

int pf(int (*)(int, char) pr, int x, char y)
{
    return pr(x, y);
}

int main()
{
    int banane = 9;  
    int pf_func = 15;
    int(*)(int(*)(int, char), int, char) pf_func2 = (int(*)(int(*)(int, char), int, char))15;
    assert(pf_func(banane, 20, (char)20) == 40);
    assert(pf_func2(banane, 20, (char)20) == 40);
    assert(pf(p, 5, (char)10) == 5);
    assert(pf(p2, 5, (char)10) == 15);
}

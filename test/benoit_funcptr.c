--

int f(int arg)
{
    int a;
    return 1;
    return 0;
}

int main()
{
    int(*) (int)  f2;
    f2 = f + 5;
    print(f2());
    assert(f2() == 0);
}

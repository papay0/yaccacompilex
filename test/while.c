--

int main()
{
    int a = 0;
    int b = 0;
    while(a < 10)
    {
        print(a);
        b = 0;
        while(b < 10)
        {
            if(b < 5) { print(a, a); print(b, b); }
            b = b + 1;
        }
        a = a + 1;
    }
    assert(a == 10);
    assert(b == 10);
}

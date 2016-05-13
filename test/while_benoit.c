--

int main()
{
    int a = 0;
    int b = 0;
    while(a < 10) {
        b = 0;
        while(b < 10) {
            b = b + 1;
        }
        a = a + 1;
    }
    
    print(a);
    print(b);
    assert(a == 10);
    assert(b == 10);
}

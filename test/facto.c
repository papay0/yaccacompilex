--

int facto(int n, int b)
{
    if(n == 0) { return 1; }
    return n * facto(n - 1, n - 1);
}
int main()
{
    int a = facto(5, 5);
    assert(a == 120);

}

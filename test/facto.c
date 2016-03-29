--

int facto(int n)
{
    if(n == 0) { return 1; }
    return n * facto(n - 1);
}
int main()
{
    int a = facto(5);
    assert(a == 120);

}

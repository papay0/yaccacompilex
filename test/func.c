--
int g2(int x, int y)
{
    return x + y;
}

int g3(int x, int y, int z)
{
    return x + y + z;
}

int main()
{
    int a = g2(1, 2) * g3(40, g2(20, 30), 30+30);
    assert(a == 450);
}

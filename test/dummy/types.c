--

int main()
{
    int(*) (int*, char, int*) a;
    char** (*) (int*, char(*) (int, void**), int) b;
    char** (*) (int*, char(*) (int, void**), int) c;



    int t;
    t = a;
    t = b;
}

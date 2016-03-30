--
int main()
{
    int* ptr = malloc(10);
    int* ptr2 = malloc(20);
    int* ptr3 = malloc(30);
    *ptr = 9;
    void v = free(ptr);
    print(ptr);
    print(ptr2);
    assert(ptr == 512);
    assert(ptr2 == 522);
    assert(ptr3 == 542);
    print(ptr[0]);
/*
    int* ptr5 = ptr + 5;
    *ptr5 = 10;
    print(ptr[5]); */
}

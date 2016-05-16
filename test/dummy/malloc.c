--
int *get_ptr()
{
    int* ptr = malloc(50);
    ptr[20] = 50;
    return ptr;
}

int main()
{
    int* ptr = get_ptr();
    assert(ptr[20] == 50);
    print(ptr[20]);

    free(ptr);
    free(ptr); // erreur !!
}



--
int haha(int truc);
int haha(char truc) { return truc+5; }
int main() {
    int(*)(int) a = haha;
    int(*)(char) b = haha;
    assert(haha(1) == 6);
}

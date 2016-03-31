--
int main(){
    int i = 0;
    int ok[2];
    while(i < 2)
    {
        if (i == 0){
            ok[0] = 1;
            assert(i == 0);
        } else if (i == 1) {
            ok[1] = 1;
            assert(i == 1);
        }
        assert(ok[i]);
        i = i + 1;
    }
}

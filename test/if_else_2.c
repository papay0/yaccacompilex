--
int main(){
    int a = 0;
    int ok[2];
    while(a < 2)
    {
        if (a==0){
            int b = 2;
            while (b < 3){
                b = b + 1;
            }
            ok[0] = 1;
        } else {
            ok[1] = 1;
        }
        assert(ok[a]);
        a = a + 1;
    }
    assert(1);
}

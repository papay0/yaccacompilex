--
int main(){
    int i = 0;
    int ok[4];
    while(i < 4)
    {
        if(i == 0){
            assert(i == 0);
            print(i);
            ok[0] = 1;
        } else if (i == 0){
            assert(0);
            ok[1] = 1;
        } else if (i == 0) {
            assert(0);
            ok[2] = 1;
        }
        else {
            assert(i > 2);
        }
        i = i +  1;
    }
}

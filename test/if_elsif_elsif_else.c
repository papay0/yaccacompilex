--
int main(){
    int i = 0;
    int ok[4];
    while(i < 4)
    {
        if(i == 0){
            assert(i == 0);
            ok[0] = 1;
        } else if (i == 1){
            assert(i == 1);
            ok[1] = 1;
        } else if (i == 2) {
            assert(i == 2);
            ok[2] = 1;
        }
        else {
            assert(i > 2);
            ok[3] = 1;
        }
        // On vérifie qu'on est bien passé par la bonne branche.
        assert(ok[i]);
        i = i +  1;
    }
}

--
int main(){
    int i = 0;
    int ok[3];
    while(i < 3)
    {
        if(i == 0){
            assert(i == 0);
            ok[0] = 1;
        } else if (i == 1){
            assert(i == 1);
            ok[1] = 1;
        } else {
            assert(i > 1);
            ok[2] = 1;
        }
        // On vérifie qu'on est bien passé par la bonne branche.
        assert(ok[i]);
        i = i +  1;
    }

    
}

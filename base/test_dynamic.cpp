
#include <stdio.h>
#include "dynamic_pool.h"

template<> StaticPool<Node<int* >* > DynamicPool<int* >::avaliable(4);
DynamicPool<int*> qi(4);

void init(){
    int * all = ::new int[4];
    for(int i=0; i<4; ++i){
        qi.push(&all[i]);
    }
}

int main(){
    int *m = qi.pop();
    int *n = qi.pop();

    fprintf(stderr, "m=%d, n=%d\n", *m, *n);
    return 0;
}

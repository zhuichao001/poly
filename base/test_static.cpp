
#include <stdio.h>
#include "static_pool.h"

StaticPool<int*> qi(2);

int main(){
    int i = 6;
    int j = 8;

    qi.push(&i);
    fprintf(stderr, "push:%d\n", &i);

    qi.push(&j);
    fprintf(stderr, "push:%d\n", &j);

    int *m=nullptr;
    int *n=nullptr;
    int *x=nullptr;

    bool b = qi.pop(&m);
    fprintf(stderr, "%d pop:%d\n", b, m);

    b = qi.pop(&n);
    fprintf(stderr, "%d pop:%d\n", b, n);

    b = qi.pop(&x);
    fprintf(stderr, "%d pop:%d\n", b, x);

    return 0;
}

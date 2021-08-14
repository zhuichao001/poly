
#include <stdio.h>
#include "ring.h"


int main(){
    Ring<int> qi;
    int i = 6;
    int j = 8;

    qi.push(i);
    qi.push(j);

    int m, n;
    qi.pop(&m);
    qi.pop(&n);

    fprintf(stderr, "m=%d, n=%d\n", m, n);
    return 0;
}


#include <stdio.h>
#include "queue.h"


int main(){
    queue<int> qi;
    int *i=new int(3);
    int *j=new int(5);

    qi.push(i);
    qi.push(j);
    int *m = qi.pop();
    int *n = qi.pop();

    fprintf(stderr, "m=%d, n=%d\n", *m, *n);
    return 0;
}

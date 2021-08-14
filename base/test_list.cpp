
#include <stdio.h>
#include "list.h"


int main(){
    List<int> qi;
    int *i=new int(3);
    int *j=new int(5);

    qi.push(i);
    qi.push(j);
    int *m = qi.pop();
    int *n = qi.pop();

    fprintf(stderr, "m=%d, n=%d\n", *m, *n);
    return 0;
}

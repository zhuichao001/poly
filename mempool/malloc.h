#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace poly{


void *malloc(siz_t size){
    void * ptr;
    if(size >= 128<<10){
        ptr = (char*)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    }else if(size<128){
    }
}

}//end of namespace poly

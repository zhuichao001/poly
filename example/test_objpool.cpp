#include "message.h"


const int DEFAULT_POOL_SIZE = 4; 
template<> StaticPool<Node<Message*>*> DynamicPool<Message*>::avaliable(DEFAULT_POOL_SIZE+1);

DynamicPool<Message*> Message::pool;

void InitObjPool(){
    Message *objects = ::new Message[DEFAULT_POOL_SIZE];
    for(int i=0; i<DEFAULT_POOL_SIZE; ++i){
        Message::pool.push(&objects[i]);
    }
}

int main(){
    fprintf(stderr, "---before init object pooln");
    InitObjPool();
    fprintf(stderr, "---after init object pooln");
    Message *m1 = new Message("msg first");
    fprintf(stderr, "new message 1 ok\n");
    Message *m2 = new Message("msg second");
    fprintf(stderr, "new message 2 ok\n");
    delete m1;
    fprintf(stderr, "delete message 1 ok\n");
    delete m2;
    fprintf(stderr, "delete message 2 ok\n");
    return 0;
}

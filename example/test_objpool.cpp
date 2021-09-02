#include "message.h"


const int DEFAULT_POOL_SIZE = 4; 
template<> StaticPool<Node<Message*>*> DynamicPool<Message*>::avaliable(DEFAULT_POOL_SIZE);

DynamicPool<Message*> Message::pool;

void InitObjPool(){
    Message *objects = ::new Message[DEFAULT_POOL_SIZE];
    for(int i=0; i<DEFAULT_POOL_SIZE; ++i){
        bool ok = Message::pool.push(&objects[i]);
        fprintf(stderr, "prepare ok:%d\n", ok);
    }
}

void test1(){
    Message *m1 = new Message("msg first");
    fprintf(stderr, "new message 1 ok\n");

    Message *m2 = new Message("msg second");
    fprintf(stderr, "new message 2 ok\n");

    delete m1;
    fprintf(stderr, "delete message 1 ok\n");

    delete m2;
    fprintf(stderr, "delete message 2 ok\n");
}

void test2(){
    Message *m1 = new Message("msg first");
    fprintf(stderr, "new message 1 ok\n");

    Message *m2 = new Message("msg second");
    fprintf(stderr, "new message 2 ok\n");
    
    Message *m3 = new Message("msg third");
    fprintf(stderr, "new message 3 ok\n");

    delete m1;
    fprintf(stderr, "delete message 1 ok\n");

    delete m2;
    fprintf(stderr, "delete message 2 ok\n");

    delete m3;
    fprintf(stderr, "delete message 3 ok\n");
}

int main(){
    fprintf(stderr, "---before init object pool\n");
    InitObjPool();
    fprintf(stderr, "---after init object pool\n");

    //test1();

    test2();

    return 0;
}

#include "message.h"


const int DEFAULT_POOL_SIZE = 4; 
template<> StaticPool<Node<Message*>*> DynamicPool<Message*>::avaliable(DEFAULT_POOL_SIZE+1);

DynamicPool<Message*> Message::pool_(DEFAULT_POOL_SIZE);

void init(){
    Message *objects = ::new Message[DEFAULT_POOL_SIZE];
    for(int i=0; i<DEFAULT_POOL_SIZE; ++i){
        Message::pool_.push(&objects[i]);
    }
}

int main(){
    init();
    Message *m1 = new Message("msg first");
    Message *m2 = new Message("msg second");
    delete m1;
    delete m2;
    return 0;
}

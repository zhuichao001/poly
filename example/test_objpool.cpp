#include "message.h"


template<>
StaticPool<Node<Message* >* > DynamicPool<Message* >::avaliable(1024);
DynamicPool<Message*> Message::pool_;

void init(){
    Node<Message*> *nodes = new Node<Message*>[1024];
    Message *messages = new Message[1024];
    for(int i=0; i<1024; ++i){
        DynamicPool<Message*>::avaliable.push(&nodes[i]);
        Message::pool_.push(&messages[i]);
    }
}

int main() { 
    init();
    fprintf(stderr, "=============\n");
    
    Message* m1=new Message("message first");
    Message* m2=new Message("message second");
    delete m1;
    delete m2;
    return 0;
}

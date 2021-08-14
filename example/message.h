#ifndef POLYMESSAGEH
#define POLYMESSAGEH

#include <stdlib.h>
#include <iostream>
#include "base/static_pool.h"
#include "base/dynamic_pool.h"


class Message {
    std::string val;
public:
    static DynamicPool<Message*> pool;

    Message(){
        std::cout<<"Message() constructed"<<std::endl;
    }

    Message(const std::string v):
        val(v){
        std::cout<<"Message("<<val<<") constructed"<<std::endl;
    }

    ~Message() {
        std::cout<<"~Message("<<val<<") destroyed"<<std::endl;
    }

    void* operator new(size_t size) {
        fprintf(stderr, "new called\n");
        void *p = Message::pool.pop();
        if(p==nullptr){
            fprintf(stderr, "new failed\n");
        } else {
            fprintf(stderr, "new ok\n");
        }
        return p;
    }

    void operator delete(void* p) {
        fprintf(stderr, "delete called\n");
        Message::pool.push(static_cast<Message*>(p));
        fprintf(stderr, "delete ok\n");
    }
};

#endif

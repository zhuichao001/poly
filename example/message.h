#ifndef _POLY_MESSAGE_H
#define _POLY_MESSAGE_H

#include <stdlib.h>
#include <iostream>
#include "base/static_pool.h"
#include "base/dynamic_pool.h"


class Message {
    std::string val_;
public:
    static DynamicPool<Message*> pool_;

    Message(){
        std::cout<<"Message() constructed"<<std::endl;
    }

    Message(const std::string v):
        val_(v){
        std::cout<<"Message("<<val_<<") constructed"<<std::endl;
    }

    ~Message() {
        std::cout<<"~Message("<<val_<<") destroyed"<<std::endl;
    }

    void* operator new(size_t size) {
        fprintf(stderr, "new called\n");
        return Message::pool_.pop();
        fprintf(stderr, "new ok\n");
    }

    void operator delete(void* p) {
        fprintf(stderr, "delete called\n");
        Message::pool_.push(static_cast<Message*>(p));
        fprintf(stderr, "delete ok\n");
    }
};

#endif

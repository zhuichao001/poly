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
        return Message::pool_.pop();
    }

    void operator delete(void* p) {
        Message::pool_.push(static_cast<Message*>(p));
    }
};

#endif

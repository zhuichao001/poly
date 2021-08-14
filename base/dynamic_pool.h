#ifndef _POLY_DANYMIIC_POOL_H
#define _POLY_DANYMIIC_POOL_H

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

template<typename T>
class Node {
public:
    Node * next;
    T val;
};


template<typename T>
class DynamicPool {
    Node<T> head;
    Node<T> *tail;
public:
    static StaticPool<Node<T>*> avaliable;

    DynamicPool() {
        tail = &head;
        tail->next = nullptr;
    }

    ~DynamicPool(){
        Node<T> *cur = head.next;
        while(cur){
            Node<T> *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    bool empty(){
        return head->next == nullptr;
    }

    void push(const T &t) {
        Node<T> * e ;
        DynamicPool<T>::avaliable.pop(&e);
        e->next = nullptr;
        e->val = t;

        Node<T> *last = nullptr;
        while(true){
            last = this->tail;
            //if tailor is moved, try again
            if(last!=this->tail){
                continue;
            }

            Node<T> *back = last->next;
            //if tailor's next is not null, set tailor to back
            if(back!=nullptr){
                __sync_bool_compare_and_swap((uint64_t**)(&tail), (uint64_t*)last, (uint64_t*)back);
                continue;
            }

            if(__sync_bool_compare_and_swap((uint64_t**)(&tail->next), (uint64_t*)back, (uint64_t*)e)){
                break;
            }
        }
        __sync_bool_compare_and_swap((uint64_t**)(&tail), (uint64_t*)last, (uint64_t*)e);
    }

    T pop() {
        Node<T> *first = nullptr;
        do{
            first = &head;
            if (first->next == nullptr){
                return nullptr;
            }
        } while( !__sync_bool_compare_and_swap((uint64_t**)(&head), (uint64_t*)first, (uint64_t*)(first->next)) );
        T ret= first->next->val;
        DynamicPool<T>::avaliable.push(first);
        return ret;
    }
};


#endif

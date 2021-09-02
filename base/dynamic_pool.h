#ifndef _POLY_DANYMIIC_POOL_H
#define _POLY_DANYMIIC_POOL_H

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "static_pool.h"

template<typename T>
class Node {
public:
    Node * next;
    T val;
};


template<typename T>
class DynamicPool {
    Node<T> *head, _;
    Node<T> *tail;
    Node<T> *nodes;
public:
    static StaticPool<Node<T>*> avaliable;

    DynamicPool() {
        const int n = DynamicPool<T>::avaliable.capacity();
        nodes = ::new Node<T>[n];
        for(int i=0; i<n; ++i){
            DynamicPool<T>::avaliable.push(&nodes[i]);
        }

        tail = head = &_;
        head->next = nullptr;
    }

    ~DynamicPool(){
        delete []nodes;
    }

    bool empty(){
        return head->next == nullptr;
    }

    void print(){
        Node<T> *cur = head->next;
        while(cur!=nullptr){
            cur = cur->next;
        }
    }

    bool push(const T &t) {
        Node<T> * e =nullptr;
        bool ok = DynamicPool<T>::avaliable.pop(&e);
        if(!ok){
            return false;
        }

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
        return true;
    }

    bool pop(T *t) {
        Node<T> *first = nullptr;
        do{
            first = head->next;
            if (first == nullptr){
                return false;
            }
        } while( !__sync_bool_compare_and_swap((uint64_t**)(&head->next), (uint64_t*)first, (uint64_t*)(first->next)) );

        first->next = nullptr;
        if(first==tail){
            tail = head;
        }

        *t = first->val;

        //for reuse Node<T>
        DynamicPool<T>::avaliable.push(first);

        return true;
    }
};


#endif

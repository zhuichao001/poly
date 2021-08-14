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
    Node<T> *head;
    Node<T> *tail;
public:
    static StaticPool<Node<T>*> avaliable;

    DynamicPool(int n) {

        Node<T> *nodes = ::new Node<T>[n+1];
        for(int i=0; i<n+1; ++i){
            DynamicPool<T>::avaliable.push(&nodes[i]);
        }

        DynamicPool<T>::avaliable.print();

        DynamicPool<T>::avaliable.pop(&head);
        fprintf(stderr, "prepare pop node, head:%d\n", head);

        DynamicPool<T>::avaliable.print();

        tail = head;
        head->next = nullptr;
    }

    ~DynamicPool(){
    }

    bool empty(){
        return head->next == nullptr;
    }

    void print(){
        Node<T> *cur = head;
        while(cur!=nullptr){
            fprintf(stderr, "%d->", cur);
            sleep(1);
            cur = cur->next;
        }
        fprintf(stderr, " |||  tail:%d\n", tail);
    }

    void push(const T &t) {
        Node<T> * e ;
        DynamicPool<T>::avaliable.pop(&e);
        fprintf(stderr, "push T, pop node:%d\n", e);
        DynamicPool<T>::avaliable.print();

        e->next = nullptr;
        e->val = t;

        Node<T> *last = nullptr;
        while(true){
            last = this->tail;
            //if tailor is moved, try again
            if(last!=this->tail){
                fprintf(stderr, "continue as last!=this->tail\n");
                sleep(1);
                continue;
            }

            Node<T> *back = last->next;
            //if tailor's next is not null, set tailor to back
            if(back!=nullptr){
                __sync_bool_compare_and_swap((uint64_t**)(&tail), (uint64_t*)last, (uint64_t*)back);
                fprintf(stderr, "continue as __sync_bool_compare_and_swap\n");
                sleep(1);
                continue;
            }

            if(__sync_bool_compare_and_swap((uint64_t**)(&tail->next), (uint64_t*)back, (uint64_t*)e)){
                break;
            }
        }
        __sync_bool_compare_and_swap((uint64_t**)(&tail), (uint64_t*)last, (uint64_t*)e);

        //print();
    }

    T pop() {
        Node<T> *first = nullptr;
        do{
            first = head;
            if (first->next == nullptr){
                return nullptr;
            }
        } while( !__sync_bool_compare_and_swap((uint64_t**)(&head), (uint64_t*)first, (uint64_t*)(first->next)) );
        T ret= first->next->val;
        first->next = nullptr;
        DynamicPool<T>::avaliable.push(first);
        fprintf(stderr, "pop T, push node\n");
        DynamicPool<T>::avaliable.print();
        return ret;
    }
};


#endif

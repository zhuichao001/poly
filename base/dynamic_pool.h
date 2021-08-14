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
    Node<T> *head, guard;
    Node<T> *tail;
public:
    static StaticPool<Node<T>*> avaliable;

    DynamicPool() {
        const int n = DynamicPool<T>::avaliable.capacity();
        Node<T> *nodes = ::new Node<T>[n];
        for(int i=0; i<n; ++i){
            DynamicPool<T>::avaliable.push(&nodes[i]);
            fprintf(stderr, "%d ", &nodes[i]);
            fprintf(stderr, "\n");
        }

        DynamicPool<T>::avaliable.print();

        //DynamicPool<T>::avaliable.pop(&head);
        //fprintf(stderr, "prepare pop node, head:%d\n", head);

        //DynamicPool<T>::avaliable.print();

        tail = head = &guard;
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

    bool push(const T &t) {
        Node<T> * e =nullptr;
        bool ok = DynamicPool<T>::avaliable.pop(&e);
        fprintf(stderr, "b:%d push T, pop node:%d\n", ok, e);
        DynamicPool<T>::avaliable.print();
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
        return true;
    }

    T pop() {
        Node<T> *first = nullptr;
        do{
            first = head->next;
            if (first == nullptr){
                return nullptr;
            }
        } while( !__sync_bool_compare_and_swap((uint64_t**)(&head->next), (uint64_t*)first, (uint64_t*)(first->next)) );
        T ret= first->val;
        first->next = nullptr;
        if(tail==first){
            tail = first;
        }
        DynamicPool<T>::avaliable.push(first);
        fprintf(stderr, "pop T, push node:%d\n", first);
        DynamicPool<T>::avaliable.print();
        return ret;
    }
};


#endif

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

template<class T>
class node {
    public:
        node * next;
        T *val;
};


template<class T>
class queue {
    node<T> *head;
    node<T> *tail;
public:

    queue() {
        head = tail = new node<T>();
        head->next = nullptr;
    }

    bool empty(){
        return head->next == nullptr;
    }

    void push(T *t) {
        node<T> * e = new node<T>();
        e->next = nullptr;
        e->val = t;

        node<T> *last = nullptr;
        while(true){
            last = this->tail;
            //if tailor is moved, try again
            if(last!=this->tail){
                continue;
            }

            node<T> *back = last->next;
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

    T *pop() {
        node<T> *first = nullptr;
        do{
            first = head;
            if (first->next == nullptr){
                return nullptr;
            }
        } while( !__sync_bool_compare_and_swap((uint64_t**)(&head), (uint64_t*)first, (uint64_t*)(first->next)) );
        T *ret= first->next->val;
        delete first;
        return ret;
    }
};

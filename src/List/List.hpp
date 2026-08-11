#ifndef LIST_HPP
#define LIST_HPP

#include <utility>
#include <stdexcept>
namespace mystd {

template <typename T>
class list{
public:
    class iterator;
    class const_iterator;
    class r_iterator;
    class const_r_iterator;

private:
    struct Node{

        T data;
        Node* next = nullptr;
        Node* prev = nullptr;

        Node() = default;
        Node(const T& data): data(data) {}
        Node(const T& data, Node* next): data(data), next(next) {}
        Node(const T& data, Node* prev, Node* next): data(data), next(next), prev(prev) {}

        Node(T&& data): data(std::forward<T>(data)) {}
        Node(T&& data, Node* next) : data(std::forward<T>(data)), next(next) {}
        Node(T&& data, Node* prev, Node* next): data(std::forward<T>(data)), next(next), prev(prev) {}


    };

    Node* sentinel;

public:


    list(){
        sentinel = new Node();
        sentinel->next = sentinel->prev = sentinel;
    }

    ~list(){
        free();
    }

    list& operator=(const list& other){
        if(this != &other){
            free();
            copyFrom(other);
        }
        return *this;
    }

    list(const list& other){
        copyFrom(other);
    }

    T& front(){
        return sentinel->next->data;
    }
    const T& front() const{
        return sentinel->next->data;
    }
    T& back(){
        return sentinel->prev->data;
    }
    const T& back() const{
        return sentinel->prev->data;
    }

    void push_back(const T& data){
        sentinel->prev = new Node(data, sentinel->prev, sentinel);
        if(sentinel->prev->prev == sentinel)
            sentinel->next = sentinel->prev;
    }
    void push_front(const T& data){
        sentinel->next = new Node(data, sentinel, sentinel->next);
        if(sentinel->next->next == sentinel)
            sentinel->prev = sentinel->next;
    }

    void push_back(T&& data){
        sentinel->prev = new Node(std::move(data), sentinel->prev, sentinel);
        if(sentinel->prev->prev == sentinel)
            sentinel->next = sentinel->prev;
    }
    void push_front(T&& data){
        sentinel->next = new Node(std::move(data), sentinel, sentinel->next);
        if(sentinel->next->next == sentinel)
            sentinel->prev = sentinel->next;
    }

    void pop_back(){
        if(sentinel->prev == sentinel)
            throw std::out_of_range("List is empty");

        sentinel->prev = sentinel->prev->prev;
        delete sentinel->prev->next;
        sentinel->prev->next = sentinel;
    }
    void pop_front(){
        if(sentinel->next == sentinel)
            throw std::out_of_range("List is empty");

        sentinel->next = sentinel->next->next;
        delete sentinel->next->prev;
        sentinel->next->prev = sentinel;
    }

    void insert(list::iterator& it, const T& data){

    }
    void insert(list::iterator& it, T&& data){

    }
    iterator erase(list::iterator& it){
        if (it == end())
            throw std::out_of_range("End iterator cannot be erased");

        Node* toErase = it.ptr;
        toErase->prev->next = toErase->next;
        toErase->next->prev = toErase->prev;
        iterator nextIter = iterator(toErase->next);
        delete toErase;
        return nextIter;
    }

    void clear() {
        free();
    }
    bool contains(const T& data) const{
        Node* iter = sentinel->next;
        while(iter != sentinel){
            if(iter->data == data)
                return true;
            iter = iter->next;
        }
        return false;
    }

    list::iterator end(){
        return list::iterator(sentinel);
    }
    list::iterator begin(){
        return list::iterator(sentinel->next);
    }
    list::const_iterator cend() const{
        return list::const_iterator(sentinel);
    }
    list::const_iterator cbegin() const{
        return list::const_iterator(sentinel->next);
    }
    list::r_iterator rend(){
        return list::r_iterator(sentinel);
    }
    list::r_iterator rbegin(){
        return list::r_iterator(sentinel->prev);
    }
    list::const_r_iterator crend() const{
        return list::const_r_iterator(sentinel);
    }
    list::const_r_iterator crbegin() const{
        return list::const_r_iterator(sentinel->prev);
    }


private:

    void free(){
        Node* iter = sentinel->next;
        while(iter != sentinel){
            iter = iter->next;
            delete iter->prev;
        }
        sentinel->next = sentinel->prev = sentinel;
    }
    void copyFrom(const list& other){
        Node* otherIter = other.sentinel->next;
        Node* thisIter = sentinel;

        while(otherIter != other.sentinel){
            thisIter->next = new Node(otherIter->data, thisIter, sentinel);
            thisIter = thisIter->next;
            otherIter = otherIter->next;
        }

        sentinel->prev = thisIter;
    }
    void move(list&& other){
        sentinel = other.sentinel;
        other.sentinel->next = other.sentinel->prev = other.sentinel;
    }

public:

    class iterator{
    protected:
        Node* ptr = nullptr;
    public:

        iterator() = default;
        iterator(Node* ptr){
            this->ptr = ptr;
        }

        T& operator*(){
            return ptr->data;
        }
        T* operator->(){
            return ptr->data;
        }
        const T& operator*() const{
            return &ptr->data;
        }
        const T* operator->() const{
            return &ptr->data;
        }

        virtual iterator& operator++(){
            ptr = ptr->next;
            return *this;
        }
        iterator operator++(int a){
            iterator old = *this;
            ptr = ptr->next;
            return old;
        }
        virtual iterator& operator--(){
            ptr = ptr->prev;
            return *this;
        }
        iterator operator--(int a){
            iterator old = *this;
            ptr = ptr->prev;
            return old;
        }
        virtual bool operator==(const iterator& other){
            return ptr == other.ptr;
        }
        virtual bool operator==(const list::const_iterator& other);

        virtual bool operator!=(const iterator& other){
            return !operator==(other);
        }
        virtual bool operator!=(const list::const_iterator& other);

    };

    class const_iterator : public list::iterator{
    public:
        const_iterator() = default;
        const_iterator(Node* ptr): iterator(ptr) {}

        Node& operator*() = delete;
        Node* operator->() = delete;
    };

    class r_iterator: public list::iterator{
    public:

        r_iterator() = default;
        r_iterator(Node* ptr): iterator(ptr){}

        r_iterator& operator++(){
            this->ptr = this->ptr->prev;
            return *this;
        }
        r_iterator operator++(int a){
            r_iterator old = *this;
            this->ptr = this->ptr->prev;
            return old;
        }
        r_iterator& operator--(){
            this->ptr = this->ptr->next;
            return *this;
        }
        r_iterator operator--(int a){
            r_iterator old = *this;
            this->ptr = this->ptr->next;
            return old;
        }

    };

    class const_r_iterator: public list::r_iterator{
    public:
        const_r_iterator() = default;
        const_r_iterator(Node* ptr): r_iterator(ptr) {}

        Node& operator*() = delete;
        Node* operator->() = delete;
    };

};

template <typename T>
bool list<T>::iterator::operator!=(const list<T>::const_iterator& other){
    return ptr != other.ptr;
}

}
#endif

#ifndef MYSTD_SMARTPOINTER_SHAREDPTR_HPP
#define MYSTD_SMARTPOINTER_SHAREDPTR_HPP
#include <iostream>

namespace mystd {

template <typename T>
class shared_ptr{
public:

    struct counter{
        size_t _sharedCount = 0;
        size_t _weakCount = 0;

        void add_shared();
        void remove_shared();
        void add_weak();
        void remove_weak();

    };

private:
    T* _data = nullptr;
    counter* _counter = nullptr;

    template <typename U> friend class weak_ptr;

public:
    shared_ptr() = default;
    shared_ptr(T* data);
    shared_ptr(const shared_ptr<T>& other);
    shared_ptr(shared_ptr<T>&& other);
    shared_ptr<T>& operator=(const shared_ptr<T>& other);
    shared_ptr<T>& operator=(shared_ptr<T>&& other);
    ~shared_ptr();

    const T& operator*() const;
    T& operator*();

    const T* operator->() const;
    T* operator->();

    bool operator==(const shared_ptr<T>& other) const;
    bool operator==(const T* ptr) const;
    operator bool() const;
    T* get();


private:
    void free();
    void copy_from(const shared_ptr<T>& other);
    void move(shared_ptr&& other);

};

template <typename T>
void shared_ptr<T>::counter::add_shared(){
    _sharedCount++;
    if(_weakCount == 0)
        _weakCount++;
}
template <typename T>
void shared_ptr<T>::counter::remove_shared(){
    _sharedCount--;
    if(_sharedCount == 0)
        _weakCount--;
}
template <typename T>
void shared_ptr<T>::counter::add_weak(){
    _weakCount++;
}
template <typename T>
void shared_ptr<T>::counter::remove_weak(){
    _weakCount--;
}


template <typename T>
shared_ptr<T>::shared_ptr(T* data){
    _data = data;
    if(_data){
        _counter = new counter;
        _counter->add_shared();
    }

}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other){
    copy_from(other);
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other){
    move(std::move(other));
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& other){
    if(this != &other){
        free();
        copy_from(other);
    }
    return *this;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& other){
    if(this != &other){
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
shared_ptr<T>::~shared_ptr(){
    free();
}


template <typename T>
const T& shared_ptr<T>::operator*() const{
    return *_data;
}
template <typename T>
T& shared_ptr<T>::operator*(){
    return *_data;
}

template <typename T>
const T* shared_ptr<T>::operator->() const{
    return _data;
}
template <typename T>
T* shared_ptr<T>::operator->(){
    return _data;
}

template <typename T>
bool shared_ptr<T>::operator==(const shared_ptr<T>& other) const{
    return _data == other._data;
}

template <typename T>
bool shared_ptr<T>::operator==(const T* ptr) const{
    return _data == ptr;
}

template <typename T>
shared_ptr<T>::operator bool() const{
    return _data;
}


template <typename T>
T* shared_ptr<T>::get(){
    return _data;
}

template <typename T>
void shared_ptr<T>::free(){
    if(_counter){
        _counter->remove_shared();
        if(_counter->_sharedCount == 0)
            delete _data;
        if(_counter->_weakCount == 0)
            delete _counter;
    }

}

template <typename T>
void shared_ptr<T>::copy_from(const shared_ptr<T>& other){
    _data = other._data;
    _counter = other._counter;

    if(_counter){
        _counter->add_shared();
    }

}

template <typename T>
void shared_ptr<T>::move(shared_ptr&& other){
    _data = other._data;
    other._data = nullptr;

    _counter = other._counter;
    other._counter = nullptr;
}

} // namespace mystd

#endif // MYSTD_SMARTPOINTER_SHAREDPTR_HPP

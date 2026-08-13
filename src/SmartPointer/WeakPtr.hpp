#ifndef MYSTD_SMARTPOINTER_WEAKPTR_HPP
#define MYSTD_SMARTPOINTER_WEAKPTR_HPP
#include "SharedPtr.hpp"

namespace mystd {

template <typename T>
class weak_ptr{
private:
    const T* _data = nullptr;
    shared_ptr<T>::counter* _counter = nullptr;

public:

    weak_ptr() = default;
    weak_ptr(const weak_ptr<T>& other);
    weak_ptr(const shared_ptr<T>& other);
    weak_ptr<T>& operator=(const weak_ptr<T>& other);
    weak_ptr<T>& operator=(const shared_ptr<T>& other);

    ~weak_ptr();

    const T& operator*() const;
    const T* operator->() const;

    bool operator==(const weak_ptr<T>& other) const;
    bool operator==(const T* ptr) const;
    operator bool() const;
    T* get();


private:
    void free();
    void copy_from(const shared_ptr<T>& other);
    void copy_from(const weak_ptr<T>& other);

};




template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr<T>& other){

    copy_from(other);
}

template <typename T>
weak_ptr<T>::weak_ptr(const shared_ptr<T>& other){
    copy_from(other);
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<T>& other){

    if(this != &other){
        free();
        copy_from(other);
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<T>& other){

    if(this != &other){
        free();
        copy_from(other);
    }
    return *this;
}

template <typename T>
weak_ptr<T>::~weak_ptr(){
    free();
}


template <typename T>
const T& weak_ptr<T>::operator*() const{
    return *_data;
}
template <typename T>
const T* weak_ptr<T>::operator->() const{
    return _data;
}
template <typename T>
T* weak_ptr<T>::get(){
    return _data;
}

template <typename T>
bool weak_ptr<T>::operator==(const weak_ptr<T>& other) const{
    return _data == other._data;
}
template <typename T>
bool weak_ptr<T>::operator==(const T* ptr) const{
    return _data == ptr;
}
template <typename T>
weak_ptr<T>::operator bool() const{
    return (_counter && _counter->_sharedCount != 0);
}

template <typename T>
void weak_ptr<T>::free(){
    if(_counter){
        _counter->remove_weak();
        if(_counter->_weakCount == 0)
            delete _counter;
    }
}
template <typename T>
void weak_ptr<T>::copy_from(const shared_ptr<T>& other){
    _data = other._data;
    _counter = other._counter;
    if(_counter)
        _counter->add_weak();

}

template <typename T>
void weak_ptr<T>::copy_from(const weak_ptr<T>& other){
    _data = other._data;
    _counter = other._counter;
    if(_counter)
        _counter->add_weak();

}

} // namespace mystd

#endif // MYSTD_SMARTPOINTER_WEAKPTR_HPP

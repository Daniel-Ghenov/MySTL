#ifndef MYSTD_ITERATOR_FORWARDITER_HPP
#define MYSTD_ITERATOR_FORWARDITER_HPP

namespace mystd {

template <typename T>
struct forward_iterator{
protected:
    T* _ptr = nullptr;
public:
    forward_iterator() = default;
    forward_iterator(T* ptr);

    virtual forward_iterator& operator++();
    forward_iterator operator++(int a);


    virtual bool operator<(const forward_iterator& other);
    virtual bool operator==(const forward_iterator& other);
    virtual bool operator!=(const forward_iterator& other);

    virtual const T& operator*() const;
    virtual T& operator*();
    virtual T* operator->();
};

template <typename T>
struct const_forward_iterator{
protected:
    const T* _ptr = nullptr;
public:
    const_forward_iterator() = default;
    const_forward_iterator(const T* ptr);

    virtual const_forward_iterator& operator++();
    const_forward_iterator operator++(int a);

    virtual bool operator<(const const_forward_iterator& other);
    virtual bool operator==(const const_forward_iterator& other);
    virtual bool operator!=(const const_forward_iterator& other);

    virtual const T& operator*() const;
    virtual const T* operator->();
};

template <typename T>
forward_iterator<T>::forward_iterator(T* ptr): _ptr(ptr){ }

template <typename T>
forward_iterator<T>& forward_iterator<T>::operator++(){
    _ptr++;
    return *this;
}
template <typename T>
forward_iterator<T> forward_iterator<T>::operator++(int a){
    _ptr++;
    return forward_iterator(_ptr - 1);
}


template <typename T>
bool forward_iterator<T>::operator<(const forward_iterator& other){
    return _ptr < other._ptr;
}
template <typename T>
bool forward_iterator<T>::operator==(const forward_iterator& other){
    return _ptr == other._ptr;
}
template <typename T>
bool forward_iterator<T>::operator!=(const forward_iterator& other){
    return _ptr != other._ptr;
}

template <typename T>
const T& forward_iterator<T>::operator*() const{
    return *_ptr;
}
template <typename T>
T& forward_iterator<T>::operator*(){
    return *_ptr;
}
template <typename T>
T* forward_iterator<T>::operator->(){
    return _ptr;
}

//Constant Forward Iterator


template <typename T>
const_forward_iterator<T>::const_forward_iterator(const T* ptr): _ptr(ptr){ }

template <typename T>
const_forward_iterator<T>& const_forward_iterator<T>::operator++(){
    _ptr++;
    return *this;
}
template <typename T>
const_forward_iterator<T> const_forward_iterator<T>::operator++(int a){
    _ptr++;
    return const_forward_iterator(_ptr - 1);
}

template <typename T>
bool const_forward_iterator<T>::operator<(const const_forward_iterator& other){
    return _ptr < other._ptr;
}
template <typename T>
bool const_forward_iterator<T>::operator==(const const_forward_iterator& other){
    return _ptr == other._ptr;
}
template <typename T>
bool const_forward_iterator<T>::operator!=(const const_forward_iterator& other){
    return _ptr != other._ptr;
}

template <typename T>
const T& const_forward_iterator<T>::operator*() const{
    return *_ptr;
}
template <typename T>
const T* const_forward_iterator<T>::operator->(){
    return _ptr;
}

} // namespace mystd

#endif // MYSTD_ITERATOR_FORWARDITER_HPP

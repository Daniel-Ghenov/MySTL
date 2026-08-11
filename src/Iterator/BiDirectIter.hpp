#ifndef MYSTD_ITERATOR_BIDIRECTITER_HPP
#define MYSTD_ITERATOR_BIDIRECTITER_HPP
#include "ForwardIter.hpp"

namespace mystd {

template <typename T>
struct bidirectional_iterator: public forward_iterator<T>{
public:
    bidirectional_iterator() = default;
    bidirectional_iterator(T* ptr);

    virtual bidirectional_iterator& operator--();
    bidirectional_iterator operator--(int a);
};

template <typename T>
struct const_bidirectional_iterator: public const_forward_iterator<T>{
public:
    const_bidirectional_iterator() = default;
    const_bidirectional_iterator(const T* ptr);

    virtual const_bidirectional_iterator& operator--();
    const_bidirectional_iterator operator--(int a);
};

template <typename T>
bidirectional_iterator<T>::bidirectional_iterator(T* ptr): forward_iterator<T>(ptr){}

template <typename T>
bidirectional_iterator<T>& bidirectional_iterator<T>::operator--(){
    this->_ptr--;
    return *this;
}
template <typename T>
bidirectional_iterator<T> bidirectional_iterator<T>::operator--(int a){
    this->_ptr--;
    return bidirectional_iterator(this->_ptr + 1);
}

template <typename T>
const_bidirectional_iterator<T>::const_bidirectional_iterator(const T* ptr): const_forward_iterator<T>(ptr){}

template <typename T>
const_bidirectional_iterator<T>& const_bidirectional_iterator<T>::operator--(){
    this->_ptr--;
    return *this;
}
template <typename T>
const_bidirectional_iterator<T> const_bidirectional_iterator<T>::operator--(int a){
    this->_ptr--;
    return const_bidirectional_iterator(this->_ptr + 1);
}

} // namespace mystd

#endif // MYSTD_ITERATOR_BIDIRECTITER_HPP

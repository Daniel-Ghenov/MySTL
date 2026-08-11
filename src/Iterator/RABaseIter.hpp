#ifndef MYSTD_ITERATOR_RABASEITER_HPP
#define MYSTD_ITERATOR_RABASEITER_HPP
#include "BiDirectIter.hpp"

namespace mystd {

template <typename T>
struct random_access_iterator: public bidirectional_iterator<T>{
public:
    random_access_iterator() = default;
    random_access_iterator(T* ptr);

    virtual random_access_iterator& operator+=(int offset);
    virtual random_access_iterator& operator-=(int offset);

    random_access_iterator operator+(int offset);
    random_access_iterator operator-(int offset);

};

template <typename T>
struct const_random_access_iterator: public const_bidirectional_iterator<T>{
public:
    const_random_access_iterator() = default;
    const_random_access_iterator(const T* ptr);
    const_random_access_iterator(const random_access_iterator<T>& other);

    virtual const_random_access_iterator& operator+=(int offset);
    virtual const_random_access_iterator& operator-=(int offset);

    const_random_access_iterator operator+(int offset);
    const_random_access_iterator operator-(int offset);
};

template <typename T>
random_access_iterator<T>::random_access_iterator(T* ptr): bidirectional_iterator<T>(ptr){
}
template <typename T>
random_access_iterator<T>& random_access_iterator<T>::operator+=(int offset){
    this->_ptr += offset;
    return *this;
}
template <typename T>
random_access_iterator<T>& random_access_iterator<T>::operator-=(int offset){
    this->_ptr -= offset;
    return *this;
}
template <typename T>
random_access_iterator<T> random_access_iterator<T>::operator+(int offset){
    return random_access_iterator<T>(this->_ptr + offset);
}

template <typename T>
random_access_iterator<T> random_access_iterator<T>::operator-(int offset){
    return random_access_iterator<T>(this->_ptr - offset);
}


//Constant Iterator


template <typename T>
const_random_access_iterator<T>::const_random_access_iterator(const T* ptr): const_bidirectional_iterator<T>(ptr){

}
template <typename T>
const_random_access_iterator<T>::const_random_access_iterator(const random_access_iterator<T>& other): const_bidirectional_iterator<T>(other._ptr){

}

template <typename T>
const_random_access_iterator<T>& const_random_access_iterator<T>::operator+=(int offset){
    this->_ptr += offset;
    return *this;
}
template <typename T>
const_random_access_iterator<T>& const_random_access_iterator<T>::operator-=(int offset){
    this->_ptr -= offset;
    return *this;
}

template <typename T>
const_random_access_iterator<T> const_random_access_iterator<T>::operator+(int offset){
    return const_random_access_iterator<T>(this->_ptr + offset);
}
template <typename T>
const_random_access_iterator<T> const_random_access_iterator<T>::operator-(int offset){
    return const_random_access_iterator<T>(this->_ptr - offset);

}

} // namespace mystd

#endif // MYSTD_ITERATOR_RABASEITER_HPP

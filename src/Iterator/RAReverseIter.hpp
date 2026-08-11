#ifndef MYSTD_ITERATOR_RAREVERSEITER_HPP
#define MYSTD_ITERATOR_RAREVERSEITER_HPP
#include "RABaseIter.hpp"

namespace mystd {

template <typename T>
class reverse_iterator : public random_access_iterator<T>{
public:
    reverse_iterator() = default;
    reverse_iterator(T* ptr);
    reverse_iterator(const random_access_iterator<T>& other);

    reverse_iterator& operator++() override;
    reverse_iterator& operator--() override;
    reverse_iterator operator++(int a);
    reverse_iterator operator--(int a);

    reverse_iterator& operator+=(int offset) override;
    reverse_iterator& operator-=(int offset) override;

    reverse_iterator operator+(int offset);
    reverse_iterator operator-(int offset);

    bool operator>(const reverse_iterator& other);
    bool operator>=(const reverse_iterator& other);
    bool operator<=(const reverse_iterator& other);
    bool operator<(const reverse_iterator& other);
    bool operator==(const reverse_iterator& other);
    bool operator!=(const reverse_iterator& other);

    const T& operator*() const override;
    T& operator*() override;
    T* operator->() override;
};

template <typename T>
class const_reverse_iterator :public const_random_access_iterator<T>{
public:
    const_reverse_iterator() = default;
    const_reverse_iterator(const T* ptr);
    const_reverse_iterator(const const_random_access_iterator<T>& other);
    const_reverse_iterator(const reverse_iterator<T>& other);

    const_reverse_iterator<T>& operator++() override;
    const_reverse_iterator<T>& operator--() override;
    const_reverse_iterator<T> operator++(int a);
    const_reverse_iterator<T> operator--(int a);

    const_reverse_iterator<T>& operator+=(int offset) override;
    const_reverse_iterator<T>& operator-=(int offset) override;

    const_reverse_iterator<T> operator+(int offset);
    const_reverse_iterator<T> operator-(int offset);

    bool operator>(const const_reverse_iterator<T>& other);
    bool operator>=(const const_reverse_iterator<T>& other);
    bool operator<=(const const_reverse_iterator<T>& other);
    bool operator<(const const_reverse_iterator<T>& other);
    bool operator==(const const_reverse_iterator<T>& other);
    bool operator!=(const const_reverse_iterator<T>& other);

    const T& operator*() const override;
    const T* operator->() override;
};

template <typename T>
reverse_iterator<T>::reverse_iterator(T* ptr): random_access_iterator<T>(ptr){
}
template <typename T>
reverse_iterator<T>::reverse_iterator(const random_access_iterator<T>& other): random_access_iterator<T>(other._ptr - 1){

}

template <typename T>
reverse_iterator<T>& reverse_iterator<T>::operator++(){
    this->_ptr--;
    return *this;
}
template <typename T>
reverse_iterator<T>& reverse_iterator<T>::operator--(){
    this->_ptr++;
    return *this;
}
template <typename T>
reverse_iterator<T> reverse_iterator<T>::operator++(int a){
    this->_ptr--;
    return (reverse_iterator<T>((this->_ptr)  + 1));
}
template <typename T>
reverse_iterator<T> reverse_iterator<T>::operator--(int a){
    this->_ptr++;
    return (reverse_iterator<T>((this->_ptr) - 1));
}
template <typename T>
reverse_iterator<T>& reverse_iterator<T>::operator+=(int offset){
    this->_ptr -= offset;
    return *this;
}
template <typename T>
reverse_iterator<T>& reverse_iterator<T>::operator-=(int offset){
    this->_ptr += offset;
    return *this;
}
template <typename T>
reverse_iterator<T> reverse_iterator<T>::operator+(int offset){
    return reverse_iterator<T>(this->_ptr - offset);

}
template <typename T>
reverse_iterator<T> reverse_iterator<T>::operator-(int offset){
    return reverse_iterator<T>(this->_ptr + offset);
}
template <typename T>
bool reverse_iterator<T>::operator>(const reverse_iterator<T>& other){
    return this->_ptr < other._ptr;
}
template <typename T>
bool reverse_iterator<T>::operator>=(const reverse_iterator<T>& other){
    return this->_ptr <= other._ptr;

}
template <typename T>
bool reverse_iterator<T>::operator<=(const reverse_iterator<T>& other){
    return this->_ptr >= other._ptr;
}
template <typename T>
bool reverse_iterator<T>::operator<(const reverse_iterator<T>& other){
    return this->_ptr > other._ptr;

}
template <typename T>
bool reverse_iterator<T>::operator==(const reverse_iterator<T>& other){
    return this->_ptr == other._ptr;
}
template <typename T>
bool reverse_iterator<T>::operator!=(const reverse_iterator<T>& other){
    return this->_ptr != other._ptr;
}

template <typename T>
const T& reverse_iterator<T>::operator*() const{
    return *(this->_ptr - 1);
}
template <typename T>
T& reverse_iterator<T>::operator*(){
    return *(this->_ptr - 1);
}
template <typename T>
T* reverse_iterator<T>::operator->(){
    return (this->_ptr - 1);
}
//Const Iter


template <typename T>
const_reverse_iterator<T>::const_reverse_iterator(const T* ptr): const_random_access_iterator<T>(ptr){

}

template <typename T>
const_reverse_iterator<T>::const_reverse_iterator(const const_random_access_iterator<T>& other): const_random_access_iterator<T>(other._ptr - 1){

}
template <typename T>
const_reverse_iterator<T>::const_reverse_iterator(const reverse_iterator<T>& other): const_random_access_iterator<T>(other){

}

template <typename T>
const_reverse_iterator<T>& const_reverse_iterator<T>::operator++(){
    this->_ptr--;
    return *this;
}
template <typename T>
const_reverse_iterator<T>& const_reverse_iterator<T>::operator--(){
    this->_ptr++;
    return *this;
}
template <typename T>
const_reverse_iterator<T> const_reverse_iterator<T>::operator++(int a){
    this->_ptr--;
    return const_reverse_iterator(this->_ptr + 1);
}
template <typename T>
const_reverse_iterator<T> const_reverse_iterator<T>::operator--(int a){
    this->_ptr++;
    return const_reverse_iterator(this->_ptr - 1);
}
template <typename T>
const_reverse_iterator<T>& const_reverse_iterator<T>::operator+=(int offset){
    this->_ptr -= offset;
    return *this;
}
template <typename T>
const_reverse_iterator<T>& const_reverse_iterator<T>::operator-=(int offset){
    this->_ptr += offset;
    return *this;
}
template <typename T>
const_reverse_iterator<T> const_reverse_iterator<T>::operator+(int offset){
    return const_reverse_iterator(this->_ptr - offset);

}
template <typename T>
const_reverse_iterator<T> const_reverse_iterator<T>::operator-(int offset){
    return const_reverse_iterator(this->_ptr + offset);

}
template <typename T>
bool const_reverse_iterator<T>::operator>(const const_reverse_iterator<T>& other){
    return this->_ptr < other._ptr;

}
template <typename T>
bool const_reverse_iterator<T>::operator>=(const const_reverse_iterator<T>& other){
    return this->_ptr <= other._ptr;

}
template <typename T>
bool const_reverse_iterator<T>::operator<=(const const_reverse_iterator<T>& other){
    return this->_ptr >= other._ptr;

}
template <typename T>
bool const_reverse_iterator<T>::operator<(const const_reverse_iterator<T>& other){
    return this->_ptr > other._ptr;

}
template <typename T>
bool const_reverse_iterator<T>::operator==(const const_reverse_iterator<T>& other){
    return this->_ptr == other._ptr;

}
template <typename T>
bool const_reverse_iterator<T>::operator!=(const const_reverse_iterator<T>& other){
    return this->_ptr != other._ptr;

}

template <typename T>
const T& const_reverse_iterator<T>::operator*() const{
    return *(this->_ptr - 1);
}
template <typename T>
const T* const_reverse_iterator<T>::operator->(){
    return (this->_ptr - 1);
}

} // namespace mystd

#endif // MYSTD_ITERATOR_RAREVERSEITER_HPP

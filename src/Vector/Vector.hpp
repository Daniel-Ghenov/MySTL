#ifndef MYSTD_VECTOR_VECTOR_HPP
#define MYSTD_VECTOR_VECTOR_HPP

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "RAReverseIter.hpp"

namespace mystd {

static const int VECTOR_UPSIZE_BY = 2;
static const int VECTOR_DOWNSIZE_BY = 4;
static const int VECTOR_DEFAULT_SIZE = 8;


template <typename T>
class vector{
public:
    class iterator: public mystd::random_access_iterator<T>{
    public:
        using mystd::random_access_iterator<T>::random_access_iterator;
    };

    class const_iterator: public mystd::const_random_access_iterator<T>{
    public:
        using mystd::const_random_access_iterator<T>::const_random_access_iterator;
    };

    class reverse_iterator : public mystd::reverse_iterator<T>{
    public:
        using mystd::reverse_iterator<T>::reverse_iterator;
    };

    class const_reverse_iterator : public mystd::const_reverse_iterator<T>{
    public:
        using mystd::const_reverse_iterator<T>::const_reverse_iterator;
    };

private:

    void* _data;
    size_t _size;
    size_t _capacity;

public:

    vector();
    vector(size_t size, const T& fill);
    vector(size_t size);
    ~vector();
    vector<T>& operator=(const vector<T>& other);
    vector(const vector<T>& other);
    vector(vector<T>&& other) noexcept;
    vector<T>& operator=(vector<T>&& other) noexcept;

    vector(const std::initializer_list<T>& initList);


    bool contains(const T& data) const;
    size_t find(const T& data) const;
    T& operator[](size_t number);   //Data access
    const T& operator[](size_t number) const;
    const T& at(size_t i) const;
    T& at(size_t i);

    T* data();
    const T& front() const;
    const T& back() const;
    operator bool() const;
    bool empty() const;

    void resize(size_t size);   //Size Modifications
    void resize(size_t size, const T& fill);
    size_t size() const;
    size_t capacity() const;
    void reserve(size_t number);

    void push_back(const T& newData);   //Data modifications
    void push_back(T&& newData);
    void pop_back();
    void insert(size_t index, const T& toInsert);//iterator supp
    void insert(size_t index, T&& toInsert);//iterator supp

    void erase(size_t index);   //iterator supp
    void swap(size_t index1, size_t index2);    //iterator supp
    void clear() noexcept;

    iterator end();     //Iterators
    const_iterator cend() const;
    iterator begin();
    const_iterator cbegin() const;

    reverse_iterator rend();     //Reverse Iterators
    const_reverse_iterator crend() const;
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;

    static const size_t npos = -1;


private:
    void move(vector<T>&& other);
    void copyFrom(const vector<T>& other);
    void free()noexcept;


};

template <typename T>
const size_t vector<T>::npos;


//Big 4 and Constructors


template <typename T>
vector<T>::vector(){
    _data = nullptr;
    _size = 0;
    _capacity = VECTOR_DEFAULT_SIZE;
}

template <typename T>
vector<T>::vector(size_t _size): vector(_size, T()){}

template <typename T>
vector<T>::vector(size_t size, const T& fill){
    _size = size;
    _capacity = _size * VECTOR_UPSIZE_BY;
    _data = ::operator new(_capacity * sizeof(T));
    auto iter = static_cast<T*>(_data);
    for(size_t i {0}; i < size; i++){
        new (iter) T(fill);
        iter++;
    }

}
template <typename T>
vector<T>::~vector(){
    clear();
}

template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& other){
    if(this != &other){
        clear();
        copyFrom(other);
    }
    return *this;
}
template <typename T>
vector<T>::vector(const vector<T>& other){
    copyFrom(other);
}

template <typename T>
vector<T>::vector(vector<T>&& other)noexcept{
    move(std::move(other));

}
template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& other)noexcept{
    if(this != &other){
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
vector<T>::vector(const std::initializer_list<T>& initList){
    _size = initList.size();
    _capacity = _size * VECTOR_UPSIZE_BY;
    _data = ::operator new(_capacity * sizeof(T));
    for(size_t i {0}; i < _size; i++){

        new (static_cast<std::byte*>(_data) + (i * sizeof(T))) T(initList.begin()[i]);
    }
}

//data Access

template <typename T>
size_t vector<T>::find(const T& data) const{
    for(size_t i {0}; i < this->size(); i++){
        if(data == (*this)[i]){
            return i;
        }
    }
    return ::mystd::vector<T>::npos;
}
template <typename T>
bool vector<T>::contains(const T& data) const{
    for(size_t i {0}; i < this->size(); i++){
        if(data == (*this)[i]){
            return true;
        }
    }
    return false;
}

template <typename T>
T& vector<T>::operator[](size_t number) {
    return *(static_cast<T*>(_data) + number);
}
template <typename T>
const T& vector<T>::operator[](size_t number) const{
    return *(static_cast<T*>(_data) + number);
}
template <typename T>
T* vector<T>::data(){
    return static_cast<T*>(_data);
}

template <typename T>
const T& vector<T>::at(size_t i) const{
    if (i >= _size)
        throw std::out_of_range("Index out of vector range");

    return operator[](i);
}

template <typename T>
T& vector<T>::at(size_t i){
    if (i >= _size)
        throw std::out_of_range("Index out of vector range");

    return operator[](i);
}

template <typename T>
const T& vector<T>::front() const{
    return operator[](0);
}

template <typename T>
const T& vector<T>::back() const{
    return operator[](_size - 1);
}

template <typename T>
bool vector<T>::empty() const{
    return _size == 0;
}


//_size Modifications


template <typename T>
void vector<T>::resize(size_t _size){
    resize(_size, T());
}

template <typename T>
void vector<T>::resize(size_t size, const T& fill){


    _capacity = size * VECTOR_UPSIZE_BY;

    void* temp = ::operator new(_capacity * sizeof(T));

    for(size_t i {0}; i <((_size > size)? size : _size); i++){
        if(i > _size){
            new (static_cast<std::byte*>(temp) + (i * sizeof(T))) T(fill);
        }
        else
            new (static_cast<std::byte*>(temp) + (i * sizeof(T))) T(operator[](i));
    }

    _size = (_size > size)? size : _size;
    ::operator delete(_data);
    _data = temp;

}
template <typename T>
size_t vector<T>::size() const{
    return _size;
}
template <typename T>
size_t vector<T>::capacity() const{
    return _capacity;
}
template <typename T>
void vector<T>::reserve(size_t number){
    if(number > _size){
        resize(number);
    }
}

//_data Modificators


template <typename T>
void vector<T>::push_back(const T& new_data){

    if(_data == nullptr){
        _capacity = VECTOR_DEFAULT_SIZE;
        _data = ::operator new(VECTOR_DEFAULT_SIZE * sizeof(T));
    }

    if(_size == _capacity){
        _capacity *= 2;
        resize(_capacity);
    }
    
    new (static_cast<std::byte*>(_data) + (_size++ * sizeof(T))) T(new_data);
}

template <typename T>
void vector<T>::push_back(T&& new_data){

    if(_data == nullptr){
        _capacity = VECTOR_DEFAULT_SIZE;
        _data = ::operator new(VECTOR_DEFAULT_SIZE * sizeof(T));
    }

    if(_size == _capacity){
        _capacity *= 2;
        resize(_capacity);
    }

    new (static_cast<std::byte*>(_data) + (_size++ * sizeof(T))) T(std::move(new_data));
}

template <typename T>
void vector<T>::pop_back(){
    _size--;
    if(_size < _capacity / VECTOR_DOWNSIZE_BY){
        resize(_capacity / VECTOR_UPSIZE_BY);
    }
}

template <typename T>
void vector<T>::clear() noexcept{
    free();
}




template <typename T>
void vector<T>::insert(size_t index, const T& toInsert){

    index = std::min(index, _size);
    if(_size == _capacity){
        resize(_capacity * VECTOR_UPSIZE_BY);
    }

    for(size_t i = _size ; i > index; i--){
        std::swap(operator[](i), operator[](i - 1));
    }
    
    new (static_cast<std::byte*>(_data) + (index * sizeof(T))) T(toInsert);
}

template <typename T>
void vector<T>::insert(size_t index, T&& toInsert){

    index = std::min(index, _size);
    if(_size == _capacity){
        resize(_capacity * VECTOR_UPSIZE_BY);
    }

    for(size_t i = _size ; i > index; i--){
        std::swap(operator[](i), operator[](i - 1));
    }
    
    new (static_cast<std::byte*>(_data) + (index * sizeof(T))) T(std::move(toInsert));
}

template <typename T>
void vector<T>::erase(size_t index){

    for(size_t i = index; i < _size - 1; i++){

        std::swap(operator[](i), operator[](i + 1));
    }
    _size--;
}

template <typename T>
void vector<T>::swap(size_t index1, size_t index2){

    std::swap(operator[](index1), operator[](index2));
}

template<typename T>
vector<T>::operator bool() const{
    return(_data == nullptr || _size == 0);
}

template<typename T>
vector<T>::iterator vector<T>::end(){
    return vector<T>::iterator(static_cast<T*>(_data) + size());
}
template<typename T>
vector<T>::const_iterator vector<T>::cend() const{
    return const_iterator(static_cast<T*>(_data) + size());

}
template<typename T>
vector<T>::iterator vector<T>::begin(){
    return vector<T>::iterator(static_cast<T*>(_data));

}
template<typename T>
vector<T>::const_iterator vector<T>::cbegin() const{
    return const_iterator(static_cast<T*>(_data));

}

template<typename T>
vector<T>::reverse_iterator vector<T>::rend(){
    return vector<T>::reverse_iterator(static_cast<T*>(_data) - 1);
}
template<typename T>
vector<T>::const_reverse_iterator vector<T>::crend() const{
    return vector<T>::const_reverse_iterator(static_cast<T*>(_data) - 1);
}
template<typename T>
vector<T>::reverse_iterator vector<T>::rbegin(){
    return vector<T>::reverse_iterator(static_cast<T*>(_data) + _size - 1);
}
template<typename T>
vector<T>::const_reverse_iterator vector<T>::crbegin() const{
    return vector<T>::const_reverse_iterator(static_cast<T*>(_data) + _size - 1);
}



template <typename T>
void vector<T>::copyFrom(const vector<T>& other){
    _data = new T[other.capacity()];

    for(size_t i {0}; i < other._size; i++){

        new (static_cast<std::byte*>(_data) + (i * sizeof(T))) T(other.operator[](i));
    }
    _size = other._size;
    _capacity = other.capacity();
}

template <typename T>
void vector<T>::move(vector<T>&& other){
    this->_data = other._data;
    this->_size = other._size;
    this->_capacity = other._capacity;
    other._data = nullptr;
    other._capacity = other._size = 0;

}
template <typename T>
void vector<T>::free() noexcept{
    operator delete(_data);
    _data = nullptr;
    _size = _capacity = 0;
}

} // namespace mystd

#endif // MYSTD_VECTOR_VECTOR_HPP

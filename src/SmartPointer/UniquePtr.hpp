#ifndef MYSTD_SMARTPOINTER_UNIQUEPTR_HPP
#define MYSTD_SMARTPOINTER_UNIQUEPTR_HPP

#include <utility>

namespace mystd {

template <typename T>
class unique_ptr{
private:
    T* _data = nullptr;

public:



    unique_ptr() = default;
    unique_ptr(T* data);
    unique_ptr(const unique_ptr<T>& other) = delete;
    unique_ptr(unique_ptr<T>&& other);
    unique_ptr<T>& operator=(const unique_ptr<T>& other) = delete;
    unique_ptr<T>& operator=(unique_ptr<T>&& other);
    ~unique_ptr();

    const T& operator*() const;
    T& operator*();

    const T* operator->() const;
    T* operator->();


private:
    void free();
    void move(unique_ptr&& other);

    template <typename U, typename ...Args>
    friend unique_ptr<U> make_unique(Args&&... args);

};


template <typename T>
unique_ptr<T>::unique_ptr(T* data){
    _data = data;
}
template <typename T>
unique_ptr<T>::unique_ptr(unique_ptr<T>&& other){
    move(std::move(other));
}
template <typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T>&& other){
    if(this != &other){
        free();
        move(std::move(other));
    }
    return *this;
}

template <typename T>
unique_ptr<T>::~unique_ptr(){
    free();
}

template <typename T>
const T& unique_ptr<T>::operator*() const{
    return *_data;
}
template <typename T>
T& unique_ptr<T>::operator*(){
    return *_data;
}

template <typename T>
const T* unique_ptr<T>::operator->() const{
    return _data;
}
template <typename T>
T* unique_ptr<T>::operator->(){
    return _data;
}

template <typename T>
void unique_ptr<T>::free(){
    delete _data;
    _data = nullptr;
}
template <typename T>
void unique_ptr<T>::move(unique_ptr&& other){
    _data = other._data;
    other._data = nullptr;
}

template <typename U, typename ...Args>
unique_ptr<U> make_unique(Args&&... args) {
    return unique_ptr<U>(new U(std::forward<Args>(args)...));
}

} // namespace mystd

#endif // MYSTD_SMARTPOINTER_UNIQUEPTR_HPP

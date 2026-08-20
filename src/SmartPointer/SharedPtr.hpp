#ifndef MYSTD_SMARTPOINTER_SHAREDPTR_HPP
#define MYSTD_SMARTPOINTER_SHAREDPTR_HPP

#include <cstddef>
#include <utility>
namespace mystd {

template <typename T>
class shared_ptr;

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args);

template <typename T>
class shared_ptr{
public:

    class counter_base {
    public:
        size_t _sharedCount = 0;
        size_t _weakCount = 0;
        virtual ~counter_base() = default;

        virtual void add_shared() = 0;
        virtual void remove_shared() = 0;
        virtual void add_weak() = 0;
        virtual void remove_weak() = 0;

    };

    class counter_seperate : public counter_base {
    public:
        T* data;

    private:
        ~counter_seperate() override;

        void add_shared() override;
        void remove_shared() override;
        void add_weak() override;
        void remove_weak() override;

    };

    class counter_inline : public counter_base {
    public:
        T data;

        template<typename ...Args>
        counter_inline(Args&&... args): data(std::forward<Args>(args)...) {}

    private:
        ~counter_inline() override = default;

        void add_shared() override;
        void remove_shared() override;
        void add_weak() override;
        void remove_weak() override;
    };


private:
    T* _data = nullptr;
    counter_base* _counter = nullptr;

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

    template <typename U, typename ...Args>
    friend shared_ptr<U> make_shared(Args&&... args);
};

template <typename T>
void shared_ptr<T>::counter_seperate::add_shared() {
    this->_sharedCount++;
    if(this->_weakCount == 0)
        this->_weakCount++;
}
template <typename T>
void shared_ptr<T>::counter_seperate::remove_shared() {
    this->_sharedCount--;
    if(this->_sharedCount == 0) {
        delete data;
        this->_weakCount--;
    }
}
template <typename T>
void shared_ptr<T>::counter_seperate::add_weak() {
    this->_weakCount++;
}
template <typename T>
void shared_ptr<T>::counter_seperate::remove_weak() {
    this->_weakCount--;
}

template <typename T>
shared_ptr<T>::counter_seperate::~counter_seperate() {
    remove_shared();
}

template <typename T>
void shared_ptr<T>::counter_inline::add_shared() {
    this->_sharedCount++;
    if(this->_weakCount == 0)
        this->_weakCount++;
}
template <typename T>
void shared_ptr<T>::counter_inline::remove_shared() {
    this->_sharedCount--;
    if(this->_sharedCount == 0) {
        this->_weakCount--;
    }
}
template <typename T>
void shared_ptr<T>::counter_inline::add_weak() {
    this->_weakCount++;
}
template <typename T>
void shared_ptr<T>::counter_inline::remove_weak() {
    this->_weakCount--;
}


template <typename T>
shared_ptr<T>::shared_ptr(T* data){
    _data = data;
    if(_data){
        auto* sep = new counter_seperate;
        sep->data = data;
        _counter = sep;
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

template <typename T, typename ...Args>
shared_ptr<T> make_shared(Args&&... args) {
    shared_ptr<T> ptr;
    auto* block = new shared_ptr<T>::counter_inline(std::forward<Args>(args)...);
    ptr._counter = block;
    ptr._counter->add_shared();
    ptr._data = &block->data;
    return ptr;
}

} // namespace mystd

#endif // MYSTD_SMARTPOINTER_SHAREDPTR_HPP

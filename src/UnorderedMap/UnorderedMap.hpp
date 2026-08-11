#ifndef UNORDERED_MAP_HPP
#define UNORDERED_MAP_HPP

#include <functional>
#include "Vector.hpp"
#include "List.hpp"
#include "Pair.hpp"
#include "Algorithm.hpp"


namespace mystd {

template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>> 
class unordered_map {
private:
    using value_type = mystd::pair<Key, T>;
    mystd::vector<mystd::list<value_type>> buckets;
    Hash hash_f;
    KeyEqual equal_f;
    size_t _size;
public:
    class iterator {
    private:
        unordered_map* map;
        size_t bucket_ind = 0;
        mystd::list<value_type>::iterator bucket_it;

    public:
        
        iterator(unordered_map* map, 
                 size_t bucket_ind, 
                 mystd::list<value_type>::iterator bucket_it): map(map),
                                                                        bucket_ind(bucket_ind), 
                                                                        bucket_it(bucket_it) {}

        iterator& operator++(){
            if(bucket_it == map->buckets[bucket_ind].end()) {
                if(bucket_ind == map->_size)
                    return *this;
                ++bucket_ind;
                bucket_it = map->buckets[bucket_ind].begin();
                return *this;
            }
            ++bucket_it;
            return *this;
        }
        iterator operator++(int a){
            iterator old = *this;
            operator++();
            return old;
        }
        iterator& operator--(){
            if(bucket_it == map->buckets[bucket_ind].begin()) {
                if(bucket_ind == 0)
                    return *this;
                --bucket_ind;
                bucket_it = map->buckets[bucket_ind].end();
                return *this;
            }
            --bucket_it;
            return *this;
        }
        iterator operator--(int a){
            iterator old = *this;
            operator--();
            return old;
        }

        bool operator!=(const iterator& other){
            return !operator==(other);
        }

        bool operator==(const iterator& other){
            return map == other.map && bucket_ind == other.bucket_ind && bucket_it == other.bucket_it;
        }
        
        value_type& operator*(){
            return &(*bucket_it);
        }
        value_type* operator->(){
            return &(*bucket_it);
        }
        
    };

public:
    explicit unordered_map(size_t bucket_count,
                        const Hash& hash = Hash(),
                        const KeyEqual& equal = KeyEqual()): hash_f(hash), equal_f(equal) {
        allocate_buckets(bucket_count);
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_t size() const noexcept {
        return _size;
    }

    size_t max_size() const noexcept {
        return -1;
    }

    void clear() {
        allocate_buckets(buckets.size());
    }

    mystd::pair<iterator, bool> insert(const value_type& val) {
        Key&& key = val.first;
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) 
            return {_insert(key, val.second, bucket)}; 
            
        return {end(), false};
    }

    mystd::pair<iterator, bool> insert(value_type&& val) {
        Key&& key = std::move(val.first);
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) 
            return {_insert(std::move(key), std::move(val.second), bucket)}; 
            
        return {end(), false};
    }

    template< class InputIt >
    void insert( InputIt first, InputIt last ) {
        for (; first != last; ++first) {
            const value_type& val = *first;
            insert(val);
        }
    }

    void insert( std::initializer_list<value_type> ilist ) {
        for(auto& val : ilist) {
            insert(val);
        }
    }

    template< class... Args >
    std::pair<iterator, bool> try_emplace( const Key& key, Args&&... args ) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) 
            return {_insert(key, bucket, std::forward<Args>(args)...)}; 
            
        return {end(), false};
    }

    iterator erase(iterator pos) {
        if(pos.map != this)
            throw std::out_of_range("Incorrect iterator");

        auto& bucket = buckets[pos.bucket_i];
        auto it = bucket.erase(pos.bucket_ind);
        return iterator(this, pos.bucket_i, it);
    }

    // const_iterator erase(iterator pos) {
    //     if(pos.map != this)
    //         throw std::out_of_range("Incorrect iterator");

    //     auto& bucket = buckets[pos.bucket_i];
    //     auto it = bucket.erase(pos.bucket_ind);
    //     return iterator(this, pos.bucket_i, it);
    // }
    
    iterator erase(iterator first, iterator last) {
        while(first != last) {
            auto next = erase(first);
            first = next;
        }
    }

    // const_iterator erase(const_iterator first, const_iterator last) {
    //     while(first != last) {
    //         auto next = erase(first);
    //         first = next;
    //     }
    // }

    T& at(const Key& key) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end())
            throw std::out_of_range("Key not found inside of unordered map");
        return found_iter->second;
    }

    const T& at(const Key& key) const {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end())
            throw std::out_of_range("Key not found inside of unordered map");
        return found_iter->second;
    }

    T& operator[](const Key& key) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end())
            return _insert(key, T());
        return found_iter->second;
    }

    T& operator[](Key&& key) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end())
            return _insert(key, T(), bucket);
        return found_iter->second;
    }

    iterator find(const Key& key) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        return found_iter == bucket.end() ? end() : found_iter;
    }

    iterator begin() {
        return iterator(this, 0, buckets[0].begin());
    }

    iterator end() {
        return iterator(this, buckets.size() - 1, buckets[buckets.size() - 1].begin());
    }

    bool contains(const Key& key) {
        iterator iter = find(key);
        return iter != end();
    }

    size_t count(const Key& key) {
        return contains(key) ? 1 : 0;
    }

private:    
    void allocate_buckets(size_t bucket_count) {
        buckets = mystd::vector<mystd::list<value_type>> (bucket_count);    
    }

    size_t get_bucket(size_t hash) const {
        return hash % buckets.size();
    }

    T& _insert(const Key& key,const T& t, mystd::list<value_type>& bucket) {
        bucket.push_front({key, t});
        _size++;
        return bucket.front().second;
    }

    T& _insert(Key&& key, T&& t, mystd::list<value_type>& bucket) {
        bucket.emplace_front(std::move(key), std::move(t));
        _size++;
        return bucket.front().second;
    }

    template< class... Args >
    T& _insert(Key&& key, mystd::list<value_type>& bucket, Args&&... args) {
        bucket.emplace_front(std::move(key), std::forward<Args>(args)...);
        _size++;
        return bucket.front().second;
    }

    mystd::pair<mystd::list<value_type>&, typename mystd::list<value_type>::iterator> get_bucket_and_iter(const Key& key) {
    
        size_t hash = hash_f(key);
        size_t bucket_i = get_bucket(hash);
        mystd::list<value_type>& bucket = buckets[bucket_i];
        auto found_iter = mystd::find_if(bucket.begin(), bucket.end(), [&](value_type& pair) {
            return equal_f(pair.first, key);
        });
        return {bucket, found_iter};
    }

    mystd::pair<const mystd::list<value_type>&, typename mystd::list<value_type>::const_iterator> get_bucket_and_iter(const Key& key) const{

        size_t hash = hash_f(key);
        size_t bucket_i = get_bucket(hash);
        const mystd::list<value_type>& bucket = buckets[bucket_i];
        auto found_iter = mystd::find_if(bucket.cbegin(), bucket.cend(), [&](value_type& pair) {
            return equal_f(pair.first, key);
        });
        return {bucket, found_iter};
    }

};

}
#endif //UNORDERED_MAP_HPP

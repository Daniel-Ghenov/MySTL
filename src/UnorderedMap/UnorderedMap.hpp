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
    size_t _size = 0;
    float max_load_factor = 2.0f;
public:
    class iterator;
    class const_iterator;

    class iterator {
    private:
        unordered_map* map;
        size_t bucket_ind = 0;
        typename mystd::list<value_type>::iterator bucket_it;

        void skip_empty_buckets(){
            while(bucket_ind < map->buckets.size() && bucket_it == map->buckets[bucket_ind].end()){
                ++bucket_ind;
                if(bucket_ind < map->buckets.size())
                    bucket_it = map->buckets[bucket_ind].begin();
                else
                    bucket_it = typename mystd::list<value_type>::iterator();
            }
        }

    public:

        iterator(unordered_map* map,
                 size_t bucket_ind,
                 typename mystd::list<value_type>::iterator bucket_it): map(map),
                                                                        bucket_ind(bucket_ind),
                                                                        bucket_it(bucket_it) {
            skip_empty_buckets();
        }

        iterator& operator++(){
            ++bucket_it;
            skip_empty_buckets();
            return *this;
        }
        iterator operator++(int a){
            iterator old = *this;
            operator++();
            return old;
        }
        iterator& operator--(){
            if(bucket_ind == map->buckets.size()){
                --bucket_ind;
                bucket_it = map->buckets[bucket_ind].end();
            }
            while(bucket_it == map->buckets[bucket_ind].begin()){
                if(bucket_ind == 0)
                    return *this;
                --bucket_ind;
                bucket_it = map->buckets[bucket_ind].end();
            }
            --bucket_it;
            return *this;
        }
        iterator operator--(int a){
            iterator old = *this;
            operator--();
            return old;
        }

        bool operator!=(const iterator& other) const{
            return !operator==(other);
        }

        bool operator==(const iterator& other) const{
            return map == other.map && bucket_ind == other.bucket_ind && bucket_it == other.bucket_it;
        }

        value_type& operator*(){
            return *bucket_it;
        }
        value_type* operator->(){
            return &(*bucket_it);
        }

        friend class unordered_map;
        friend class const_iterator;
    };

    class const_iterator {
    private:
        const unordered_map* map;
        size_t bucket_ind = 0;
        typename mystd::list<value_type>::const_iterator bucket_it;

        void skip_empty_buckets(){
            while(bucket_ind < map->buckets.size() && bucket_it == map->buckets[bucket_ind].cend()){
                ++bucket_ind;
                if(bucket_ind < map->buckets.size())
                    bucket_it = map->buckets[bucket_ind].cbegin();
                else
                    bucket_it = typename mystd::list<value_type>::const_iterator();
            }
        }

    public:

        const_iterator(const unordered_map* map,
                       size_t bucket_ind,
                       typename mystd::list<value_type>::const_iterator bucket_it): map(map),
                                                                       bucket_ind(bucket_ind),
                                                                       bucket_it(bucket_it) {
            skip_empty_buckets();
        }

        const_iterator(const iterator& other): map(other.map),
                                                bucket_ind(other.bucket_ind),
                                                bucket_it(other.bucket_it) {}

        const_iterator& operator++(){
            ++bucket_it;
            skip_empty_buckets();
            return *this;
        }
        const_iterator operator++(int a){
            const_iterator old = *this;
            operator++();
            return old;
        }
        const_iterator& operator--(){
            if(bucket_ind == map->buckets.size()){
                --bucket_ind;
                bucket_it = map->buckets[bucket_ind].cend();
            }
            while(bucket_it == map->buckets[bucket_ind].cbegin()){
                if(bucket_ind == 0)
                    return *this;
                --bucket_ind;
                bucket_it = map->buckets[bucket_ind].cend();
            }
            --bucket_it;
            return *this;
        }
        const_iterator operator--(int a){
            const_iterator old = *this;
            operator--();
            return old;
        }

        bool operator!=(const const_iterator& other) const{
            return !operator==(other);
        }

        bool operator==(const const_iterator& other) const{
            return map == other.map && bucket_ind == other.bucket_ind && bucket_it == other.bucket_it;
        }

        const value_type& operator*() const{
            return *bucket_it;
        }
        const value_type* operator->() const{
            return &(*bucket_it);
        }

        friend class unordered_map;
        friend class iterator;
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
        const Key& key = val.first;
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) {
            _insert(key, val.second, bucket);
            if (check_load_factor()) {
                return{find(val.first), true};
            }
            size_t bucket_i = get_bucket(hash_f(key));
            return {iterator(this, bucket_i, bucket.begin()), true};
        }
        return {end(), false};
    }

    mystd::pair<iterator, bool> insert(value_type&& val) {
        Key&& key = std::move(val.first);
        size_t bucket_i = get_bucket(hash_f(key));
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) {
            _insert(std::move(key), std::move(val.second), bucket);
            Key key_copy = bucket.begin()->first;
            if (check_load_factor()) {
                return{find(key_copy), true};
            }
            return {iterator(this, bucket_i, bucket.begin()), true};
        }
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
    mystd::pair<iterator, bool> try_emplace( const Key& key, Args&&... args ) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) {
            _insert(key, bucket, std::forward<Args>(args)...);
            if (check_load_factor()) {
                return {find(key), true};
            }
            size_t bucket_i = get_bucket(hash_f(key));
            return {iterator(this, bucket_i, bucket.begin()), true};
        }
        return {end(), false};
    }

    iterator erase(iterator pos) {
        if(pos.map != this)
            throw std::out_of_range("Incorrect iterator");

        auto& bucket = buckets[pos.bucket_ind];
        auto it = bucket.erase(pos.bucket_it);
        _size--;
        return iterator(this, pos.bucket_ind, it);
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
        return first;
    }

    // const_iterator erase(const_iterator first, const_iterator last) {
    //     while(first != last) {
    //         auto next = erase(first);
    //         first = next;
    //     }
    // }

    void swap(unordered_map& other) {
        std::swap(hash_f, other.hash_f);
        std::swap(equal_f, other.equal_f);
        std::swap(buckets, other.buckets);
        std::swap(_size, other._size);
    }

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
        if (found_iter == bucket.end()) {
            _insert(key, T(), bucket);
            if (check_load_factor()) {
                return find(key)->second;
            }
            return bucket.begin()->second;
        }
        return found_iter->second;
    }

    T& operator[](Key&& key) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end()) {
            _insert(key, T(), bucket);
            if (check_load_factor()) {
                return find(key)->second;
            }
            return bucket.begin()->second;
        }
        return found_iter->second;
    }

    iterator find(const Key& key) {
        auto [bucket, found_iter] = get_bucket_and_iter(key);
        if (found_iter == bucket.end())
            return end();
        return iterator(this, get_bucket(hash_f(key)), found_iter);
    }

    iterator begin() {
        return iterator(this, 0, buckets[0].begin());
    }

    iterator end() {
        return iterator(this, buckets.size(), typename mystd::list<value_type>::iterator());
    }

    const_iterator begin() const {
        return const_iterator(this, 0, buckets[0].cbegin());
    }

    const_iterator end() const {
        return const_iterator(this, buckets.size(), typename mystd::list<value_type>::const_iterator());
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
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
        _size = 0;
    }

    size_t get_bucket(size_t hash) const {
        return hash % buckets.size();
    }

    T& _insert(const Key& key,const T& t, mystd::list<value_type>& bucket) {
        bucket.push_front({key, t});
        ++_size;
        return bucket.front().second;
    }

    T& _insert(Key&& key, T&& t, mystd::list<value_type>& bucket) {
        bucket.emplace_front(std::move(key), std::move(t));
        ++_size;
        return bucket.front().second;
    }

    template< class... Args >
    T& _insert(const Key& key, mystd::list<value_type>& bucket, Args&&... args) {
        bucket.emplace_front(key, std::forward<Args>(args)...);
        ++_size;
        return bucket.front().second;
    }

    // Relocates an already-counted element into a (possibly new) bucket
    // during rehashing. Unlike _insert, this must not touch _size: the
    // element already exists, it's just moving buckets.
    void _relocate(Key&& key, T&& t, mystd::list<value_type>& bucket) {
        bucket.emplace_front(std::move(key), std::move(t));
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
        auto found_iter = mystd::find_if(bucket.cbegin(), bucket.cend(), [&](const value_type& pair) {
            return equal_f(pair.first, key);
        });
        return {bucket, found_iter};
    }

    bool check_load_factor() {
        float curr_load_factor = static_cast<float>(_size) / static_cast<float>(buckets.size());
        if (curr_load_factor - max_load_factor >= 0.000001f) {
            rehash(buckets.size() * 2);
            return true;
        }
        return false;
    }

    void rehash(size_t new_bucket_count) {
        mystd::vector<mystd::list<value_type>> new_buckets(new_bucket_count);
        for(auto& bucket: buckets) {
            auto it = bucket.begin();
            while(it != bucket.end()) {
                value_type& pair = *it;
                size_t new_bucket_i = hash_f(pair.first) % new_bucket_count;
                _relocate(std::move(pair.first), std::move(pair.second), new_buckets[new_bucket_i]);
                ++it;
            }
        }

        buckets = std::move(new_buckets);
    }

};

}
#endif //UNORDERED_MAP_HPP

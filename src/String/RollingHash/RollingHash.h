#ifndef MYSTD_STRING_ROLLINGHASH_ROLLINGHASH_H
#define MYSTD_STRING_ROLLINGHASH_ROLLINGHASH_H
#include "Helper.h"
#include "Const.h"
#include <cmath>

namespace mystd {

class rolling_hash{
private:
    size_t hash = 0;
    const size_t strLen = 0;
    const size_t primeFac = 0;

public:
    rolling_hash(const char* string, size_t size = 0);

    size_t get_hash();

    void push_front(char newChar);
    void pop_back();

    bool operator ==(const rolling_hash& other);
private:
    size_t get_prime(size_t number);
};

} // namespace mystd

#endif // MYSTD_STRING_ROLLINGHASH_ROLLINGHASH_H

#include "RollingHash.h"

namespace mystd {

rolling_hash::rolling_hash(const char* string, size_t strlen): primeFac(){
    if(!strlen){
        strlen = mystd::strlen(string);
    }
    for(size_t i {0}; i < strlen; i++){
        hash *= CHAR_SIZE;
        hash += string[i];
    }

}


size_t rolling_hash::get_hash(){
    return hash;
}


void rolling_hash::push_front(char newChar){
    hash = (hash * CHAR_SIZE) + newChar;

}

void rolling_hash::pop_back(){
    hash %= static_cast<int>(log(hash)/log(CHAR_SIZE));

}

bool rolling_hash::operator==(const rolling_hash& other){
    return hash == other.hash;
}
size_t rolling_hash::get_prime(size_t number){


}

} // namespace mystd

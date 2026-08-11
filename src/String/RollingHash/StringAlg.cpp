#include "StringAlg.h"

namespace mystd {

size_t find(const char* find, const char* findIn){
    size_t findLen = strlen(find);
    size_t findInLen = strlen(findIn);

    rolling_hash findInHash(findIn, findInLen);
    rolling_hash findHash(find, findInLen);

    for(size_t i {0}; i < findLen - findInLen;i++){
        if(findInHash == findHash)
            return i;
        else{
            findHash.pop_back();
            findHash.push_front(find[i + findInLen]);
        }
    }
    return -1;

}

} // namespace mystd

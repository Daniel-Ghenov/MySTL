#include "Helper.h"

namespace mystd {

int strcmp(const char* first, const char* second){
    if(!first || !second)
        throw std::logic_error("Cannot process nullptr");


    for(size_t i {0};;i++){
        if(first[i] < second[i]){
            return -1;
        }
        if(first[i] > second[i]){
            return 1;
        }
        if(first[i] == second[i] && first[i] == '\0')
            return 0;
    }

}
void strcpy(char* Destination, const char* Source){
    if(!Destination || !Source)
        throw std::logic_error("Cannot process nullptr");

    for(size_t i {0};; i++){
        Destination[i] = Source[i];
        if(Source[i] == '\0')
            return;
    }

}
void strcpy_s(char* Destination ,size_t size, const char* Source){
    if(!Destination || !Source)
        throw std::logic_error("Cannot process nullptr");

    for(size_t i {0}; i < size; i++){
        Destination[i] = Source[i];
        if(Source[i] == '\0')
            return;
    }
    Destination[size] = '\0';
}
void strcat(char* Destination, const char* Source){
    if(!Destination || !Source)
        throw std::logic_error("Cannot process nullptr");

    unsigned nullIdx = 0;
    while(Destination[nullIdx++] != '\0');
    nullIdx--;

    for(size_t i {0} ;; i++){
        if(Destination[i + nullIdx] == '\0')
            return;
        Destination[i + nullIdx] = Source[i];
        if(Source[i] == '\0')
            return;
    }

}
unsigned strlen(const char* str){
    if(!str)
        throw std::logic_error("Cannot process nullptr");

    unsigned length = 0;
    while(str[length++] != '\0');
    return length - 1;
}
size_t linear_find(const char* findIn, const char* toFind){
    if(!toFind || !findIn)
        throw std::logic_error("Cannot process nullptr");

    size_t toFindLen = strlen(toFind);
    size_t findInLen = strlen(findIn);

    for(size_t i {0}; i < findInLen; i++){
        for(size_t j {0}; j <= findInLen; j++){
            if(j == findInLen)
                return i;
            if(findIn[i + j] != toFind[j]){
                break;
            }

        }
    }
    return -1;
}

} // namespace mystd

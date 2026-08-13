#ifndef MYSTD_ALGORITHM_ALGORITHM_HPP
#define MYSTD_ALGORITHM_ALGORITHM_HPP

#include <cstddef>
#include <utility>
namespace mystd {

    template<class Iterator, class T>
    Iterator find(Iterator first, Iterator last, const T& find){    //returns iterator to first instance of find

        while (first != last){
           if(*first == find){
                return first;
           }
           first++;
        }
        return last;
    }


    template<class Iterator, class BoolFunc>
    Iterator find_if(Iterator first, Iterator last, BoolFunc condition){    //returns iterator to first instance to meet condition

        while(first != last){
            if(condition(*first)){
                return first;
            }
            first++;
        }
        return last;
    }


    template<class Iterator, class boolFunc>
    Iterator find_if_not(Iterator first, Iterator last, boolFunc condition){

        while(first != last){
            if(!condition(*first)){
                return first;
            }
            first++;
        }
        return last;
    }

    template<class Iterator, class T>
    int count(Iterator first, Iterator last, const T& find){ //returns the count of find

        int count = 0;
        while (first != last){
           if(*first == find){
                count++;
           }
           first++;
        }
        return count;
    }

    template<class Iterator, class BoolFunc>
    int count_if(Iterator first, Iterator last, BoolFunc condition){    //returns the count that matches condition

        int count = 0;
        while(first != last){
            if(condition(*first)){
                count++;
            }
            first++;
        }
        return count;
    }

    template<class Iterator, class UnaryFunc>
    void for_each(Iterator first, Iterator last, UnaryFunc f){

        while(first != last){
            f(*first);
            first++;
        }
    }


    template< class ForwardIt, class UnaryPred >
    ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPred p ) {
        size_t partition_index = mystd::count_if(first, last, p);
        ForwardIt second_part = first;
        for(size_t i{0uz}; i < partition_index; ++i) {
            second_part++;
        }
        ForwardIt rangeEnd = second_part;
        while(first != rangeEnd) {
            if(p(*first)) { 
                ++first;
                continue;
            }
            std::swap(*first, *second_part);
            second_part++;
        }
        return rangeEnd;
    }

} // namespace mystd

#endif // MYSTD_ALGORITHM_ALGORITHM_HPP

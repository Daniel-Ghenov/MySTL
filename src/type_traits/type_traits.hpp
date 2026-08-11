#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <functional>
namespace mystd {

    template<class T>
    struct unwrap_reference { using type = T; };

    template<class U>
    struct unwrap_reference<std::reference_wrapper<U>> { using type = U&; };

    template<class T>
    using unwrap_reference_t = typename unwrap_reference<T>::type;

    

}

#endif // TYPE_TRAITS_HPP

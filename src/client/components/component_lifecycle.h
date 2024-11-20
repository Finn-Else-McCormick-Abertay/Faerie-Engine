#pragma once

template <typename T>
class has_onadded
{
    typedef char yes[1];
    typedef char no[2];
    
    template <typename C>
    static yes& test(decltype(&C::__OnAdded));

    template <typename> static no& test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};
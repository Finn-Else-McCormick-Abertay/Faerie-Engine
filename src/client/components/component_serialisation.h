#pragma once

template <typename T>
class has_serialise
{
    typedef char yes[1];
    typedef char no[2];
    
    template <typename C>
    static yes& test(decltype(&C::__Serialise));

    template <typename> static no& test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename T>
class has_deserialise
{
    typedef char yes[1];
    typedef char no[2];
    
    template <typename C>
    static yes& test(decltype(&C::__Deserialise));

    template <typename> static no& test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};
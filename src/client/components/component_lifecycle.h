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

// __OnRemoved only gets called when Erase is called, not when the entity is destroyed. Don't put destructor logic in there, just stuff like removing related components
template <typename T>
class has_onremoved
{
    typedef char yes[1];
    typedef char no[2];
    
    template <typename C>
    static yes& test(decltype(&C::__OnRemoved));

    template <typename> static no& test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};
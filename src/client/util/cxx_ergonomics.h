#pragma once

#include <cxx.h>
#include <memory>

template<typename T>
class OptionalBox
{
public:
    OptionalBox(OptionalBox&&) = default;
    OptionalBox() = default;

    OptionalBox(rust::Box<T>&& rhs) noexcept : ppm_val(new rust::Box<T>(std::move(rhs))) {}
    
    OptionalBox& operator=(OptionalBox&& rhs) &noexcept {
        ppm_val = std::move(rhs.ppm_val);
        rhs.ppm_val = nullptr;
        return *this;
    }

    OptionalBox& operator=(rust::Box<T>&& rhs) &noexcept {
        ppm_val = new std::unique_ptr<rust::Box<T>>(rhs);
        return *this;
    }
    
    inline const T *operator->() const { return &(**ppm_val); }
    inline const T &operator*() const { return **ppm_val; }
    inline T *operator->() { return &(**ppm_val); }
    inline T &operator*() { return **ppm_val; }

    inline operator bool() const { return ppm_val; }

private:
    std::unique_ptr<rust::Box<T>> ppm_val;
};
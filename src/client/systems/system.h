#pragma once

#include <string>
#include <iostream>

#define SYSTEM_LOGGER_NAME(name) virtual std::string Name() const override { return #name; }

class ISystem
{
public:
    bool Init();
    void Shutdown();

    virtual std::string Name() const = 0;

protected:
    virtual bool InitImpl() = 0;
    virtual void ShutdownImpl() = 0;

    bool Initialised() const;

private:
    bool m_initialised = false;
};

inline std::ostream& operator <<(std::ostream &os, const ISystem &v) { os << v.Name(); return os; }
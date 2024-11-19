#pragma once

#include <string>
#include <iostream>

class ISystem
{
public:
    bool Init();
    void Shutdown();

protected:
    virtual bool InitImpl() = 0;
    virtual void ShutdownImpl() = 0;

    bool Initialised() const;

private:
    bool m_initialised = false;
};
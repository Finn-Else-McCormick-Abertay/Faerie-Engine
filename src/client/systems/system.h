#pragma once

class ISystem
{
public:
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
};
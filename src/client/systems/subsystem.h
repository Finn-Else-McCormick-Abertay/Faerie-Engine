#pragma once

class ISubsystem
{
public:
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
};
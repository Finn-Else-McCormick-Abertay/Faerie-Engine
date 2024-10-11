#pragma once

class ISystem
{
public:
    ~ISystem();

    bool Init();
    void Shutdown();

protected:
    virtual bool InitImpl() = 0;
    virtual void ShutdownImpl() = 0;

private:
    bool m_initialised = false;
};
#pragma once

#include <systems/system.h>
#include <functional>
#include <queue>
#include <map>
#include <util/type_name.h>

// Interface for logic which should be exposed to the render system but nothing else
class IDebugSystemInternal
{
protected:
    friend class IRenderSystem;

    virtual void __Internal_RunImGuiCallbacks() = 0;
};

class Debug final : public ISystem, public IDebugSystemInternal
{
public:
    static Debug& Instance();
    
    static void PushInstant(std::function<void()>&& callback);

    static void SetPersistent(const std::string& identifier, std::function<void()>&& callback);
    static void ClearPersistent(const std::string& identifier);

    template<typename T> static void SetPersistent(const std::string& identifier, std::function<void()>&& callback) {
        SetPersistent(type_name<T>() + "/" + identifier, std::move(callback));
    }
    template<typename T> static void ClearPersistent(const std::string& identifier) {
        ClearPersistent(type_name<T>() + "/" + identifier);
    }

private:
    Debug() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    virtual void __Internal_RunImGuiCallbacks() override;

    std::map<std::string, std::function<void()>> m_persistentCallbacks;
    std::queue<std::function<void()>> m_instantCallbackQueue;
};
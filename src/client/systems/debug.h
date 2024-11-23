#pragma once

#include <systems/system_lifecycle_declare.h>
#include <functional>
#include <queue>
#include <map>
#include <util/type_name.h>

// Interface for logic which should be exposed to the render system but nothing else
class IDebugSystemInternal
{
protected:
    friend class Renderer;
    virtual void __Internal_RunImGuiCallbacks() = 0;
};

class Debug final : public IDebugSystemInternal {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(Debug)
public:    
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
    virtual void __Internal_RunImGuiCallbacks() override;

    std::map<std::string, std::function<void()>> m_persistentCallbacks;
    std::queue<std::function<void()>> m_instantCallbackQueue;
};
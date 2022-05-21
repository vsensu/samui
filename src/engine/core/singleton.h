#pragma once

namespace samui
{
// do not use this in dll
template <typename T>
class Singleton
{
  public:
    virtual ~Singleton() {}

    template <typename... Args>
    static T& instance(Args&&... args)
    {
        // This is a thread-safe singleton.
        // c++ compiler will confirm the initialization of the local static
        // variable only once.
        static T* instance = new T(std::forward<Args>(args)...);

        return *instance;
    }
};

}  // namespace samui
#pragma once

#include <thread>

namespace ModernCMake
{
class A
{
    public:
    /// Constructor
    A();
    /// Destructor
    virtual ~A();
    /// Foo
    void foo();
    /// Bar
    void bar();

    private:
    /// value
    int m_value {0};
    /// thread
    std::thread m_thread;
};

} //namespace ModernCMake
#pragma once

#include <A.hpp>
#include <memory>

namespace ModernCMake
{

class B
{
public:

    B();
    virtual ~B();

    void CallFoo();

private:

std::shared_ptr<A> m_aPtr;


};
} // namespace ModernCMake
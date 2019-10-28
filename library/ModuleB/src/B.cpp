#include "B.hpp"

namespace ModernCMake
{

B::B()
{
    m_aPtr = std::make_shared<A>();
}

B::~B()
{
    m_aPtr.reset();
}

void B::CallFoo()
{

    m_aPtr->foo();
}

} //ModernCMake
#include "B.hpp"

#include <iostream>

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
    std::cout<<"[ModernCMake::B]: Call ModernCMake::A"<<std::endl;
    m_aPtr->foo();
}

} //ModernCMake
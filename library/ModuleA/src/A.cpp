#include "A.hpp"

#include <iostream>

namespace ModernCMake
{

A::A()
{

}

A::~A()
{

}

void A::foo()
{
    std::cout<<"Lanch process..."<<std::endl;
    m_thread = std::thread(&A::bar, this);

    m_thread.join();
}

void A::bar()
{
    // some useless process
    for(unsigned int i = 0; i < 10; ++i)
    {
        m_value++;
    }
    std::cout<<"Value: "<<m_value<<std::endl;
}

}// ModernCMake
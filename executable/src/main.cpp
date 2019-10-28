#include <ModernCMake/B.hpp>

#include <iostream>

int main(void)
{
    std::cout<<"[sampleExec]: Hello world !"<<std::endl;
    
    ModernCMake::B* foobar = new ModernCMake::B();

    foobar->CallFoo();

    delete foobar;

    return 0;
}
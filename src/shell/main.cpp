#include "Application.h"
#include <iostream>

int main()
{
    Application Application;

    if (!Application.Initialize())
    {
        std::cerr << "Failed to initialize\n";
        
        return 1;
    }

    Application.Run();

    return 0;
}
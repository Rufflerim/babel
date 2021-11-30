// Gaemi-00.cpp : Définit les fonctions exportées de la DLL.
//

#include "pch.h"
#include "framework.h"
#include "Gaemi.h"
#include <iostream>

// Il s'agit d'un exemple de fonction exportée.
GAPI int display(const str& message)
{
    std::cout << message << std::endl;
    return 0;
}


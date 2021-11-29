// Gaemi-00.cpp : Définit les fonctions exportées de la DLL.
//

#include "pch.h"
#include "framework.h"
#include "Gaemi-00.h"
#include <iostream>

// Il s'agit d'un exemple de variable exportée
GAPI int nGaemi00=0;

// Il s'agit d'un exemple de fonction exportée.
GAPI int fnGaemi00(void)
{
    std::cout << "Gaemi-00" << std::endl;
    return 0;
}

// Il s'agit du constructeur d'une classe qui a été exportée.
CGaemi00::CGaemi00()
{
    return;
}

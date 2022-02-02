/*
 * Plato_CLI_Main.cpp
 *
 *  Created on: Jan 28, 2022
 */

#ifndef NDEBUG
#include <fenv.h>
#endif

#include "Plato_AppCLI.hpp"
#include "Plato_TemplateMain.hpp"

/******************************************************************************/
int main(int aArgc, char **aArgv)
/******************************************************************************/
{
    Plato::Main<Plato::AppCLI>(aArgc, aArgv);
}
// main
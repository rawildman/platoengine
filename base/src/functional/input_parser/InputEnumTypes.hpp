#ifndef PLATO_INPUTENUMTYPES_HPP
#define PLATO_INPUTENUMTYPES_HPP

#include "EnumTable.hpp"
#include "EnumTypeHelpers.hpp"

// clang-format off
DECLARE_ENUM_SYMBOL_TABLE(CodeOptions,
                         (kSierraMassApp, "sierra_mass_app")
                         (kNodalSum, "nodal_sum")
                         (kCustomApp, "custom_app"))

DECLARE_ENUM_SYMBOL_TABLE(ObjectiveTypes,
                         (kMinimize, "minimize")
                         (kMaximize, "maximize"))

DECLARE_ENUM_SYMBOL_TABLE(FilterTypes,
                         (kIdentity, "identity")
                         (kHelmholtz, "helmholtz")
                         (kKernel, "kernel"))
// clang-format on

#endif

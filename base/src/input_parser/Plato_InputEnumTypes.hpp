#ifndef PLATO_INPUTENUMTYPES_HPP
#define PLATO_INPUTENUMTYPES_HPP

#include "Plato_EnumTypeHelpers.hpp"
#include "Plato_EnumTable.hpp"

// clang-format off
#include "Plato_SuppressBoostNvccWarnings.hpp"
// clang-format on

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

#include "Plato_RestoreBoostNvccWarnings.hpp"

#endif

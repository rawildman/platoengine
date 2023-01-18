#pragma once

#include "Plato_Utils.hpp"

namespace Plato
{
using StageName = Utils::NamedType<std::string, struct StageNameTag>;
using OperationName = Utils::NamedType<std::string, struct OperationNameTag>;
using ParameterName = Utils::NamedType<std::string, struct ParameterNameTag>;
}
/*
//@HEADER
// *************************************************************************
//   Plato Engine v.1.0: Copyright 2018, National Technology & Engineering
//                    Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Sandia Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact the Plato team (plato3D-help@sandia.gov)
//
// *************************************************************************
//@HEADER
*/

#include <gtest/gtest.h>
#include "Plato_InputData.hpp"
#include "Plato_Parser.hpp"

namespace PlatoTestEngineDataParser
{
TEST(PlatoTestEngineDataParser, ParseOptimizerData)
{
  const std::string tInput =
  "<OptimizationVariables>\n"
  "  <ValueName>Control</ValueName>\n"
  "  <InitializationStage>Initial Guess</InitializationStage>\n"
  "  <FilteredName>Topology</FilteredName>\n"
  "  <LowerBoundValueName>Lower Bound Value</LowerBoundValueName>\n"
  "  <LowerBoundVectorName>Lower Bound Vector</LowerBoundVectorName>\n"
  "  <UpperBoundValueName>Upper Bound Value</UpperBoundValueName>\n"
  "  <UpperBoundVectorName>Upper Bound Vector</UpperBoundVectorName>\n"
  "  <SetLowerBoundsStage>Set Lower Bounds</SetLowerBoundsStage>\n"
  "  <SetUpperBoundsStage>Set Upper Bounds</SetUpperBoundsStage>\n"
  "  <StochasticParametersName>Stochastic Parameters</StochasticParametersName>\n"
  "</OptimizationVariables>\n";

  const Plato::PugiParser tParser;
  const Plato::InputData tInputData = tParser.parseString(tInput);

  Plato::OptimizerEngineStageData tEngineData;
  Plato::Parse::parseOptimizationVariablesNames(tInputData, tEngineData);

  EXPECT_EQ(tEngineData.getNumControlVectors(), 1); // ValueName
  EXPECT_EQ(tEngineData.getControlName(0), "Control"); // ValueName
  EXPECT_EQ(tEngineData.getInitializationStageName(), "Initial Guess"); // InitializationStage
  EXPECT_EQ(tEngineData.getFilteredControlName(0), "Topology"); // FilteredName
  EXPECT_EQ(tEngineData.getLowerBoundValueName(), "Lower Bound Value"); // LowerBoundValueName
  EXPECT_EQ(tEngineData.getLowerBoundVectorName(), "Lower Bound Vector"); // LowerBoundVectorName
  EXPECT_EQ(tEngineData.getUpperBoundValueName(), "Upper Bound Value"); // UpperBoundValueName
  EXPECT_EQ(tEngineData.getUpperBoundVectorName(), "Upper Bound Vector"); // UpperBoundVectorName
  EXPECT_EQ(tEngineData.getSetLowerBoundsStageName(), "Set Lower Bounds"); // SetLowerBoundsStage
  EXPECT_EQ(tEngineData.getSetUpperBoundsStageName(), "Set Upper Bounds"); // SetUpperBoundsStage
  EXPECT_EQ(tEngineData.getStochasticParametersName(), "Stochastic Parameters");
}
} // end PlatoTestInputData namespace

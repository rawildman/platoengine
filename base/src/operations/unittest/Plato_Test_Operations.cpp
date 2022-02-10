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

#include "Plato_Utils.hpp"
#include "Plato_InputData.hpp"
#include "Plato_SystemCall.hpp"
#include "Plato_InitializeValues.hpp"
#include "Plato_HarvestDataFromFile.hpp"
#include "Plato_OperationsUtilities.hpp"

//#include <aprepro.h>

#include <fstream>

namespace PlatoTestOperations
{

TEST(LocalOperation, SystemCall)
{
    Plato::InputData tInputNode("Operation");
    tInputNode.add<std::string>("Command", "cd evaluations0; aprepro");
    tInputNode.add<std::string>("Name", "aprepro_0");
    tInputNode.add<std::string>("OnChange", "true");
    tInputNode.add<std::string>("AppendInput", "true");
    tInputNode.add<std::string>("Argument", "matched.yaml.template matched.yaml");
    tInputNode.add<std::string>("Argument", "-q");
    tInputNode.add<std::string>("Option", "r=");
    tInputNode.add<std::string>("Option", "h=");

    Plato::InputData tInput("Input");
    tInput.add<std::string>("ArgumentName", "Parameters_0");
    tInputNode.add<Plato::InputData>("Input", tInput);

    std::cout << "1\n";
    Plato::SystemCall tSystemCall(nullptr, tInputNode);
    std::cout << "2\n";

    EXPECT_STREQ("aprepro_0", tSystemCall.name().c_str());
    EXPECT_STREQ("cd evaluations0; aprepro", tSystemCall.command().c_str());
    EXPECT_TRUE(tSystemCall.onChange());
    EXPECT_TRUE(tSystemCall.appendInput());

    auto tOptions = tSystemCall.options();
    std::vector<std::string> tGoldOptions = {"r=", "h="};
    for(auto& tOption : tOptions)
    {
        auto tItr = std::find(tGoldOptions.begin(), tGoldOptions.end(), tOption);
        EXPECT_TRUE(tItr != tGoldOptions.end());
    }

    auto tArguments = tSystemCall.arguments();
    std::vector<std::string> tGoldArguments = {"matched.yaml.template matched.yaml", "-q"};
    for(auto& tArgument : tArguments)
    {
        auto tItr = std::find(tGoldArguments.begin(), tGoldArguments.end(), tArgument);
        EXPECT_TRUE(tItr != tGoldArguments.end());
    }

    auto tInputs = tSystemCall.inputNames();
    std::vector<std::string> tGoldInputs = {"matched.yaml.template matched.yaml", "-q"};
    for(auto& tInput : tInputs)
    {
        auto tItr = std::find(tGoldInputs.begin(), tGoldInputs.end(), tInput);
        EXPECT_TRUE(tItr != tGoldInputs.end());
    }
}

TEST(LocalOperation, read_table_1)
{
    std::ofstream tOutFile;
    tOutFile.open("test.txt");
    std::stringstream ss;
    ss << "#f[Hz]  SE[dB]\n";
    tOutFile << ss.str();
    tOutFile.close();

    // no numerical values in table - outcome: empty table
    std::vector<std::vector<double>> tTable;
    Plato::read_table("test.txt", tTable);
    EXPECT_EQ(0, tTable.size());

    auto tTrash = std::system("rm -f test.txt");
    Plato::Utils::ignore_unused(tTrash);
}

TEST(LocalOperation, read_table_2)
{
    std::ofstream tOutFile;
    tOutFile.open("test.txt");
    std::stringstream ss;
    ss << "#f[Hz]  SE[dB]\n    1000000000    39.00415657\n    1010000000    38.93005158\n    1020000000    38.85582205";
    tOutFile << ss.str();
    tOutFile.close();

    // no numerical values in table - outcome: empty table
    std::vector<std::vector<double>> tTable;
    Plato::read_table("test.txt", tTable);
    
    auto tTol = 1e-6;
    std::vector<std::vector<double>> tGold = 
        {{1000000000, 39.00415657},{1010000000, 38.93005158},{1020000000, 38.85582205}};
    EXPECT_EQ(3, tTable.size());
    for(auto& tRow : tTable)
    {
        EXPECT_EQ(2, tRow.size());
        auto tRowI = &tRow - &tTable[0];
        for(auto& tCol : tRow)
        {
            auto tColI = &tCol - &tRow[0];
            EXPECT_NEAR(tGold[tRowI][tColI], tCol, tTol);
            //printf("X(%d,%d)=%f ", tRowI, tColI, tCol);
        }
        //printf("\n");
    }

    auto tTrash = std::system("rm -f test.txt");
    Plato::Utils::ignore_unused(tTrash);
}

TEST(LocalOperation, parse_tokens)
{
    std::vector<char> tBuffer = {'#', 'f', '[', 'H', 'z', ']', ' ', 'S', 'E', '[', 'd', 'B', ']'};
    std::vector<std::string> tTokens;
    Plato::parse_tokens(tBuffer.data(), tTokens);
    EXPECT_EQ(2u, tTokens.size());
    EXPECT_STREQ("#f[Hz]", tTokens[0].c_str());

    // remove garbage from last token
    if(tTokens[1].size() > 6u)
    {
        tTokens[1].erase(tTokens[1].begin()+6, tTokens[1].end());
        //std::cout << tTokens[1] << "\n";
    }

    EXPECT_STREQ("SE[dB]", tTokens[1].c_str());
}

TEST(LocalOperation, compute)
{
    Plato::Table tTable;
    tTable.mCol = 1;
    tTable.mData = 
        {{1000000000, 39.00415657}, {1010000000, 38.93005158}, {1020000000, 38.85582205},
         {1030000000, 38.78149587}, {1040000000, 38.70709947}, {1050000000, 38.63265785} };

    // test max
    auto tTol = 1e-4;
    tTable.mOperation = "max";
    auto tOutput = Plato::compute(tTable);
    EXPECT_NEAR(39.00415657, tOutput, tTol);

    // test min
    tTable.mOperation = "min";
    tOutput = Plato::compute(tTable);
    EXPECT_NEAR(38.63265785, tOutput, tTol);

    // test min
    tTable.mOperation = "sum";
    tOutput = Plato::compute(tTable);
    EXPECT_NEAR(232.91128339, tOutput, tTol);
}

TEST(LocalOperation, HarvestDataFromFile)
{
    Plato::InputData tInputNode("Operation");
    tInputNode.add<std::string>("Column", "1");
    tInputNode.add<std::string>("Operation", "Max");
    tInputNode.add<std::string>("Name", "Harvest Gemma Data");
    tInputNode.add<std::string>("Function", "HarvestDataFromFile");
    tInputNode.add<std::string>("File", "matched_power_balance.dat");

    Plato::InputData tInput("Input");
    tInput.add<std::string>("ArgumentName", "shape_parameters");
    tInputNode.add<Plato::InputData>("Input", tInput);

    Plato::InputData tOutput("Output");
    tOutput.add<std::string>("ArgumentName", "criterion_value");
    tInputNode.add<Plato::InputData>("Output", tOutput);

    Plato::HarvestDataFromFile tOperation(nullptr, tInputNode);

    // test single-value inputs
    EXPECT_STREQ("1", tOperation.column().c_str());
    EXPECT_STREQ("Max", tOperation.operation().c_str());
    EXPECT_STREQ("Harvest Gemma Data", tOperation.name().c_str());
    EXPECT_STREQ("HarvestDataFromFile", tOperation.function().c_str());
    EXPECT_STREQ("matched_power_balance.dat", tOperation.file().c_str());

    // test input/output arguments 
    std::vector<Plato::LocalArg> tLocalArgs;
    tOperation.getArguments(tLocalArgs);
    EXPECT_EQ(2u, tLocalArgs.size());

    std::vector<std::string> tGoldNames = {"shape_parameters", "criterion_value"};
    for(auto tValue : tLocalArgs)
    {
        EXPECT_EQ(Plato::data::SCALAR, tValue.mLayout);
        auto tItr = std::find(tGoldNames.begin(), tGoldNames.end(), tValue.mName);
        EXPECT_TRUE(tItr != tGoldNames.end());
    }

    // test operator()()
    std::ofstream tOutFile;
    tOutFile.open("test.txt");
    std::stringstream tSS;
    tSS << "#f[Hz]  SE[dB]\n    1000000000    39.00415657\n    1010000000    38.93005158\n    1020000000    38.85582205";
    tOutFile << tSS.str();
    tOutFile.close();
    tOperation.unittest(true);
    tOperation.operator()();
    auto tResult = tOperation.result();
    auto tTol = 1e-4;
    EXPECT_NEAR(39.00415657, tResult, tTol);
    auto tTrash = std::system("rm -f test.txt");
    Plato::Utils::ignore_unused(tTrash);

/*
    SEAMS::Aprepro tFileObject;
    bool tResult = tFileObject.parse_file("matched_power_balance.dat");
    if ( tResult ) 
    {
        std::cout << "PARSING RESULTS: \n" << tFileObject.parsing_results().str();
    }

    SEAMS::Aprepro tDumpFuncObject;
    tResult = tDumpFuncObject.parse_string("{DUMP_FUNC()}");
    if ( tResult ) 
    {
        std::cout << tDumpFuncObject.parsing_results().str() << "\n";
    }
    
    SEAMS::Aprepro tFuncObject;
    std::string tfodge("{abs(-2)*exp(8)}");
    tResult = tFuncObject.parse_string(tfodge);
    if ( tResult ) 
    {
        std::cout << "PARSING RESULTS: " << tFuncObject.parsing_results().str() << "\n";
        std::cout << std::stod(tFuncObject.parsing_results().str()) << "\n";
    }

    SEAMS::Aprepro tArrayObject;
    std::string tArray = std::string("{a = sqrt(4)}\n{a*a}\n{array_a = make_array(1,3,1)}") + 
        "\n{print_array(array_a)}\n{array_b = make_array(1,3,3)}\n{sol=array_a+array_b}" +
        "\n{print_array(sol)}";
    tResult = tArrayObject.parse_string(tArray);
    if ( tResult ) 
    {
        std::cout << "PARSING RESULTS: " << tArrayObject.parsing_results().str() << "\n";
    }
    */
}

TEST(InitializeValues, getValuesFromCSMFile)
{
    Plato::InputData tNode;
    Plato::InitializeValues tIntializeValuesOperation(NULL, tNode);

    std::istringstream tStringStream;
    std::string tStringInput;

    tStringInput =
            "# Constant, Design, and Output Parameters:\n"
            "despmtr   Px       2.00000 lbound 1.5  ubound 2.5\n"
            "despmtr   Py       2.00000 lbound 1.5  ubound 2.5\n"
            "despmtr   Pz       2.00000 lbound 1.5  ubound 2.5\n"
            "despmtr   Lx       5.00000 lbound 3.0  ubound 10.0\n"
            "despmtr   Rx       8.00000 lbound 3.0  ubound 10.0\n"
            "despmtr   Rcap     1.00000 lbound 1.5  ubound 2.5\n"
            "despmtr   Boffset  0.25000 lbound 0.05 ubound 0.49\n"
            "despmtr   Rp       0.70000 lbound 0.5  ubound 0.9\n"
            "#despmtr   Rp       0.70000 lbound 0.5  ubound 0.9*Px\n"
            "despmtr   Rpo      0.50000 lbound 0.1  ubound 0.9\n"
            "#despmtr   Rpo      0.50000 lbound 0.1  ubound 0.9*Rp\n"
            "despmtr   Rlo      0.30000 lbound 0.1  ubound 0.9\n"
            "#despmtr   Rlo      0.30000 lbound 0.1  ubound 0.9*Px/4.0\n"
            "despmtr   Po       0.20000 lbound 0.1  ubound 0.5\n";
    tStringStream.str(tStringInput);
    tIntializeValuesOperation.getValuesFromStream(tStringStream);
    EXPECT_EQ(tIntializeValuesOperation.getValue(0), 2.0);
    EXPECT_EQ(tIntializeValuesOperation.getValue(1), 2.0);
    EXPECT_EQ(tIntializeValuesOperation.getValue(2), 2.0);
    EXPECT_EQ(tIntializeValuesOperation.getValue(3), 5.0);
    EXPECT_EQ(tIntializeValuesOperation.getValue(4), 8.0);
    EXPECT_EQ(tIntializeValuesOperation.getValue(5), 1.0);
    EXPECT_EQ(tIntializeValuesOperation.getValue(6), .25);
    EXPECT_EQ(tIntializeValuesOperation.getValue(7), .7);
    EXPECT_EQ(tIntializeValuesOperation.getValue(8), .5);
    EXPECT_EQ(tIntializeValuesOperation.getValue(9), .3);
    EXPECT_EQ(tIntializeValuesOperation.getValue(10), .2);

    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(0), 2.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(1), 2.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(2), 2.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(3), 10.0);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(4), 10.0);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(5), 2.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(6), .49);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(7), .9);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(8), .9);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(9), .9);
    EXPECT_EQ(tIntializeValuesOperation.getValueUpperBound(10), .5);

    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(0), 1.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(1), 1.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(2), 1.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(3), 3.);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(4), 3.0);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(5), 1.5);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(6), .05);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(7), .5);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(8), .1);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(9), .1);
    EXPECT_EQ(tIntializeValuesOperation.getValueLowerBound(10), .1);
}


} // end PlatoTestOperations namespace

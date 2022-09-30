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
#include <fstream>
#include <string>
#include <sstream>
#include <Plato_FreeFunctions.hpp>

#include "Plato_CopyValue.hpp"
#include "Serializable.hpp"

#include "Plato_LocalOperation.hpp"

#include "Plato_Filter.hpp"
#include "Plato_CopyField.hpp"
#include "Plato_CopyValue.hpp"
#include "Plato_Roughness.hpp"

#include "Plato_SystemCallOperation.hpp"
#include "Plato_Aggregator.hpp"
#include "Plato_DesignVolume.hpp"
#include "Plato_Reinitialize.hpp"
#include "Plato_EnforceBounds.hpp"
#include "Plato_UpdateProblem.hpp"
#include "Plato_ComputeVolume.hpp"
#include "Plato_CSMMeshOutput.hpp"
#include "Plato_SetUpperBounds.hpp"
#include "Plato_SetLowerBounds.hpp"
#include "Plato_PlatoMainOutput.hpp"
#include "Plato_InitializeField.hpp"
#include "Plato_InitializeValues.hpp"
#include "Plato_WriteGlobalValue.hpp"
#include "Plato_CSMParameterOutput.hpp"
#include "Plato_HarvestDataFromFile.hpp"
#include "Plato_OperationsUtilities.hpp"
#include "Plato_NormalizeObjectiveValue.hpp"
#include "Plato_MeanPlusVarianceMeasure.hpp"
#include "Plato_MeanPlusVarianceGradient.hpp"
#include "Plato_ReciprocateObjectiveValue.hpp"
#include "Plato_NormalizeObjectiveGradient.hpp"
#include "Plato_OutputNodalFieldSharedData.hpp"
#include "Plato_ReciprocateObjectiveGradient.hpp"

BOOST_CLASS_IMPLEMENTATION(Plato::SetLowerBounds, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::SetLowerBounds, boost::serialization::track_never )

/*
BOOST_CLASS_IMPLEMENTATION(Plato::Aggregator, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::Aggregator, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::ComputeVolume, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::ComputeVolume, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::CopyField, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::CopyField, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::CopyValue, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::CopyValue, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::DesignVolume, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::DesignVolume, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::EnforceBounds, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::EnforceBounds, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::Filter, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::Filter, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::HarvestDataFromFile, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::HarvestDataFromFile, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::OutputNodalFieldSharedData, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::OutputNodalFieldSharedData, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::InitializeField, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::InitializeField, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::InitializeValues, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::InitializeValues, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::PlatoMainOutput, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::PlatoMainOutput, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::SetLowerBounds, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::SetLowerBounds, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::SetUpperBounds, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::SetUpperBounds, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::SystemCallOperation, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::SystemCallOperation, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::UpdateProblem, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::UpdateProblem, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::Roughness, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::Roughness, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::Reinitialize, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::Reinitialize, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::CSMMeshOutput, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::CSMMeshOutput, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::WriteGlobalValue, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::WriteGlobalValue, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::CSMParameterOutput, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::CSMParameterOutput, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::NormalizeObjectiveGradient, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::NormalizeObjectiveGradient, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::NormalizeObjectiveValue, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::NormalizeObjectiveValue, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::MeanPlusVarianceGradient, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::MeanPlusVarianceGradient, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::MeanPlusVarianceMeasure, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::MeanPlusVarianceMeasure, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::ReciprocateObjectiveGradient, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::ReciprocateObjectiveGradient, boost::serialization::track_never )
BOOST_CLASS_IMPLEMENTATION(Plato::ReciprocateObjectiveValue, boost::serialization::object_serializable )
BOOST_CLASS_TRACKING(Plato::ReciprocateObjectiveValue, boost::serialization::track_never )
*/
using namespace Plato;

/*
void load(CopyValue& tCopy, std::string aFilename)                                        
    {                                                                       
        std::fstream tInFileStream;
        tInFileStream.open(aFilename.c_str(),std::fstream::in);                     
                
        boost::archive::text_iarchive tInputArchive(tInFileStream);       
        tInputArchive >> boost::serialization::make_nvp("CopyValue", tCopy);
        tInFileStream.close(); 
    }                                           
		                                  
	void save(CopyValue& tCopy, std::string aFilename)                                        
    {                                                                       
        std::ofstream tOutFileStream(aFilename.c_str());                    
        boost::archive::text_oarchive tOutputArchive(tOutFileStream);       
        tOutputArchive << boost::serialization::make_nvp("CopyValue", tCopy);             
        tOutFileStream.close();                                             
    }     
*/
template<class Archive,class Serial>
void save(Serial& tS, std::string aFilename)
{                                                                       
    std::ofstream tOutFileStream(aFilename.c_str());                    
    Archive tOutputArchive(tOutFileStream, boost::archive::no_header | boost::archive::no_tracking);       
    tOutputArchive << boost::serialization::make_nvp("Serial", tS);             
}         

template<class Archive, class Serial>
void load(Serial& tS, std::string aFilename)
{                                                                       
    std::fstream tInFileStream;
    tInFileStream.open(aFilename.c_str(),std::fstream::in);
    Archive tInputArchive(tInFileStream, 1);       
    tInputArchive >> boost::serialization::make_nvp("Serial", tS);
}        

template<class Serial>
bool serializeEquals(Serial& aSerialOne, Serial& aSerialTwo)
{
    save<boost::archive::text_oarchive>(aSerialOne,"out");
    save<boost::archive::text_oarchive>(aSerialTwo,"out2");

    std::ifstream tInfile("out");
    std::stringstream tStringstream;
    tStringstream << tInfile.rdbuf();
    auto tString = tStringstream.str();
    
    std::ifstream tInfile2("out2");
    std::stringstream tStringstream2;
    tStringstream2 << tInfile2.rdbuf();
    auto tString2 = tStringstream2.str();
    
    Plato::system("rm -rf out");
    Plato::system("rm -rf out2");

    return tString.compare(tString2) == 0;
}

Plato::FixedBlock::Metadata generateFixedBlockMetadata()
{
    Plato::FixedBlock::Metadata tMeta;
    tMeta.mOptimizationBlockValue = 1;
    tMeta.mBoundaryValues = {0.0, 0.1, 0.2};
    tMeta.mDomainValues = {-1.0, -2.0};
    tMeta.mMaterialStates = {"state1", "state2"};
    tMeta.mBlockIDs = {1 , 2 , 3, 4};
    tMeta.mSidesetIDs = {5, 6, 7};
    tMeta.mNodesetIDs = {8, 9};
    return tMeta;
}

TEST(BoostSerialization, SetLowerBounds)
{
    auto tFixedBlockMetadata = generateFixedBlockMetadata();

    Plato::SetLowerBounds tOperation("shape",
                                     "fluid",
                                     tFixedBlockMetadata,
                                     Plato::data::layout_t::SCALAR_FIELD,
                                     3,
                                     4);
    Plato::SetLowerBounds tOperation2;

    save<boost::archive::xml_oarchive>(tOperation,"out.xml");
    load<boost::archive::xml_iarchive>(tOperation2,"out.xml");
    Plato::system("rm -rf out.xml");

    EXPECT_TRUE(serializeEquals(tOperation,tOperation2));
}

TEST(BoostSerialization, SetUpperBounds)
{
    auto tFixedBlockMetadata = generateFixedBlockMetadata();

    Plato::SetUpperBounds tOperation("shape",
                                     "fluid",
                                     tFixedBlockMetadata,
                                     Plato::data::layout_t::SCALAR_FIELD,
                                     3,
                                     4);
    Plato::SetUpperBounds tOperation2;

    save<boost::archive::xml_oarchive>(tOperation,"out.xml");
    load<boost::archive::xml_iarchive>(tOperation2,"out.xml");
    Plato::system("rm -rf out.xml");

    EXPECT_TRUE(serializeEquals(tOperation,tOperation2));
}

TEST(BoostSerialization, CopyValue)
{
    Plato::CopyValue tOperation("in",
                                "out");
    Plato::CopyValue tOperation2;

    save<boost::archive::xml_oarchive>(tOperation,"out.xml");
    load<boost::archive::xml_iarchive>(tOperation2,"out.xml");
    Plato::system("rm -rf out.xml");

    EXPECT_TRUE(serializeEquals(tOperation,tOperation2));
}

TEST(BoostSerialization, CopyField)
{
    Plato::CopyField tOperation("in",
                                "out");
    Plato::CopyField tOperation2;

    save<boost::archive::xml_oarchive>(tOperation,"out.xml");
    load<boost::archive::xml_iarchive>(tOperation2,"out.xml");
    //Plato::system("rm -rf out.xml");

    EXPECT_TRUE(serializeEquals(tOperation,tOperation2));
}
